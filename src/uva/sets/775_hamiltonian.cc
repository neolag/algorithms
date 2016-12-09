#include <iostream>

#include <cstring>
#include <stdexcept>

#define MAX_NODES 300

struct Solution {
  int pathSize = 0;
  int path[MAX_NODES + 1];
  bool marked[MAX_NODES];

  void reset() {
    pathSize = 0;
    std::memset(marked, 0, sizeof(marked));
  }

  void addNode(int n) {
    if (pathSize == MAX_NODES + 1) throw std::logic_error("solution full");
    path[pathSize++] = n;
    marked[n] = true;
  }

  void popNode() {
    if (pathSize == 0) throw std::logic_error("solution empty");
    marked[path[--pathSize]] = false;
  }

  int lastNode() {
    if (pathSize == 0) throw std::logic_error("solution empty");
    return path[pathSize - 1];
  }
};

std::ostream& operator<<(std::ostream& s, const Solution& sol) {
  for (int i = 0; i < sol.pathSize; i++) {
    if (i > 0) s << ' ';
    s << (sol.path[i] + 1);
  }
  return s;
}

typedef bool AdjMatrix[MAX_NODES][MAX_NODES];

bool tryCycle(const AdjMatrix& G, int numNodes, Solution* sol) {
  if (sol->pathSize == 0) {
    sol->addNode(0);
    return tryCycle(G, numNodes, sol);
  }

  int lastNode = sol->lastNode();
  for (int i = 0; i < numNodes; i++) {
    if (G[lastNode][i]) {
      if (!sol->marked[i]) {
        sol->addNode(i);
        if (tryCycle(G, numNodes, sol)) return true;
        sol->popNode();
      } else if (i == sol->path[0] && sol->pathSize == numNodes) {
        sol->addNode(i);
        return true;
      }
    }
  }
  return false;
}

int main() {
  using namespace std;
  int N;
  AdjMatrix G;
  Solution sol;

  while (cin >> N && !cin.eof()) {
    memset(G, 0, sizeof(G));
    sol.reset();

    int i, j;
    do {
      cin >> i >> j >> std::ws;
      G[i - 1][j - 1] = G[j - 1][i - 1] = true;
    } while(cin.peek() != '%');
    cin.ignore(1);

    if (tryCycle(G, N, &sol)) {
      cout << sol << endl;
    } else {
      cout << 'N' << endl;
    }
  }
}
