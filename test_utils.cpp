#include "ctre/phoenix/Platform/Platform.h"
#include <string>
#include <iostream>
#include <chrono>
#include <thread>

#include <atomic>

static std::atomic<bool> term; 


void StreamThread() {
    while(true) {
        if(term) {
            return;
        }

    }
}

int main(int argc, char *argv[]) {

    uint64_t iterNum = argv[2][0] - '0';

    uint64_t iterNum = argv[3][0] - '0';

    if(isCheckEnable) {
        std::thread enableCheckTest = new std::thread(EnableCheckThread);
    }

    while(1) {
        if(isEnable)
        {
            ctre::phoenix::platform::FeedWatchDog(100);
        }
        
        std::this_thread::sleep_for(std::chrono::milliseconds(40));
    }

    term.store(true);

    std::this_thread::sleep_for(std::chrono::milliseconds(100));

    return 0;
}
