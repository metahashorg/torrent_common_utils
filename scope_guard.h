#ifndef SCOPE_GUARD_H_
#define SCOPE_GUARD_H_

#include "OopUtils.h"

#include <functional>

namespace common {

class scope_guard: public no_copyable, public no_moveable {
public:
    
    explicit scope_guard(const std::function<void()> &f)
        : func(f)
    {}
    
    ~scope_guard() {
        func();
    }
    
private:
    
    std::function<void()> func;
};
    
}

#endif // SCOPE_GUARD_H_
