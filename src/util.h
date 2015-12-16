template<class InputIt, class UnaryPredicate>
  InputIt nth_where(InputIt first, InputIt last, int count, UnaryPredicate p) {
  for (; first != last; ++first) {
    if (p(*first)) {
      if (count==0) {
        return first;
      }
      --count;
    }
  }
  return last;
}
