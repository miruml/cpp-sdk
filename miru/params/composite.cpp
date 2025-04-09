// std
#include <algorithm>
#include <vector>

// miru
#include <miru/params/parameter.hpp>
#include <miru/params/type.hpp>
#include <miru/utils.hpp>

namespace miru::params {

// ============================= HELPER FUNCTIONS ================================== //
void assert_valid_array_element_types(
    const std::string& object_to_initialize, const std::vector<Parameter>& items,
    std::function<bool(const Parameter&)> is_valid_type,
    const std::string& valid_type_name) {
  for (const auto& item : items) {
    if (!is_valid_type(item)) {
      throw InvalidParameterType(
          object_to_initialize,
          "Cannot instantiate " + object_to_initialize + " with non-" +
              valid_type_name + " parameters. Item '" + item.get_name() +
              "' is of type '" + to_string(item.get_type()) + "'");
    }
  }
}
void assert_unique_field_names(const std::string& object_to_initialize,
                               const std::vector<Parameter>& fields) {
  // ensure the field names are unique
  std::vector<std::string> field_names;
  for (const auto& field : fields) {
    field_names.push_back(field.get_name());
  }
  try {
    utils::assert_unique_strings(field_names);
  } catch (const std::exception& e) {
    throw DuplicateFieldNames(object_to_initialize, std::string(e.what()));
  }
}

void assert_identical_parent_names(const std::vector<Parameter>& fields) {
  for (const auto& field : fields) {
    if (field.get_parent_name() != fields[0].get_parent_name()) {
      throw MismatchingParentNames("Map", fields[0].get_name(),
                                   fields[0].get_parent_name(), field.get_name(),
                                   field.get_parent_name());
    }
  }
}

void assert_ascending_integer_keys(const std::vector<Parameter>& items) {
  for (size_t i = 0; i < items.size(); ++i) {
    if (items[i].get_key() != std::to_string(i)) {
      throw InvalidArrayKeys("MapArray", i, items[i].get_name());
    }
  }
}

// =================================== MAP ======================================== //
Map::Map(const std::vector<Parameter>& fields) {
  if (fields.empty()) {
    throw EmptyInitialization("Map");
  }

  // name uniqueness and parent name consistency
  assert_unique_field_names("Map", fields);
  assert_identical_parent_names(fields);

  // set the fields
  for (const auto& field : fields) {
    sorted_fields_.push_back(std::make_pair(field.get_key(), field));
  }

  // store the fields by name for comparison / access purposes in the future
  std::sort(
    sorted_fields_.begin(), sorted_fields_.end(),
    [](const std::pair<std::string, Parameter>& a, const std::pair<std::string, Parameter>& b) {
      return a.first < b.first;
    });
}

bool Map::operator==(const Map& other) const {
  return sorted_fields_ == other.sorted_fields_;
}

bool Map::operator!=(const Map& other) const { return !(*this == other); }

const Parameter& Map::operator[](const std::string& key) const {
  // use binary search to find the field since the fields are sorted
  auto it = std::lower_bound(
      sorted_fields_.begin(),
      sorted_fields_.end(),
      key,
      [](const std::pair<std::string, Parameter>& p, const std::string& key) {
        return p.first < key;
      });

  if (it == sorted_fields_.end() || it->first != key) {
    throw std::invalid_argument("Unable to find map field with key: " + key);
  }
  return it->second;
}

// ================================= MAP ARRAY ===================================== //
MapArray::MapArray(const std::vector<Parameter>& items) : items_(items) {
  if (items.empty()) {
    throw EmptyInitialization("MapArray");
  }

  // ensure the items are maps
  assert_valid_array_element_types(
      "MapArray", items, [](const Parameter& item) { return item.is_map(); }, "Map");

  // name uniqueness and parent name consistency
  assert_unique_field_names("MapArray", items);
  assert_identical_parent_names(items);

  // store the items by name for comparison / access purposes in the future
  std::sort(items_.begin(), items_.end(), [](const Parameter& a, const Parameter& b) {
    return a.get_name() < b.get_name();
  });

  // ensure the items keys are integers in ascending order
  assert_ascending_integer_keys(items);
}

bool MapArray::operator==(const MapArray& other) const {
  return items_ == other.items_;
}

bool MapArray::operator!=(const MapArray& other) const { return !(*this == other); }

const Parameter& MapArray::operator[](const size_t index) const {
  return items_[index];
}

// ================================= NESTED ARRAY ================================== //
NestedArray::NestedArray(const std::vector<Parameter>& items) : items_(items) {
  if (items.empty()) {
    throw EmptyInitialization("NestedArray");
  }

  // ensure the items are arrays
  assert_valid_array_element_types(
      "NestedArray", items, [](const Parameter& item) { return item.is_array(); },
      "NestedArray");

  // name uniqueness and parent name consistency
  assert_unique_field_names("MapArray", items);
  assert_identical_parent_names(items);

  // store the items by name for comparison / access purposes in the future
  std::sort(items_.begin(), items_.end(), [](const Parameter& a, const Parameter& b) {
    return a.get_name() < b.get_name();
  });

  // ensure the items keys are integers in ascending order
  assert_ascending_integer_keys(items);
}

bool NestedArray::operator==(const NestedArray& other) const {
  return items_ == other.items_;
}

bool NestedArray::operator!=(const NestedArray& other) const {
  return !(*this == other);
}

const Parameter& NestedArray::operator[](const size_t index) const {
  return items_[index];
}

}  // namespace miru::params