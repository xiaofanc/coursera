/* note for cpp-templates */

// A template variable is defined by declaring it before the 
// beginning of a class or function
template <typename T>  // can take on any type
class List {} / int max(T a, T b) {}

#include <iostream>
using std::cout;
using std::endl;

#include "Cube.h"
using uiuc::Cube;

template <typename T>
T max(T a, T b) {
  if (a > b) { return a; }
  return b;
}

int main() {
  cout << "max(3, 5): " << max(3, 5) << endl;
  cout << "max('a', 'd'): " << max('a', 'd') << endl;
  cout << "max(\"Hello\", \"World\"): " << max("Hello", "World") << endl;
  // cannot compare cube(3) and cube(6)
  cout << "max( Cube(3), Cube(6) ): " << max( Cube(3), Cube(6) ) << endl;

  return 0;
}
