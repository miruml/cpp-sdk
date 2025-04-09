// std
#include <algorithm>
#include <vector>

// miru
#include "miru/params/parameter.hpp"
#include "miru/utils.hpp"

namespace miru::params {

Map::Map(const std::vector<Parameter>& fields) : sorted_fields_(fields) {
  if (fields.empty()) {
    throw std::invalid_argument("Map must be initialized with at least one field");
  }

  // ensure the field names are unique
  std::vector<std::string> field_names;
  for (const auto& field : fields) {
    field_names.push_back(field.get_name());
  }
  try {
    utils::assert_unique_strings(field_names);
  } catch (const std::exception& e) {
    throw std::invalid_argument("Unable to instantiate Map with duplicate field names: " + std::string(e.what()));
  }

  // store the fields by name for comparison / access purposes in the future
  std::sort(sorted_fields_.begin(), sorted_fields_.end(),
            [](const Parameter& a, const Parameter& b) {
              return a.get_name() < b.get_name();
            });
}

const std::vector<Parameter>& Map::get_fields() const { return sorted_fields_; }

bool Map::operator==(const Map& other) const {
  return sorted_fields_ == other.sorted_fields_;
}

bool Map::operator!=(const Map& other) const { return !(*this == other); }

MapArray::MapArray(const std::vector<Parameter>& items) : items_(items) {
  if (items.empty()) {
    throw std::invalid_argument("MapArray must be initialized with at least one item");
  }

  // ensure the items are maps
  for (const Parameter& item : items_) {
    if (!item.is_map()) {
      throw std::invalid_argument(
          "Cannot instantiate MapArray with non-Map parameters");
    }
  }

  // ensure the items have unique names
  std::vector<std::string> item_names;
  for (const auto& item : items_) {
    item_names.push_back(item.get_name());
  }
  utils::assert_unique_strings(item_names);

  // store the items by name for comparison / access purposes in the future
  std::sort(items_.begin(), items_.end(), [](const Parameter& a, const Parameter& b) {
    return a.get_name() < b.get_name();
  });

  // ensure the items keys are integers in ascending order
  for (size_t i = 0; i < items_.size(); ++i) {
    if (items_[i].get_key() != std::to_string(i)) {
      throw std::invalid_argument(
          "MapArray items must have keys that are integers in ascending order. "
          "Unable to find index '" +
          std::to_string(i) +
          "' in the names of the provided items: " + items_[i].get_name());
    }
  }
}

const std::vector<Parameter>& MapArray::get_items() const { return items_; }

bool MapArray::operator==(const MapArray& other) const {
  return items_ == other.items_;
}

bool MapArray::operator!=(const MapArray& other) const {
  return !(*this == other);
}

NestedArray::NestedArray(const std::vector<Parameter>& items) : items_(items) {
  if (items.empty()) {
    throw std::invalid_argument("NestedArray must be initialized with at least one item");
  }
  for (const Parameter& item : items) {
    if (!item.is_array()) {
      throw std::invalid_argument(
          "Cannot instantiate NestedArray with non-array parameters");
    }
  }

  // ensure the items have unique names
  std::vector<std::string> item_names;
  for (const auto& item : items_) {
    item_names.push_back(item.get_name());
  }
  utils::assert_unique_strings(item_names);

  // store the items by name for comparison / access purposes in the future
  std::sort(items_.begin(), items_.end(), [](const Parameter& a, const Parameter& b) {
    return a.get_name() < b.get_name();
  });

  // ensure the items keys are integers in ascending order
  for (size_t i = 0; i < items_.size(); ++i) {
    if (items_[i].get_key() != std::to_string(i)) {
      throw std::invalid_argument(
          "NestedArray items must have keys that are integers in ascending order. "
          "Unable to find index '" +
          std::to_string(i) +
          "' in the names of the provided items: " + items_[i].get_name());
    }
  }
}

const std::vector<Parameter>& NestedArray::get_items() const { return items_; }

bool NestedArray::operator==(const NestedArray& other) const {
  return items_ == other.items_;
}

bool NestedArray::operator!=(const NestedArray& other) const {
  return !(*this == other);
}

}  // namespace miru::params