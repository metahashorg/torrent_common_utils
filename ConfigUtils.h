#ifndef SERVER_CONFIGUTILS_H
#define SERVER_CONFIGUTILS_H

#include <string>
#include <optional>

#include "check.h"

#include <libconfig.h++>

namespace common {

struct ConfigException: public std::runtime_error {
    ConfigException(const std::string &exception)
        : std::runtime_error(exception)
    {}
};

struct ConfigArray {};

struct ConfigObject {};

using RefCfg = std::reference_wrapper<const libconfig::Setting>;

template<typename T>
std::optional<std::conditional_t<std::is_same_v<T, ConfigArray>, RefCfg, std::conditional_t<std::is_same_v<T, ConfigObject>, RefCfg, T>>> getOpt(const libconfig::Setting &obj) {
    if constexpr (std::is_same_v<T, ConfigArray>) {
        if (obj.isArray() || obj.isList()) {
            return obj;
        }
    } else if constexpr (std::is_same_v<T, ConfigObject>) {
        if (obj.isGroup()) {
            return obj;
        }
    } else {
        try {
            return static_cast<T>(obj);
        } catch (const libconfig::SettingTypeException &) {
            return std::nullopt;
        }
    }
    return std::nullopt;
}

template<typename T>
auto getOpt(const libconfig::Setting &obj, std::string_view name) -> decltype(getOpt<T>(obj[name.data()])) {
    if (!obj.exists(name.data())) {
        return std::nullopt;
    }
    return getOpt<T>(obj[name.data()]);
}

template<typename T, typename Exception=ConfigException>
auto getOpt(const libconfig::Config &obj, std::string_view name) -> decltype(getOpt<T>(obj, name)) {
    return getOpt<T>(obj.getRoot(), name);
}

template<typename T>
T getOpt(const libconfig::Setting &obj, std::string_view name, T defaultValue) {
    const auto result = getOpt<T>(obj, name);
    if (result.has_value()) {
        return result.value();
    } else {
        return defaultValue;
    }
}

template<typename T, typename Exception=ConfigException>
decltype(auto) get(const libconfig::Setting &obj) {
    const auto result = getOpt<T>(obj);
    CHECK1(result.has_value(), std::string("Incorrect config: ") + obj.getPath() + " not found", Exception);
    const auto value = result.value();
    if constexpr (std::is_same_v<std::decay_t<decltype(value)>, RefCfg>) {
        return value.get();
    } else {
        return value;
    }
}

template<typename T, typename Exception=ConfigException>
decltype(auto) get(const libconfig::Setting &obj, std::string_view name) {
    CHECK1(obj.exists(name.data()), std::string("Incorrect config: ") + obj.getPath() + "/" + std::string(name) + " not found", Exception);
    return get<T>(obj[name.data()]);
}

template<typename T, typename Exception=ConfigException>
decltype(auto) get(const libconfig::Config &obj, std::string_view name) {
    return get<T, Exception>(obj.getRoot(), name);
}

} // namespace common

#endif //SERVER_CONFIGUTILS_H
