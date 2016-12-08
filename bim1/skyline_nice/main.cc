#include <iostream>

#include <math.h>

#include "105skyline.cc"
#include "105skyline_easy.cc"
#include "building.h"

#define MAX_BLDGS 5000
#define BLDGS_STEP 100
#define NUM_TRIES 1
#define MAX_DIST 10000

using namespace std;

inline int sqrand(int max) {
  return int(sqrt(rand() % (max * max)));
}

template <typename Type1, typename Type2>
ostream& operator<<(ostream& o, const pair<Type1, Type2>& p) {
  return o << p.first << ' ' << p.second;
}

template <typename Type>
ostream& operator<<(ostream& o, const vector<Type>& vec) {
  for (auto& v : vec) o << v << endl;
  return o;
}

int main() {
  srand(time(NULL));

  vector<Building> bldgs;
  for (int N = BLDGS_STEP; N <= MAX_BLDGS; N += BLDGS_STEP) {
    for (int t = 1; t <= NUM_TRIES; t++) {
      bldgs.clear();
      int maxDist = MAX_DIST / NUM_TRIES * t;
      for (int i = 0; i < N; i++) {
        int left = rand() % maxDist;
        int height = sqrand(maxDist);
        int right = left + sqrand(maxDist - left);
        bldgs.push_back({left, right, height});
      }

      cout << "Testing with " << N << " max dist " << maxDist << endl;
      auto sol1 = solve(bldgs);
      auto sol2 = solve_easy(bldgs);
      if (sol1 != sol2) {
        cout << "Found!" << endl
             << bldgs << endl
             << "sol1: " << sol1 << endl
             << "sol2: " << sol2 << endl << endl;
        cout << "Continue? (y/n): ";

        string s = "";
        while (cin >> s && !cin.eof()) {
          if (s == "y" || s == "n") break;
        }
        if (s == "n" || cin.eof()) return 0;
      }
    }
  }
  return 0;
}