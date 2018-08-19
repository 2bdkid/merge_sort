#ifndef MERGE_SORT_HPP
#define MERGE_SORT_HPP

#include <vector>
#include <iterator>
#include <algorithm>
#include <utility>

namespace sorting {
  namespace detail {
    template<typename RandomIt>
    RandomIt middle_iterator(RandomIt begin, RandomIt end) noexcept {
      return std::next(begin, (end - begin) / 2);
    }

    template<typename InputIt, typename OutIt>
    OutIt move_if_noexcept(InputIt begin, InputIt end, OutIt out) noexcept {
      while (begin != end)
	*out++ = std::move_if_noexcept(*begin++);
      return out;
    }

    template<typename RandomIt1, typename RandomIt2, typename OutIt>
    OutIt merge(RandomIt1 left, RandomIt1 left_end, RandomIt2 right, RandomIt2 right_end, OutIt out) noexcept {
      while(left < left_end && right < right_end) {
        (*left < *right) ? *out++ = std::move_if_noexcept(*left++) : *out++ = std::move_if_noexcept(*right++);
       }
      
      detail::move_if_noexcept(left, left_end, out);
      detail::move_if_noexcept(right, right_end, out);
      return out;
    }

    template<typename RandomIt1, typename RandomIt2>
    void merge_sort(RandomIt1 begin, RandomIt1 end, RandomIt2 scratch_begin, RandomIt2 scratch_end) noexcept {
      if (end - begin > 1) {
	const auto mid = detail::middle_iterator(begin, end);
	const auto scratch_mid = detail::middle_iterator(scratch_begin, scratch_end);
	detail::merge_sort(scratch_begin, scratch_mid, begin, mid);
	detail::merge_sort(scratch_mid, scratch_end, mid, end);
	detail::merge(scratch_begin, scratch_mid, scratch_mid, scratch_end, begin);
      }
    }
  }
  
  template<typename RandomIt>
  void merge_sort(RandomIt begin, RandomIt end) noexcept {
    using value_type = typename std::iterator_traits<RandomIt>::value_type;
    std::vector<value_type> scratch_buffer;
    scratch_buffer.reserve(end - begin);
    // replacement for no move_if_no_except_iterator
    for (auto i = begin; i != end; ++i)
      scratch_buffer.push_back(std::move_if_noexcept(*i));
    detail::merge_sort(begin, end, scratch_buffer.begin(), scratch_buffer.end());
  }
}

#endif
