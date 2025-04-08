#pragma once

// std
#include <algorithm>
#include <cstdint>
#include <string>
#include <type_traits>
#include <vector>

// internal
#include <miru/params/exceptions.hpp>
#include <miru/params/type.hpp>

namespace miru::params {

template<typename T>
struct is_scalar_type : std::false_type {};

template<> struct is_scalar_type<bool> : std::true_type {};
template<> struct is_scalar_type<int64_t> : std::true_type {};
template<> struct is_scalar_type<double> : std::true_type {};
template<> struct is_scalar_type<std::string> : std::true_type {};

class Scalar {
public:
    Scalar(const std::string & value) : value_(value) {}

    bool operator==(const Scalar & other) const { return value_ == other.value_; }
    bool operator!=(const Scalar & other) const { return value_ != other.value_; }   

    bool as_bool() const;
    int64_t as_int() const;
    double as_double() const;
    const std::string & as_string() const { return value_; }

    template<ParameterType type>
    typename std::enable_if<type == ParameterType::PARAMETER_BOOL, bool>::type
    as() const {
        return as_bool();
    }

    template<ParameterType type>
    typename std::enable_if<type == ParameterType::PARAMETER_INTEGER, int64_t>::type
    as() const {
        return as_int();
    }

    template<ParameterType type>
    typename std::enable_if<type == ParameterType::PARAMETER_DOUBLE, double>::type
    as() const {
        return as_double();
    }

    template<ParameterType type>
    typename std::enable_if<type == ParameterType::PARAMETER_STRING, std::string>::type
    as() const {
        return as_string();
    }

    template<typename type> 
    constexpr
    typename std::enable_if<std::is_same<type, bool>::value, bool>::type
    as() const {
        return as_bool();
    }

    template<typename type> 
    constexpr
    typename std::enable_if<std::is_integral<type>::value && !std::is_same<type, bool>::value, int64_t>::type
    as() const {
        return cast_int64_to<type>(as_int());
    }

    template<typename type> 
    constexpr
    typename std::enable_if<std::is_floating_point<type>::value, double>::type
    as() const {
        return cast_double_to<type>(as_double());
    }

    template<typename type> 
    constexpr
    typename std::enable_if<std::is_convertible<type, std::string>::value, std::string>::type
    as() const {
        return as_string();
    }

private:
    std::string value_;
};

std::string to_string(const Scalar & scalar);
std::ostream & operator<<(std::ostream & os, const Scalar & scalar);

template<typename T>
struct is_convertible_to_scalar_type : 
    std::bool_constant<
        is_scalar_type<T>::value || 
        std::is_same_v<T, bool> ||
        std::is_convertible_v<T, int64_t> ||
        std::is_convertible_v<T, double> ||
        std::is_convertible_v<T, std::string>
    >
{};

template<typename T>
typename std::enable_if<is_convertible_to_scalar_type<T>::value, std::vector<T>>::type
transform_scalar_array(const std::vector<Scalar> & scalars) {
    std::vector<T> dest;
    dest.reserve(scalars.size());
    std::transform(scalars.begin(), scalars.end(), std::back_inserter(dest), [](const Scalar & s) { return s.as<T>(); });
    return dest;
}


} // namespace miru::params

