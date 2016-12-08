#include <stdio.h>

#include <algorithm>
#include <vector>

int main() {
  std::vector<int> skyline(10001, 0);

  int L, H, R;
  int maxR = 0;
  while (scanf("%d %d %d", &L, &H, &R) > 0) {
    for (int i = L; i < R; i++) {
      skyline[i] = std::max(skyline[i], H);
    }
    maxR = std::max(maxR, R);
  }

  int currheight = 0;
  for (int p = 0; p < maxR; p++) {
    if (skyline[p] != currheight) {
      currheight = skyline[p];
      printf("%d %d ", p, currheight);
    }
  }
  printf("%d %d\n", maxR, 0);
  return 0;
}