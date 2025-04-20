#pragma once

// std
#include <cstddef>
#include <string>

// internal
#include <miru/params/errors.hpp>
#include <miru/params/iterator.hpp>
#include <miru/params/parameter_fwd.hpp>
#include <miru/utils.hpp>

// external
#include <yaml-cpp/yaml.h>

#include <nlohmann/json.hpp>

namespace miru::params {

class Parameter;

// ================================ MAP ========================================== //
class Map {
 public:
  Map(const std::vector<Parameter> &fields);

  bool operator==(const Map &other) const;
  bool operator!=(const Map &other) const;

  // Map methods using Iterator
  ParameterIterator begin() const;
  ParameterIterator end() const;
  size_t size() const { return sorted_fields_.size(); }

  // Access a parameter by key
  const Parameter &operator[](const std::string &key) const;

 private:
  std::vector<Parameter> sorted_fields_;
};

std::string to_string(const Map &map);
std::ostream &operator<<(std::ostream &os, const Map &map);

// =============================== MAP ARRAY ======================================= //
class MapArray {
 public:
  MapArray(const std::vector<Parameter> &maps);
  MapArray(const std::vector<Map> &maps);

  // Iterator access methods
  ParameterIterator begin() const { return ParameterIterator(items_.begin()); }
  ParameterIterator end() const { return ParameterIterator(items_.end()); }
  size_t size() const { return items_.size(); }

  bool operator==(const MapArray &other) const;
  bool operator!=(const MapArray &other) const;

  // Access a parameter by index
  const Parameter &operator[](const size_t index) const;

 private:
  std::vector<Parameter> items_;
};

std::string to_string(const MapArray &map_array);
std::ostream &operator<<(std::ostream &os, const MapArray &map_array);

// =============================== NESTED ARRAY ==================================== //
class NestedArray {
 public:
  NestedArray(const std::vector<Parameter> &items);
  NestedArray(const std::vector<NestedArray> &items);

  // Iterator access methods
  ParameterIterator begin() const { return ParameterIterator(items_.begin()); }
  ParameterIterator end() const { return ParameterIterator(items_.end()); }
  size_t size() const { return items_.size(); }

  bool operator==(const NestedArray &other) const;
  bool operator!=(const NestedArray &other) const;

  // Access a parameter by index
  const Parameter &operator[](const size_t index) const;

 private:
  std::vector<Parameter> items_;
};

std::string to_string(const NestedArray &nested_array);
std::ostream &operator<<(std::ostream &os, const NestedArray &nested_array);

}  // namespace miru::params
