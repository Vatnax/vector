#pragma once

#include <type_traits>
#include <iterator>

template<typename Iter>
using require_input_it = std::enable_if_t<std::is_base_of_v<std::input_iterator_tag, typename std::iterator_traits<Iter>::iterator_category>>;

template<typename Iter>
using require_forward_it = std::enable_if_t<std::is_base_of_v<std::forward_iterator_tag, typename std::iterator_traits<Iter>::iterator_category>>;

template<typename Iter>
using require_bidirectional_it = std::enable_if_t<std::is_base_of_v<std::bidirectional_iterator_tag, typename std::iterator_traits<Iter>::iterator_category>>;

template<typename Iter>
using require_random_access_it = std::enable_if_t<std::is_base_of_v<std::random_access_iterator_tag, typename std::iterator_traits<Iter>::iterator_category>>;

template<typename Iter>
using is_random_access_it = std::enable_if_t<std::is_same_v<std::random_access_iterator_tag, typename std::iterator_traits<Iter>::iterator_category>>;

template<typename Iter>
using is_not_random_access_it = std::enable_if_t<!std::is_same_v<std::random_access_iterator_tag, typename std::iterator_traits<Iter>::iterator_category>>;

template<bool TEST, typename Type>
using const_T_or_nonconst_T = std::conditional_t<TEST, const Type, Type>;