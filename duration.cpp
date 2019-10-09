#include "duration.h"

namespace common {

std::string currentDateToStr(const std::string &format) {
    time_t rawtime;
    struct tm * timeinfo;
    char buffer[80];
    
    time (&rawtime);
    timeinfo = localtime(&rawtime);
    
    strftime(buffer,sizeof(buffer), format.c_str(),timeinfo);
    std::string str(buffer);
    return str;
}

}
