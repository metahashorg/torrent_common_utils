#ifndef VARIANT_UTILS_H_
#define VARIANT_UTILS_H_

#include <variant>

namespace common {

template<class T> struct always_false : std::false_type {};

template<class... Ts> struct overloaded : Ts... { using Ts::operator()...; };
template<class... Ts> overloaded(Ts...) -> overloaded<Ts...>;

template<typename Visitor, typename Variant>
constexpr decltype(auto)
visit_impl(Variant&& variant, Visitor&& visitor) {
    return std::visit(std::forward<Visitor>(visitor), std::forward<Variant>(variant));
}

#define BEGIN_VARIANT(variant, element) \
common::visit_impl(variant, [&](auto &element) { \
using ElementType = std::decay_t<decltype(element)>; \
if constexpr (false) { \

#define END_VARIANT() \
} else { \
    static_assert(common::always_false<ElementType>::value, "non-exhaustive visitor!"); \
} \
});

#define CASE_VARIANT(type) \
} else if constexpr (std::is_same_v<type, ElementType>) {

}

#endif // VARIANT_UTILS_H_
