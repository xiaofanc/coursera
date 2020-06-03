/* note for cpp-dtor folder */

/* automatic default destructor */
// added if no other destructor is defined

/* custom default destructor */
// member function; destructor is the name of the class, preceded by tilde ~; no argements and no return type
Cube::~Cube()

// example
Cube.cpp
#include "Cube.h"
#include <iostream>

using std::cout;
using std::endl;

namespace uiuc {  
  Cube::Cube() {
    length_ = 1;
    cout << "Created $1 (default)" << endl;
  }

  Cube::Cube(double length) {
    length_ = length;
    cout << "Created $" << getVolume() << endl;
  }

  Cube::Cube(const Cube & obj) {
    length_ = obj.length_;
    cout << "Created $" << getVolume() << " via copy" << endl;
  }

  Cube::~Cube() {
    cout << "Destroyed $" << getVolume() << endl;
  }

  Cube & Cube::operator=(const Cube & obj) {
    cout << "Transformed $" << getVolume() << "-> $" << obj.getVolume() << endl;
    length_ = obj.length_;
    return *this;
  }
}

main.cpp
#include "Cube.h"
using uiuc::Cube;

double cube_on_stack() {
  Cube c(3);              // create $27
  return c.getVolume();   // return volumn, destory $27
}

void cube_on_heap() {
  Cube * c1 = new Cube(10); // create $1000
  Cube * c2 = new Cube;     // create $1 (default)
  delete c1;                // destory $1000, c2 is not destoried since it is heap memory
}

int main() {
  cube_on_stack();
  cube_on_heap();
  cube_on_stack();
  return 0;
}

