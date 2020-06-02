#ifndef LOG_H_
#define LOG_H_

#include "logger/easylogging++.h"

#include <string>

namespace common {

enum class LogInitResult {
    Ok, AlreadyInitialized
};

class LogBuilder {
    friend LogInitResult configureLog(const LogBuilder &builder);
public:
    
    LogBuilder& setFolder(const std::string &value) {
        folder = value;
        return *this;
    }

    LogBuilder& append(bool value) {
        isAppend = value;
        return *this;
    }

    LogBuilder& console(bool value) {
        isConsole = value;
        return *this;
    }
    
    LogBuilder& autoSpacing(bool value) {
        isAutoSpacing = value;
        return *this;
    }
    
    LogBuilder& printTime(bool value) {
        isTime = value;
        return *this;
    }
    
    LogBuilder& disableDebug(bool value) {
        isDisabledDebug = value;
        return *this;
    }
    
    LogBuilder& disableInfo(bool value) {
        isDisabledInfo = value;
        return *this;
    }
    
protected:
    
    std::string folder;
    
    bool isAppend = true;
    
    bool isConsole = true;
    
    bool isAutoSpacing = true;
    
    bool isTime = true;
    
    bool isDisabledDebug = false;
    
    bool isDisabledInfo = false;
};

LogBuilder makeLog(const std::string &folder);

[[nodiscard]]
LogInitResult configureLog(const LogBuilder &builder);

[[nodiscard]]
LogInitResult configureLog(const std::string &folder, bool isAppend, bool isConsole, bool isAutoSpacing, bool isTime);

void disableDebug();

void disableInfo();

void flushLogsAll();

struct EmptyLog {
    template<typename T>
    EmptyLog& operator <<(const T &) {
        return *this;
    }
};

}

#define LOGERR LOG(ERROR)
#define LOGWARN LOG(WARNING)
#define LOGINFO LOG(INFO)
#define LOGDEBUG LOG(DEBUG)

#endif // LOG_H_
