#include <stdio.h>

#include <algorithm>
#include <tuple>
#include <vector>

#include "building.h"

using namespace std;

vector<pair<int, int>> solve_easy(const vector<Building>& bldgs) {
  vector<int> skyline(10001, 0);

  int maxR = 0;
  for (auto& bldg : bldgs) {
    for (int i = bldg.left; i < bldg.right; i++) {
      skyline[i] = std::max(skyline[i], bldg.height);
    }
    maxR = max(maxR, bldg.right);
  }

  vector<pair<int, int>> result;
  int currheight = 0;
  for (int p = 0; p <= maxR; p++) {
    if (skyline[p] != currheight) {
      currheight = skyline[p];
      result.push_back(make_pair(p, currheight));
    }
  }

  return result;
}
