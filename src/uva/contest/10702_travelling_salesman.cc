// UVA Problem 10.702:
// https://uva.onlinejudge.org/index.php?option=com_onlinejudge&Itemid=8&page=show_problem&problem=1643

#include <iostream>
#include <vector>
#include <algorithm>

using std::cin;
using std::cout;

int main() {
  std::ios::sync_with_stdio(false);
  int numCities, startCity, numDests, numTravels;
  int gains[101][101];

  std::vector<int> maxProfit(101), nextMaxProfit(101);

  while (!cin.eof()) {
    cin >> numCities >> startCity >> numDests >> numTravels;
    if (numCities == 0) break;

    for (int i = 0; i < numCities; i++) {
      for (int j = 0; j < numCities; j++) {
        cin >> gains[i][j];
      }
    }
    for (int i = 0; i < numCities; i++) {
      maxProfit[i] = nextMaxProfit[i] = 0;
    }
    int endCity;
    for (int i = 0; i < numDests; i++) {
      cin >> endCity;
      endCity--;
      for (int j = 0; j < numCities; j++) {
        if (gains[j][endCity] > maxProfit[j]) {
          maxProfit[j] = gains[j][endCity];
        }
      }
    }

    for (int t = 1; t < numTravels; t++) {
      for (int i = 0; i < numCities; i++) {
        for (int j = 0; j < numCities; j++) {
          int profitGoingThroughJ = gains[i][j] + maxProfit[j];
          if (profitGoingThroughJ > nextMaxProfit[i]) {
            nextMaxProfit[i] = profitGoingThroughJ;
          }
        }
      }
      std::swap(maxProfit, nextMaxProfit);
    }

    cout << maxProfit[startCity - 1] << std::endl;
  }
}