#include <miru/params/parameter.hpp>
#include <miru/params/details/value.hpp>

namespace miru::params {

bool is_leaf(const ParameterValue& parameter);
ParametersView get_children_view(const Parameter& parameter);

}  // namespace miru::params
