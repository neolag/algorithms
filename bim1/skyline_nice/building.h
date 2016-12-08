#pragma once

#include <iostream>

struct Building {
  int left, right;
  int height;
};

std::istream& operator>>(std::istream& s, Building& b) {
  return s >> b.left >> b.height >> b.right;
}

std::ostream& operator<<(std::ostream& s, const Building& b) {
  return s << b.left << ' ' << b.height << ' ' << b.right;
}
