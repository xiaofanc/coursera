/**
 * C++ program invoking Cube's assignment operator.
 * 
 * @author
 *   Wade Fagen-Ulmschneider <waf@illinois.edu>
 */

#include "Cube.h"
#include <iostream>
using uiuc::Cube;

Cube intersect(Cube &left, Cube &right) {
    return *(new Cube);
}

int main() {
  // Cube c;
  // Cube myCube;
  // myCube = c;

  Cube a(10), b(5);
  Cube c = intersect(a, b);

  std::cout << std::endl;

  Cube d;
  d = intersect(a, b);
  return 0;
}
