#include <functional>

template<typename Type, typename CmpType>
class Heap {
 public:
  using Comparer = std::function<bool(Type&, Type&)>;

  Heap(Comparer comparer = [](Type& a, Type& b){return a < b;},
       int capacity = 10)
  : comparer_(comparer) {
    elements_.reserve(capacity);
  }

  template<typename AType>
  void push(AType elm) {
    elements_.push_back(std::forward(elm));

    int currpos = size() - 1;
    while (currpos > 0) {
      int parent = currpos / 2;
      if (compare(currpos, parent)) {
        swap(currpos, parent);
      } else {
        break;
      }
    }
  }

  Type pop() {
    swap(0, size() - 1);
    int size = size() - 1;

    int currpos = 0;
    while (true) {
      int child1 = 2 * currpos, child2 = 2 * currpos + 1;
      if (child1 < size && compare(child1, currpos)) {
        if (child2 < size && compare(child2, child1)) {
          swap(child2, currpos);
        } else {
          swap(child1, currpos);
        }
      } else if (child2 < size && compare(child2, currpos)) {
        swap(child2, currpos);
      } else {
        break;
      }
    }

    return elements_.pop_back();
  }

  inline Type& peek() {
    return elements_.front();
  }

  inline int size() {
    return elements_.size();
  }

  inline bool empty() {
    return elements_.empty();
  }

 private:
  bool compare(int pos1, int pos2) {
    return comparer_(elements[pos1], elements[pos2]);
  }

  void swap(int& pos1, int& pos2) {
    iter_swap(elements_.begin() + pos1, elements_.begin() + pos2);
    std::swap(pos1, pos2);
  }

  Comparer comparer_;
  vector<Type> elements_;
};
