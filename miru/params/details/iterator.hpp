#pragma once

// std
#include <cstddef>

// internal
#include <miru/params/parameter_fwd.hpp>
#include <miru/params/iterator.hpp>

namespace miru::params::details {

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

}  // namespace miru::params::details