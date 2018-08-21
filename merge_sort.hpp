#ifndef MERGE_SORT_HPP
#define MERGE_SORT_HPP

#include <vector>
#include <iterator>
#include <algorithm>
#include <utility>

namespace sorting {
  namespace detail {
    template<typename InputIt>
    InputIt middle_iterator(InputIt begin, InputIt end, std::input_iterator_tag) noexcept {
      return std::next(begin, std::distance(begin, end) / 2);
    }

    template<typename RandomIt>
    RandomIt middle_iterator(RandomIt begin, RandomIt end, std::random_access_iterator_tag) noexcept {
      return std::next(begin, (end - begin) / 2);
    }
    
    template<typename InputIt>
    InputIt middle_iterator(InputIt begin, InputIt end) noexcept {
      using tag = typename std::iterator_traits<InputIt>::iterator_category;
      return detail::middle_iterator(begin, end, tag{});
    }

    template<typename InputIt, typename OutIt>
    OutIt move_if_noexcept(InputIt begin, InputIt end, OutIt out) noexcept {
      while (begin != end)
	*out++ = std::move_if_noexcept(*begin++);
      return out;
    }

    template<typename InputIt1, typename InputIt2, typename OutIt>
    OutIt merge(InputIt1 left, InputIt1 left_end, InputIt2 right, InputIt2 right_end, OutIt out) noexcept {
      while(left < left_end && right < right_end)
        (*left < *right) ? *out++ = std::move_if_noexcept(*left++) : *out++ = std::move_if_noexcept(*right++);
      
      detail::move_if_noexcept(left, left_end, out);
      detail::move_if_noexcept(right, right_end, out);
      return out;
    }

    template<typename InputIt1, typename InputIt2>
    void merge_sort(InputIt1 begin, InputIt1 end, InputIt2 temp_begin, InputIt2 temp_end) noexcept {
      if (std::distance(begin, end) > 1) {
	const auto mid = detail::middle_iterator(begin, end);
	const auto temp_mid = detail::middle_iterator(temp_begin, temp_end);
	detail::merge_sort(temp_begin, temp_mid, begin, mid);
	detail::merge_sort(temp_mid, temp_end, mid, end);
	detail::merge(temp_begin, temp_mid, temp_mid, temp_end, begin);
      }
    }
  }
   
  template<typename InputIt>
  void merge_sort(InputIt begin, InputIt end) {
    using value_type = typename std::iterator_traits<InputIt>::value_type;
    std::vector<value_type> temp;
    temp.reserve(end - begin);
    // replacement for no move_if_no_except_iterator
    for (auto i = begin; i != end; ++i)
      temp.push_back(std::move_if_noexcept(*i));
    detail::merge_sort(begin, end, temp.begin(), temp.end());
  }
}

#endif
