#pragma once

#include <iterator>
#include <ranges>

template <std::forward_iterator Iterator>
struct ForwardIterator {
    using ValueType = decltype(*std::declval<Iterator>());
    using Pointer = decltype(std::declval<Iterator>().operator->());
    ForwardIterator(Iterator begin, Iterator end) : m_cur(begin), m_end(end) {}
    template <std::ranges::forward_range Range>
    ForwardIterator(Range &&range) : m_cur(std::ranges::begin(range)), m_end(std::ranges::end(range)) {}
    operator bool() const { return m_cur != m_end; }
    ValueType& operator*() const { return *m_cur; }
    Pointer operator->() const { return m_cur.operator->(); }
    ForwardIterator& operator++() { ++m_cur; return *this; }
    ForwardIterator operator++(int) { ForwardIterator tmp(*this); ++m_cur; return tmp; }
    Iterator m_cur;
    Iterator m_end;
};