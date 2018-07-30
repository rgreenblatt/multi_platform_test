#include "ctre/phoenix/Platform/Platform.h"
#include <string>
#include <cstring>

#ifdef WINDOWS_CAN_TEST
    //Do this
#else
    #include "Platform-linux-socket-can.h"
#endif

int main(int argc, char *argv[]) {

    ctre::phoenix::platform::can::SetCANInterface(argv[1]);
    
    uint64_t iterNum = argv[2][0] - '0';
    
    while(1) {
        ctre::phoenix::platform::can::canframe_t readFrame;

        ctre::phoenix::platform::can::CANbus_ReceiveFrame(readFrame);

        if(readFrame.arbID == 0x0101) {
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
    }

    return 0;
}



