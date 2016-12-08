#include <iostream>

#include <set>
#include <tuple>
#include <vector>

#include "building.h"

using namespace std;

struct cmp_by_left {
  bool operator()(const Building* b1, const Building* b2) {
    return b1->left < b2->left;
  }
};

struct cmp_by_right {
  bool operator()(const Building* b1, const Building* b2) {
    return b1->right < b2->right;
  }
};

struct cmp_by_height_inv {
  bool operator()(const Building* b1, const Building* b2) {
    return b1->height > b2->height;
  }
};

template<typename Type, typename Compare>
bool erase_equals(multiset<Type, Compare>* s, const Type& k) {
  auto range = s->equal_range(k);
  for (auto it = range.first; it != range.second; ++it) {
    if (*it == k) {
      s->erase(it);
      return true;
    }
  }
  return false;
}

vector<pair<int, int>> solve(const vector<Building>& bldgs) {
  multiset<const Building*, cmp_by_left> by_left_set;
  multiset<const Building*, cmp_by_right> by_right_set;
  for (auto& bldg : bldgs) {
    by_left_set.insert(&bldg);
    by_right_set.insert(&bldg);
  }

  multiset<const Building*, cmp_by_height_inv> current;
  int currpos = 0, currheight = 0;

  auto by_left = by_left_set.begin();
  auto by_right = by_right_set.begin();

  vector<pair<int, int>> result;
  while (by_right != by_right_set.end()) {
    if (by_left != by_left_set.end() &&
        (*by_left)->left <= (*by_right)->right) {
      auto* next_start = *by_left++;
      currpos = next_start->left;
      current.insert(next_start);
    } else {
      auto* next_end = *by_right++;
      currpos = next_end->right;
      erase_equals(&current, next_end);
    }
    if ((by_left != by_left_set.end() && (*by_left)->left == currpos) ||
        (by_right != by_right_set.end() && (*by_right)->right == currpos)) {
      continue;
    }

    int height = current.empty() ? 0 : (*current.begin())->height;
    if (height != currheight) {
      currheight = height;
      result.push_back(make_pair(currpos, currheight));
    }
  }
  return result;
}
