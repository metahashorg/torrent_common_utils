#ifndef LOG_H_
#define LOG_H_

#include "logger/easylogging++.h"

#include <string>

#include "OopUtils.h"

namespace common {

class LogBuilder: public no_copyable, public no_moveable {
    friend void configureLogInternal(const LogBuilder &builder);
public:
    
    LogBuilder(const std::string &folder)
        : folder(folder)
    {}
    
    ~LogBuilder();
    
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
    
    const std::string folder;
    
    bool isAppend = true;
    
    bool isConsole = true;
    
    bool isAutoSpacing = true;
    
    bool isTime = true;
    
    bool isDisabledDebug = false;
    
    bool isDisabledInfo = false;
};

LogBuilder configureLog(const std::string &folder);

void configureLog(const std::string &folder, bool isAppend, bool isConsole, bool isAutoSpacing, bool isTime);

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
