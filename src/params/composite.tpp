// // std
// #include <vector>
// #include <type_traits>

// // internal
// #include <miru/params/composite.hpp>
// #include <miru/params/parameter.hpp>

// namespace miru::params {

// template<typename T>
// struct always_false : std::false_type {};

// template<typename VectorT>
// VectorT NestedArray::as() const {
//     VectorT result;
//     result.reserve(items_.size());
//     for (const auto& item : items_) {
//         if constexpr (
//             std::is_same_v<VectorT, std::vector<std::vector<bool>>>
//         ) {
//             result.push_back(item.as_bool_array());
//         } else if constexpr (
//             std::is_same_v<VectorT, std::vector<std::vector<int64_t>>>
//         ) {
//             result.push_back(item.as_integer_array());
//         } else if constexpr (
//             std::is_same_v<VectorT, std::vector<std::vector<double>>>
//         ) {
//             result.push_back(item.as_double_array());
//         } else if constexpr (
//             std::is_same_v<VectorT, std::vector<std::vector<std::string>>>
//         ) {
//             result.push_back(item.as_string_array());
//         } else if constexpr (
//             std::is_same_v<VectorT, std::vector<std::vector<Parameter>>>
//         ) {
//             result.push_back(item.as_nested_array());
//         } else {
//             static_assert(always_false<VectorT>::value, "Unsupported type for NestedArray::as()");
//         }
//     }
//     return result;
// }

// }