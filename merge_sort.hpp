#ifndef MERGE_SORT_HPP
#define MERGE_SORT_HPP

#include <vector>
#include <iterator>
#include <algorithm>
#include <utility>

namespace sorting {
  namespace detail {
    template<typename RandomIt>
    RandomIt middle_iterator(RandomIt begin, RandomIt end) {
      return std::next(begin, (end - begin) / 2);
    }

    template<typename RandomIt1, typename RandomIt2, typename OutIt>
    OutIt merge(RandomIt1 left, RandomIt1 left_end, RandomIt2 right, RandomIt2 right_end, OutIt out) {
      while(left < left_end && right < right_end) {
	if (*left < *right) {
	  *out++ = std::move(*left++);
	} else {
	  *out++ = std::move(*right++);
	}
      }

      std::move(left, left_end, out);
      std::move(right, right_end, out);
      return out;
    }
  }

  template<typename RandomIt1, typename RandomIt2>
  void merge_sort(RandomIt1 begin, RandomIt2 end, RandomIt2 scratch_begin, RandomIt2 scratch_end) {
    if (end - begin > 1) {
      const auto mid = detail::middle_iterator(begin, end);
      const auto scratch_mid = detail::middle_iterator(scratch_begin, scratch_end);
      merge_sort(scratch_begin, scratch_mid, begin, mid);
      merge_sort(scratch_mid, scratch_end, mid, end);
      detail::merge(scratch_begin, scratch_mid, scratch_mid, scratch_end, begin);
    }
  }

  template<typename RandomIt>
  void merge_sort(RandomIt begin, RandomIt end) {
    using value_type = typename std::iterator_traits<RandomIt>::value_type;
    auto scratch_buffer = std::vector<value_type>(std::make_move_iterator(begin), std::make_move_iterator(end));
    merge_sort(begin, end, scratch_buffer.begin(), scratch_buffer.end());
  }
}

#endif
