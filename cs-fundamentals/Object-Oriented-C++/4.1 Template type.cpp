/* note for cpp-vector */

/* Templates */
// Template type: vector -> dynamically growing array
#include <vector>
std::vector<T> v;
v.push_back(4); // add to the back of array
v[0];    // access specific element
v.size() // number of elements

// ex1 main.cpp
#include <vector>
#include <iostream>

int main() {
  std::vector<int> v;
  v.push_back( 2 );
  v.push_back( 3 );
  v.push_back( 5 );

  std::cout << v[0] << std::endl;
  std::cout << v[1] << std::endl;
  std::cout << v[2] << std::endl;

  return 0;
}

// ex2 main.cpp
#include <vector>
#include <iostream>

int main() {
  std::vector<int> v;
  for (int i = 0; i < 100; i++) {
    v.push_back( i * i );
  }

  std::cout << v[12] << std::endl; // 144

  return 0;
}



