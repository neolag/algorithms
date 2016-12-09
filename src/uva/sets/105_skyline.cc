#include <iostream>

#include <set>
#include <vector>

using namespace std;

struct Building {
  int left, right;
  int height;
};

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

istream& operator>>(istream& s, Building& b) {
  return s >> b.left >> b.height >> b.right;
}

/**
 * Helper function to erase a specific element from a multiset, instead of all
 * elements that are compared equal in it (but not necessarily entirely equal).
 */
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

/**
 * Builds the skyline of the city by iterating over the buildings in 2 sequences
 * ordered by the left and right edges respectively, additionally keeping a set
 * (implemented as a tree) of the current buildings "active" in the current
 * position, adding and removing them as they are iterated over in the by-left
 * and by-right sequences.
 */
vector<pair<int, int>> make_skyline(const vector<Building>& bldgs) {
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

  vector<pair<int, int>> skyline;
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
      // There will possibly be more changes in height in the current position.
      continue;
    }

    int height = current.empty() ? 0 : (*current.begin())->height;
    if (height != currheight) {
      currheight = height;
      skyline.push_back(make_pair(currpos, currheight));
    }
  }
  return skyline;
}


int main() {
  vector<Building> bldgs;

  Building b;
  // The eof flag is only set when we try to read a building after the input is
  // over, so read first, check for EOF and only then add to the building list.
  while (cin >> b && !cin.eof()) {
    bldgs.push_back(b);
  }

  bool first = true;
  for (auto point : make_skyline(bldgs)) {
    cout << (first ? "" : " ") << point.first << ' ' << point.second;
    if (first) first = false;
  }
  cout << endl;

  return 0;
}
