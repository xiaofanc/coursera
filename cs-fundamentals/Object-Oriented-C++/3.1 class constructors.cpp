/* note for cpp-ctor folder */

/* automatic default constructor */
no cube();

/* custom default constructor + custom non-default constructor */
// custom default constructor: same name as class, no argument, return nothing
// if any custom default constructor exists, then no automatic defaut constructor will be given 
// - in header file
Cube::Cube() {
}

namespace uiuc {
  Class Cube {
    Cube();   // custom default constructor -> no return values
    Cube(double length) // one-argument constructor
  }
}

// - in implementation file
namespace uiuc {
  Cube::Cube() {
    length_ = 1;
  }

  Cube::Cube(double length) {
    length_ = length;
  }

  }

// another example ex2
int main() {
  uiuc::Cube c(2);     // search all constructors and find the constructor that takes one parameter
  std::cout << "Volume: " << c.getVolume() << std::endl;
  return 0;
}

// ex3
// cannot call an undefined default constructor




