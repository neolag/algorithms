#include <iostream>

using namespace std;

int main() {
  int N;
  int table[101][101];
  int mem[101][101][101];

  // Read the table.
  cin >> N;
  for (int i = 0; i < N; i++) {
    for (int j = 0; j < N; j++) {
      cin >> table[i][j];
    }
  }

  // Calculate intermediate sums for all lines in the table, correspondent to
  // every possible horizontal interval.
  for (int i = 0; i < N; i++) {
    for (int start_j = 0; start_j < N; start_j++) {
      mem[i][start_j][start_j] = table[i][start_j];
      for (int end_j = start_j + 1; end_j < N; end_j++) {
        mem[i][start_j][end_j] = mem[i][start_j][end_j - 1] + table[i][end_j];
      }
    }
  }

  // Now perform the 1-dimension max-sum algorithm on each of the interval sums
  // calculated in the previous step for consecutive lines, thus passing through
  // all possible maximum sums and finding the global maximum.
  int best;
  for (int start_j = 0; start_j < N; start_j++) {
    for (int end_j = start_j; end_j < N; end_j++) {
      int sum = 0;
      for (int i = 0; i < N; i++) {
        sum += mem[i][start_j][end_j];
        if (sum > best) best = sum;
        if (sum < 0) sum = 0;
      }
    }
  }

  cout << best << endl;
  return 0;
}