#include "stopProgram.h"

#include <signal.h>
#include <iostream>

#include "log.h"

namespace common {

std::atomic<bool> isStopSignalCalled(false);

void initializeStopProgram() {
    signal(SIGTERM, stopHandler);
    signal(SIGABRT, stopHandler);
    signal(SIGINT, stopHandler);
}

void stopHandler(int sig) {
    signal(SIGTERM, SIG_IGN);
    signal(SIGABRT, SIG_IGN);
    signal(SIGINT, SIG_IGN);
    
    std::cout << "Stop signal received " + std::to_string(sig) << std::endl;
    isStopSignalCalled = true;
    
    flushLogsAll();
}

void checkStopSignal(const std::atomic<bool> &checkedVariable) {
    if (checkedVariable.load()) {
        throw StopException();
    }
}

void checkStopSignal() {
    checkStopSignal(isStopSignalCalled);
}

void sleep(const std::atomic<bool> &checkedVariable, const seconds &dur) {
    const time_point beginTime = common::now();
    while (!checkedVariable.load()) {
        checkStopSignal();
        const time_point now = common::now();
        if (now - beginTime >= dur) {
            break;
        }
        std::this_thread::sleep_for(1s);
    }
}

void sleepUntil(const std::atomic<bool> &checkedVariable, const time_point_system &tp) {
    const time_point beginTime = common::now();
    while (!checkedVariable.load()) {
        checkStopSignal();
        const time_point_system now = common::nowSystem();
        if (now >= tp) {
            break;
        }
        std::this_thread::sleep_until(std::min(now + 1s, tp));
    }
}

void sleep(const seconds &dur) {
    sleep(std::atomic<bool>(false), dur);
}

void sleepUntil(const time_point_system &tp) {
    sleepUntil(std::atomic<bool>(false), tp);
}

void sleepMs(const milliseconds &dur) {
    std::this_thread::sleep_for(dur);
    checkStopSignal();
}

void whileTrue() {
    try {
        while (true) {
            sleep(1s);
            checkStopSignal();
        }
    } catch (const StopException &e) {
        return;
    }
}

void waitStop() {
    whileTrue();
}

void stopProgram() {
    isStopSignalCalled = true;
}

}
