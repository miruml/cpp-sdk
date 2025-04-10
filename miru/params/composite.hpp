#pragma once

// std
#include <cstddef>
#include <string>

// internal
#include <miru/params/exceptions.hpp>
#include <miru/params/parameter_fwd.hpp>
#include <miru/params/scalar.hpp>
#include <miru/params/type.hpp>
#include <miru/utils.hpp>

// external
#include <yaml-cpp/yaml.h>

#include <nlohmann/json.hpp>

namespace miru::params {

class Parameter;

// ============================== PARAMETER ITERATOR ============================== //
class ParameterIterator {
public:
    using iterator_category = std::forward_iterator_tag;
    using value_type = Parameter;
    using reference = const Parameter&;
    using pointer = const Parameter*;
    using difference_type = std::ptrdiff_t;

    ParameterIterator(std::vector<Parameter>::const_iterator it);
    ParameterIterator(std::vector<std::pair<std::string, Parameter>>::const_iterator it);
    ParameterIterator& operator++();
    ParameterIterator operator++(int);
    reference operator*() const;
    pointer operator->() const;
    bool operator==(const ParameterIterator& other) const;
    bool operator!=(const ParameterIterator& other) const;

private:
    std::vector<Parameter>::const_iterator it_;
};

class ParametersView {
public:
    using difference_type = std::ptrdiff_t;

    ParametersView(ParameterIterator begin, ParameterIterator end)
        : begin_(begin), end_(end) {}

    ParameterIterator begin() const { return begin_; }
    ParameterIterator end() const { return end_; }
    bool empty() const { return begin_ == end_; }

private:
    ParameterIterator begin_;
    ParameterIterator end_;
};

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

// =============================== MAP ARRAY ======================================= //
class MapArray {
 public:
  MapArray(const std::vector<Parameter> &maps);

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

// =============================== NESTED ARRAY ==================================== //
class NestedArray {
 public:
  NestedArray(const std::vector<Parameter> &items);

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

} // namespace miru::params

