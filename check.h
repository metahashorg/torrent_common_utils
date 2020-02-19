#ifndef CHECK_H_
#define CHECK_H_

#include <string>
#include <stdexcept>

namespace common {

using exception = std::string;

struct UserException: public std::runtime_error {
    UserException(const std::string &exception)
        : std::runtime_error(exception)
    {}
};

}

#define throwErr1(except, s) { \
    { \
        throw except((s) + std::string(". Error at file ") \
        + std::string(__FILE__) + std::string(" line ") + std::to_string(__LINE__)); \
    } \
}

#define throwErr(s) { \
    { \
        throwErr1(common::exception, s); \
    } \
}

#define throwUserErr(s) { \
    { \
        throwErr1(common::UserException, s); \
    } \
}

#define CHECK1(v, s, except) { \
    if (!(v)) { \
        throwErr1(except, s); \
    } \
}

#define CHECK(v, s) { \
    CHECK1(v, s, common::exception); \
}

#define CHECK_USER(v, s) { \
    CHECK1(v, s, common::UserException); \
}

#endif // CHECK_H_
