#include <stdio.h>

#include <cstring>
#include <stdexcept>
#include <vector>

#define MAX_NODES 300

using std::vector;

template<typename Type>
struct Matrix {
  vector<vector<Type>> values;

  Matrix(int size) : values(size, vector<Type>(size)) {}

  void resize(int size) {
    values.resize(size);
    for (int i = 0; i < size; i++) values[i].resize(size);
  }

  vector<Type>& operator[](int idx) {
    return values[idx];
  }

  
};

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
  int N;
  AdjMatrix G;
  Solution sol;
  while (scanf("%d", &N) > 0) {
    std::memset(G, 0, sizeof(G));
    sol.reset();

    int i, j;
    while (scanf("%d %d", &i, &j) > 0) {
      G[i - 1][j - 1] = G[j - 1][i - 1] = true;
    }
    scanf(" %%");

    if (tryCycle(G, N, &sol)) {
      for (int i = 0; i < sol.pathSize; i++) {
        printf(i == 0 ? "%d" : " %d", sol.path[i] + 1);
      }
      printf("\n");
    } else {
      printf("N\n");
    }
  }
}
