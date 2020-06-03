 /* note for cpp-assignmentOp */

/* custom assignment operator */
// must be a public member function of the class; 
// has the function name operator = ; 
// has a return value of a reference of the class' type
// has exactly one argument (const reference of the class' type)

// assign the content of the object to the instance of the class
Cube & Cube:: operator=(const Cube &pbj)

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

  Cube & Cube::operator=(const Cube & obj) {
    length_ = obj.length_;
    std::cout << "Assignment operator invoked!" << std::endl;    
    return *this;        // this is a pointer to the instance of the class itself
  }
}

#include "../Cube.h"
using uiuc::Cube;

int main() {
  Cube c;            // invoke custom default constructor
  Cube myCube;       // invoke custom default constructor

  myCube = c;        // assignment operation, myCube takes the content of c
                     // needs the assignment operation function to be defined first
                     
  return 0;
}


