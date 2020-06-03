/* note for cpp-cctor folder */

/* automatic copy constructor */
// copy constructor: a class constructor, one argument (constant reference of the same type as class)

Cube::Cube(const Cube & obj)

// example
namespace uiuc {
  Cube::Cube() {
    length_ = 1;
    std::cout << "Default constructor invoked!" << std::endl;
  }

  Cube::Cube(const Cube & obj) {
    length_ = obj.length_;
    std::cout << "Copy constructor invoked!" << std::endl;
  }
}

// ex1
#include "../Cube.h"
using uiuc::Cube;

void foo(Cube cube) {    // pass object cube as parameter
  // Nothing :)
}

int main() {
  Cube c; // invoke custom default constructor
  foo(c); // invoke custom copy constructor

  return 0;
}

// ex2
#include "../Cube.h"
using uiuc::Cube;

Cube foo() {
  Cube c;            // a cube called c was created on stack -> 1: invoke default constructor
  return c;          // return a default constructor    
                     // 2: invoke copy constructor -> in order to go back in main stack frame
}

int main() {
  Cube c2 = foo();   // cube c2 takes on the return value of foo 
                     // 3: invoke copy constructor -> copy from the main stack frame into the variable c2
  return 0;
}

// ex3
#include "../Cube.h"
using uiuc::Cube;

int main() {
  Cube c;            // invoke custom default constructor
  Cube myCube = c;   // invoke custom copy constructor

  return 0;
}

// ex4
#include "../Cube.h"
using uiuc::Cube;

int main() {
  Cube c;            // invoke custom default constructor
  Cube myCube;       // invoke custom default constructor

  myCube = c;        // constructor was used to create an object itself, not just copy things between two existing objects
                     // assignment operation, this will not invoke anything

  return 0;
}


