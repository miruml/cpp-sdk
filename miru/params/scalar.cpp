// internal
#include <miru/params/errors.hpp>
#include <miru/params/parameter.hpp>
#include <miru/details/utils.hpp>

namespace miru::params {

std::string to_string(const Scalar& scalar) { return scalar.as_string(); }

std::ostream& operator<<(std::ostream& os, const Scalar& scalar) {
  os << to_string(scalar);
  return os;
}

// Interprets the scalar as a boolean using YAML boolean rules
//
// https://yaml.org/type/bool.html
bool Scalar::as_bool() const {
  try {
    return miru::utils::details::yaml_string_to_bool(value_);
  } catch (const std::exception& e) {
    THROW_INVALID_SCALAR_CONVERSION(
      value_, to_string(ParameterType::PARAMETER_BOOL), e.what()
    );
  }
}

int64_t Scalar::as_int() const {
  try {
    return miru::utils::details::string_to_int64(value_);
  } catch (const std::exception& e) {
    THROW_INVALID_SCALAR_CONVERSION(
      value_, to_string(ParameterType::PARAMETER_INTEGER), e.what()
    );
  }
}

double Scalar::as_double() const {
  try {
    return miru::utils::details::string_to_double(value_);
  } catch (const std::exception& e) {
    THROW_INVALID_SCALAR_CONVERSION(
      value_, to_string(ParameterType::PARAMETER_DOUBLE), e.what()
    );
  }
}

}  // namespace miru::params
