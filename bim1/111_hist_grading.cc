#include <iostream>

#include <vector>
#include <algorithm>

using namespace std;

/**
 * Reads the historic event order in the confusing way specified in the problem.
 */
bool readReverse(vector<int>& out) {
  for (int i = 0; i < out.size(); i++) {
    int order;
    cin >> order;
    if (cin.eof()) return false;
    out[order - 1] = i;
  }
  return true;
}

template <typename Type>
void print(const vector<Type>& vec) {
  for (auto& v : vec) cout << v << ' ';
  cout << endl;
}

int main() {
  int N;
  cin >> N;
  vector<int> original(N);
  readReverse(original);

  // Perform the LCS algorithm with only a single line of extra memory, since
  // problems only depend on sub-problems of the previous line. Calculate them
  // in bottom-up order so that we only need to store one line at once.
  vector<int> mem(N), test(N);
  while (readReverse(test)) {
    fill(mem.begin(), mem.end(), 0);
    int prev_memj;
    for (int i = 0; i < N; i++) {
      prev_memj = 0; // the previous value of mem[j - 1] of the previous line
      for (int j = 0; j < N; j++) {
        int new_v;
        if (original[i] == test[j]) {
          new_v = prev_memj + 1;
        } else {
          new_v = max(j > 0 ? mem[j - 1] : 0, mem[j]);
        }
        prev_memj = mem[j];
        mem[j] = new_v;
      }
    }
    cout << mem[N - 1] << endl;
  }
  return 0;
}