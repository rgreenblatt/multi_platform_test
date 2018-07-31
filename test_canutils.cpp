#include "ctre/phoenix/Platform/Platform.h"
#include <string>
#include <iostream>
#include <cstring>
#include <chrono>
#include <thread>
#include <map>
#include <math.h>

#ifdef WINDOWS_CAN_TEST
    //Do this
#else
    #include "Platform-linux-socket-can.h"
#endif

#include <atomic>

static std::atomic<bool> term; 

void StreamThread(uint64_t iterNum) {
    
    std::map<uint32_t, uint32_t> errors; 

    uint32_t handle = 0;

    int32_t err = 0;

    ctre::phoenix::platform::can::CANComm_OpenStreamSession(&handle, 0x0101, 0x1fffffff, 20, &err); 
    
    std::chrono::steady_clock::time_point startTime = std::chrono::steady_clock::now();
    std::chrono::steady_clock::time_point lastTime = std::chrono::steady_clock::now();

    while(1) {
        if(term) {
            return;
        }

        if(err != 0) {
            auto iter = errors.find(err);

            if (iter == errors.end())
            {
                errors.insert(std::pair<uint32_t, uint32_t>(err, 0));

                iter = errors.find(err);
            }

            iter->second++;

        }

        ctre::phoenix::platform::can::canframe_t readFrame;

        uint32_t read = 0;
    
        ctre::phoenix::platform::can::CANComm_ReadStreamSession(handle, &readFrame, 1, &read, &err);

        if(read) {
            uint64_t checkNum = 0;
            
            std::memcpy(&checkNum, readFrame.data, readFrame.dlc);
            
            if(checkNum == iterNum) {
                iterNum++;
                
                uint8_t data[8];
                
                std::memcpy(data, &iterNum, 8);

                ctre::phoenix::platform::can::CANbus_SendFrame(0x0101, data, 8); 
                
                iterNum++;
           } 
        }
        
        std::chrono::steady_clock::time_point endTime = std::chrono::steady_clock::now();
        
        if(std::chrono::duration_cast<std::chrono::milliseconds>(endTime - lastTime).count() >= 1000) {
            
            double timeTotal = ((double) std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime).count()) / 1000.0;

            lastTime = endTime;

            for (auto iter = errors.begin(); iter != errors.end(); ++iter)
            {
                std::cout << "code: " << iter->first << " num: " << iter->second << " total time (s): " << round(timeTotal) << " avg num per s: " <<  ((double) iter->second) / timeTotal << std::endl;

            }
        }
    }
}

int main(int argc, char *argv[]) {
    
    ctre::phoenix::platform::can::SetCANInterface(argv[1]);

    uint64_t iterNum = argv[2][0] - '0';
    int arbOut = argv[3][0] - '0';
    int arbIn = argv[4][0] - '0';

    std::thread streamTest = std::thread(&StreamThread, iterNum);

    int err = 0;
    
    uint64_t val = 0x1122334455667788;

    uint8_t data[8];

    std::memcpy(data, &val, 8);

    ctre::phoenix::platform::can::CANComm_SendMessage(0x0200 + arbOut, data, 8, 20, &err);

    std::this_thread::sleep_for(std::chrono::milliseconds(100));

    if(err) {
        std::cout << "ERROR SEND: " << err << std::endl;
    }
    
    uint32_t ID = 0x0200 + arbIn;

    uint32_t stampLast = 0;

    while(1) {
        std::this_thread::sleep_for(std::chrono::milliseconds(20));

        uint8_t size = 0;

        uint32_t stamp = 0;

        ctre::phoenix::platform::can::CANComm_ReceiveMessage(&ID, 0x1fffffff, data, &size, &stamp, &err); 
        
        if(err) {
            std::cout << "ERROR REC: " << err << std::endl;
        }
        
    
        uint64_t checkVal = 0;

        std::memcpy(&checkVal, data, size); 

        if(checkVal != val || size != 8 || stampLast >= stamp) {
            std::cout << "BAD DATA: " << checkVal << " SIZE: " << unsigned(size) << " STAMP: " << stamp << " STAMPLAST: " << stampLast << std::endl;

        }
    }

    term.store(true);

    std::this_thread::sleep_for(std::chrono::milliseconds(100));

    return 0;
}
