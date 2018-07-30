#include "ctre/phoenix/Platform/Platform.h"
#include <string>
#include <cstring>
#include <chrono>
#include <thread>
#include <iostream>
#include <math.h>

#ifdef WINDOWS_CAN_TEST
    //Do this
#else
    #include "Platform-linux-socket-can.h"
#endif

int main(int argc, char *argv[]) {

    ctre::phoenix::platform::can::SetCANInterface(argv[1]);
    
    uint64_t lastCheckNum = 0;
    
    uint64_t initialCheckNum = 0;

    std::chrono::steady_clock::time_point startTime = std::chrono::steady_clock::now();
    std::chrono::steady_clock::time_point lastTime = std::chrono::steady_clock::now();

    while(1) {

        ctre::phoenix::platform::can::canframe_t readFrame;

        ctre::phoenix::platform::can::CANbus_ReceiveFrame(readFrame);

        if(readFrame.arbID == 0x0101) {
            uint64_t checkNum = 0;
            
            std::memcpy(&checkNum, readFrame.data, readFrame.dlc);
        
    
            std::chrono::steady_clock::time_point endTime = std::chrono::steady_clock::now();
            
            if(std::chrono::duration_cast<std::chrono::milliseconds>(endTime - lastTime).count() >= 1000) {
            
                double timeTotal = ((double) std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime).count()) / 1000.0;

                std::cout << "count per s recent: " << checkNum - lastCheckNum << "\ttotal count: " 
                          << checkNum - initialCheckNum << "\t\ttotal time (s): " << round(timeTotal) 
                          << "\t\tavg count per s: " <<  (checkNum - initialCheckNum) / timeTotal <<  std::endl;
        
                lastTime = endTime;

                lastCheckNum = checkNum;
            }


            
            if(initialCheckNum == 0) {
                initialCheckNum = checkNum;
                lastCheckNum = checkNum;
            }
        }
        else {
            std::cout << "else on bus" << std::endl;
        }
    }

    return 0;
}



