// internal
#include <miru/params/parameter.hpp>
#include <miru/params/exceptions.hpp>

namespace miru::params {

std::string to_string(const Scalar & scalar) {
    return scalar.as_string();
}

std::ostream & operator<<(std::ostream & os, const Scalar & scalar) {
    os << to_string(scalar);
    return os;
}

// Interprets the scalar as a boolean using YAML boolean rules
// 
// https://yaml.org/type/bool.html
bool Scalar::as_bool() const {
    std::string lower = value_;
    std::transform(lower.begin(), lower.end(), lower.begin(), ::tolower);
    if (lower == "y" || lower == "yes" || lower == "true" || lower == "on") {
        return true;
    }
    if (lower == "n" || lower == "no" || lower == "false" || lower == "off") {
        return false;
    }
    throw InvalidScalarConversion(
        value_,
        to_string(ParameterType::PARAMETER_BOOL),
        "cannot interpret value as a boolean"
    );
}

int64_t Scalar::as_int() const {
    // use long long to ensure we maintain precision (long is not enough on all systems)
    if (value_.find(".") != std::string::npos) {
        throw InvalidScalarConversion(
            value_,
            to_string(ParameterType::PARAMETER_INTEGER),
            "cannot interpret value as an integer: contains a decimal point"
        );
    }
    try {
        size_t pos = 0;
        int64_t result = std::stoll(value_, &pos);
        if (pos != value_.size()) {
            throw InvalidScalarConversion(
                value_,
                to_string(ParameterType::PARAMETER_INTEGER),
                "cannot interpret value as an integer: contains invalid characters"
            );
        }
        return result;
    } catch (const std::exception& e) {
        throw InvalidScalarConversion(
            value_,
            to_string(ParameterType::PARAMETER_INTEGER),
            "cannot interpret value as an integer: " + std::string(e.what())
        );
    }
}

double Scalar::as_double() const {
    try {
        size_t pos = 0;
        double result = std::stod(value_, &pos);
        if (pos != value_.size()) {
            throw InvalidScalarConversion(
                value_,
                to_string(ParameterType::PARAMETER_DOUBLE),
                "cannot interpret value as a double: contains invalid characters"
            );
        }
        return result;
    } catch (const std::exception& e) {
        throw InvalidScalarConversion(
            value_,
            to_string(ParameterType::PARAMETER_DOUBLE),
            "cannot interpret value as a double: " + std::string(e.what())
        );
    }
}

} // namespace miru::params

