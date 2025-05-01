#include <miru/params/iterator.hpp>
#include <miru/params/parameter.hpp>
#include <params/value.hpp>

namespace miru::params {

bool is_leaf(const Parameter& parameter);
bool is_leaf(const ParameterValue& parameter);

bool has_children(const Parameter& parameter);
ParametersView get_children_view(const Parameter& parameter);

}  // namespace miru::params
