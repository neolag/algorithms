#include <algorithm>
#include <vector>

template<typename Type, typename Comparator = std::less<Type>>
class heap {
 public:
  heap(int capacity, Comparator cmp) : cmp_(std::move(cmp)) {
    data_.reserve(capacity);
  }

  bool empty() {
    return data_.empty();
  }

  void clear() {
    data_.clear();
  }

  Type& head() {
    return data_.front();
  }

  void push(const Type& t) {
    data_.push_back(t);
    std::push_heap(data_.begin(), data_.end(), cmp_);
  }

  void pop() {
    std::pop_heap(data_.begin(), data_.end(), cmp_);
    data_.pop_back();
  }

  Type pop_copy() {
    Type val = head();
    pop();
    return val;
  }
 private:
  std::vector<Type> data_;
  Comparator cmp_;
};

template<typename Type, typename Comparator = std::less<Type>>
heap<Type, Comparator> make_heap(int cap = 0, Comparator cmp = Comparator()) {
  return heap<Type, Comparator>(cap, cmp);
}
