#pragma once

// std
#include <string>

// internal
#include <miru/params/parameter.hpp>
#include <miru/query/query.hpp>

// external
#include <yaml-cpp/yaml.h>

#include <nlohmann/json.hpp>

namespace miru::query {

using Parameter = miru::params::Parameter;

// Config class
class ROS2StyleParameterQuery {
 public:
  ROS2StyleParameterQuery(const miru::params::Parameter& root) : root_(root) {}

    // ============================== ROS2 INTERFACES ============================== //

    // The following ROS2 interfaces are pulled from the rclcpp NodeParametersInterface.

    // https://github.com/ros2/rclcpp/blob/a0a2a067d84fd6a38ab4f71b691d51ca5aa97ba5/rclcpp/include/rclcpp/node.hpp#L598

    /// Return true if a given parameter is declared.
    /**
    * \param[in] name The name of the parameter to check for being declared.
    * \return true if the parameter name has been declared, otherwise false.
    */
    bool has_parameter(const std::string& parameter_name);

    // https://github.com/ros2/rclcpp/blob/a0a2a067d84fd6a38ab4f71b691d51ca5aa97ba5/rclcpp/include/rclcpp/node.hpp#L768

  /// Return the parameter by the given name.
  /**
   * If the parameter has not been declared, then this method may throw the
   * rclcpp::exceptions::ParameterNotDeclaredException exception.
   * If the parameter has not been initialized, then this method may throw the
   * rclcpp::exceptions::ParameterUninitializedException exception.
   *
   * If undeclared parameters are allowed, see the node option
   * rclcpp::NodeOptions::allow_undeclared_parameters, then this method will
   * not throw the rclcpp::exceptions::ParameterNotDeclaredException exception,
   * and instead return a default initialized rclcpp::Parameter, which has a type of
   * rclcpp::ParameterType::PARAMETER_NOT_SET.
   *
   * \param[in] name The name of the parameter to get.
   * \return The requested parameter inside of a rclcpp parameter object.
   * \throws rclcpp::exceptions::ParameterNotDeclaredException if the parameter
   *   has not been declared and undeclared parameters are not allowed.
   * \throws rclcpp::exceptions::ParameterUninitializedException if the parameter
   *   has not been initialized.
   */
  Parameter get_parameter(const std::string & name) const;

  // https://github.com/ros2/rclcpp/blob/a0a2a067d84fd6a38ab4f71b691d51ca5aa97ba5/rclcpp/include/rclcpp/node.hpp#L787

  /// Get the value of a parameter by the given name, and return true if it was set.
  /**
   * This method will never throw the
   * rclcpp::exceptions::ParameterNotDeclaredException exception, but will
   * instead return false if the parameter has not be previously declared.
   *
   * If the parameter was not declared, then the output argument for this
   * method which is called "parameter" will not be assigned a value.
   * If the parameter was declared, and therefore has a value, then it is
   * assigned into the "parameter" argument of this method.
   *
   * \param[in] name The name of the parameter to get.
   * \param[out] parameter The output storage for the parameter being retrieved.
   * \return true if the parameter was previously declared, otherwise false.
   */
  bool get_parameter(const std::string & name, Parameter & parameter) const;

  // https://github.com/ros2/rclcpp/blob/a0a2a067d84fd6a38ab4f71b691d51ca5aa97ba5/rclcpp/include/rclcpp/node.hpp#L804

  /// Get the value of a parameter by the given name, and return true if it was set.
  /**
   * Identical to the non-templated version of this method, except that when
   * assigning the output argument called "parameter", this method will attempt
   * to coerce the parameter value into the type requested by the given
   * template argument, which may fail and throw an exception.
   *
   * If the parameter has not been declared, it will not attempt to coerce the
   * value into the requested type, as it is known that the type is not set.
   *
   * \throws rclcpp::ParameterTypeException if the requested type does not
   *   match the value of the parameter which is stored.
   */
  template<typename ParameterT>
  bool
  get_parameter(const std::string & name, ParameterT & parameter) const {
    SearchParamFilters filters = SearchParamFiltersBuilder().with_param_name(name).build();
    return miru::query::get_parameter(root_, filters, parameter);
  }

  // https://github.com/ros2/rclcpp/blob/a0a2a067d84fd6a38ab4f71b691d51ca5aa97ba5/rclcpp/include/rclcpp/node.hpp#L823

  /// Get the parameter value, or the "alternative_value" if not set, and assign it to "parameter".
  /**
   * If the parameter was not set, then the "parameter" argument is assigned
   * the "alternative_value".
   *
   * Like the version of get_parameter() which returns a bool, this method will
   * not throw the rclcpp::exceptions::ParameterNotDeclaredException exception.
   *
   * In all cases, the parameter is never set or declared within the node.
   *
   * \param[in] name The name of the parameter to get.
   * \param[out] parameter The output where the value of the parameter should be assigned.
   * \param[in] alternative_value Value to be stored in output if the parameter was not set.
   * \returns true if the parameter was set, false otherwise.
   */
  template<typename ParameterT>
  bool
  get_parameter_or(
    const std::string & name,
    ParameterT & parameter,
    const ParameterT & alternative_value) const {
    SearchParamFilters filters = SearchParamFiltersBuilder().with_param_name(name).build();
    return miru::query::get_parameter_or(root_, filters, parameter, alternative_value);
  }

  // https://github.com/ros2/rclcpp/blob/a0a2a067d84fd6a38ab4f71b691d51ca5aa97ba5/rclcpp/include/rclcpp/node.hpp#L842

  /// Return the parameter value, or the "alternative_value" if not set.
  /**
   * If the parameter was not set, then the "alternative_value" argument is returned.
   *
   * This method will not throw the rclcpp::exceptions::ParameterNotDeclaredException exception.
   *
   * In all cases, the parameter is never set or declared within the node.
   *
   * \param[in] name The name of the parameter to get.
   * \param[in] alternative_value Value to be stored in output if the parameter was not set.
   * \returns The value of the parameter.
   */
  template<typename ParameterT>
  ParameterT
  get_parameter_or(
    const std::string & name,
    const ParameterT & alternative_value) const {
    SearchParamFilters filters = SearchParamFiltersBuilder().with_param_name(name).build();
    return miru::query::get_parameter_or(root_, filters, alternative_value);
  }

// https://github.com/ros2/rclcpp/blob/a0a2a067d84fd6a38ab4f71b691d51ca5aa97ba5/rclcpp/include/rclcpp/node.hpp#L868

  /// Return the parameters by the given parameter names.
  /**
   * Like get_parameter(const std::string &), this method may throw the
   * rclcpp::exceptions::ParameterNotDeclaredException exception if the
   * requested parameter has not been declared and undeclared parameters are
   * not allowed, and may throw the rclcpp::exceptions::ParameterUninitializedException exception.
   *
   * Also like get_parameter(const std::string &), if undeclared parameters are allowed and the
   * parameter has not been declared, then the corresponding rclcpp::Parameter
   * will be default initialized and therefore have the type
   * rclcpp::ParameterType::PARAMETER_NOT_SET.
   *
   * \param[in] names The names of the parameters to be retrieved.
   * \return The parameters that were retrieved.
   * \throws rclcpp::exceptions::ParameterNotDeclaredException if any of the
   *   parameters have not been declared and undeclared parameters are not
   *   allowed.
   * \throws rclcpp::exceptions::ParameterUninitializedException if any of the
   *   parameters have not been initialized.
   */
  std::vector<Parameter>
  get_parameters(const std::vector<std::string> & names) const;

// https://github.com/ros2/rclcpp/blob/a0a2a067d84fd6a38ab4f71b691d51ca5aa97ba5/rclcpp/include/rclcpp/node.hpp#L909

  /// Get the parameter values for all parameters that have a given prefix.
  /**
   * The "prefix" argument is used to list the parameters which are prefixed
   * with that prefix, see also list_parameters().
   *
   * The resulting list of parameter names are used to get the values of the
   * parameters.
   *
   * The names which are used as keys in the values map have the prefix removed.
   * For example, if you use the prefix "foo" and the parameters "foo.ping" and
   * "foo.pong" exist, then the returned map will have the keys "ping" and
   * "pong".
   *
   * An empty string for the prefix will match all parameters.
   *
   * If no parameters with the prefix are found, then the output parameter
   * "values" will be unchanged and false will be returned.
   * Otherwise, the parameter names and values will be stored in the map and
   * true will be returned to indicate "values" was mutated.
   *
   * This method will never throw the
   * rclcpp::exceptions::ParameterNotDeclaredException exception because the
   * action of listing the parameters is done atomically with getting the
   * values, and therefore they are only listed if already declared and cannot
   * be undeclared before being retrieved.
   *
   * Like the templated get_parameter() variant, this method will attempt to
   * coerce the parameter values into the type requested by the given
   * template argument, which may fail and throw an exception.
   *
   * \param[in] prefix The prefix of the parameters to get.
   * \param[out] values The map used to store the parameter names and values,
   *   respectively, with one entry per parameter matching prefix.
   * \returns true if output "values" was changed, false otherwise.
   * \throws rclcpp::ParameterTypeException if the requested type does not
   *   match the value of the parameter which is stored.
   */
  template<typename ParameterT>
  bool
  get_parameters(
    const std::string & prefix,
    std::map<std::string, ParameterT> & values
  ) const {
    SearchParamFilters filters = SearchParamFiltersBuilder().with_prefix(prefix).build();
    std::vector<Parameter> parameters = miru::query::get_parameters(root_, filters);
    for (const auto& parameter : parameters) {
      std::string name_wo_prefix = parameter.get_name().substr(prefix.size());
      values[name_wo_prefix] = parameter.get_value<ParameterT>();
    }
    return true;
  }

 private:
  miru::params::Parameter root_;
};

}  // namespace miru::params
