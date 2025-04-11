#pragma once

// std
#include <cstddef>
#include <iterator>
#include <vector>

// internal
#include <miru/params/parameter_fwd.hpp>

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

    ParameterIterator(std::vector<Parameter>::const_iterator it) : it_(it) {}

    ParameterIterator& operator++() { ++it_; return *this; }
    ParameterIterator operator++(int) {
        ParameterIterator tmp = *this;
        ++it_;
        return tmp;
    }

    reference operator*() const { return *it_; }

    pointer operator->() const { return &(*it_); }

    bool operator==(const ParameterIterator& other) const { return it_ == other.it_; }

    bool operator!=(const ParameterIterator& other) const { return it_ != other.it_; }

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

}  // namespace miru::params