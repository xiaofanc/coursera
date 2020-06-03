/* note for cpp-memory2 */

/* variable storage */
// store in memory directly; store by pointer; store by reference
/**
 * Simple C++ class for representing a Cube (with constructors).
 * 
 * @author
 *   Wade Fagen-Ulmschneider <waf@illinois.edu>
 */

#include "Cube.h"
#include <iostream>

namespace uiuc {  
// three difference functions of Cube class

// create a new cube when we pass parameter in
  Cube::Cube(double length) {
    length_ = length;
    std::cout << "Created $" << getVolume() << std::endl;
  }

// do not create a new cube when we pass parameter in
// alias cube was copied to this newly constructed cube
  Cube::Cube(const Cube & obj) {  // takes a cube by reference
    length_ = obj.length_;
    std::cout << "Created $" << getVolume() << " via copy" << std::endl;
  }

// transform a value into another value
  Cube & Cube::operator=(const Cube & obj) {
    std::cout << "Transformed $" << getVolume() << "-> $" << obj.getVolume() << std::endl;
    length_ = obj.length_;
    return *this;
  }

  double Cube::getVolume() const {
    return length_ * length_ * length_;
  }

  double Cube::getSurfaceArea() const {
    return 6 * length_ * length_;
  }

  void Cube::setLength(double length) {
    length_ = length;
  }
}


// ex1 byvalue.cpp
#include "../Cube.h"
using uiuc::Cube;

int main() {
  // Create a 1,000-valued cube
  Cube c(10);

  // Transfer the cube
  Cube myCube = c;    // copy constructor is called!!

  return 0;
}

// byvalue.cpp creates another $1000 by copy

// ex1 byreference.cpp
#include "../Cube.h"
using uiuc::Cube;

int main() {
  // Create a 1,000-valued cube
  Cube c(10);

  // Transfer the cube
  Cube & myCube = c;   // myCube is the alias of c

  return 0;
}

// byreference.cpp only created one $1000 and with names as myCube and c, does not create extra memory

// ex1 bypointer.cpp
#include "../Cube.h"
using uiuc::Cube;

int main() {
  // Create a 1,000-valued cube
  Cube c(10);

  // Transfer the cube
  Cube * myCube = &c;  // myCube is a pointer that points to the memory that c is defined in

  return 0;
}

// bypointer.cpp only created one $1000 as well, but it uses extra memory

// ex2 send money byvalue.cpp
#include "../Cube.h"
using uiuc::Cube;

bool sendCube(Cube c) {    
  // ... logic to send a Cube somewhere ...
  return true;
}

int main() {
  // Create a 1,000-valued cube
  Cube c(10);

  // Send the cube to someone
  sendCube(c);     // send a copy of the cube, not itself

  return 0;
}

// ex2 send money byreference.cpp
#include "../Cube.h"
#include <iostream>

using uiuc::Cube;

bool sendCube(Cube & c) {    // reference of c 
  // ... logic to send a Cube somewhere ...
  return true;
}

int main() {
  // Create a 1,000-valued cube
  Cube c(10);

  // Send the cube to someone
  sendCube(c);

  return 0;
}

// ex2 send money bypointer.cpp
#include "../Cube.h"
#include <iostream>

using uiuc::Cube;

bool sendCube(Cube * c) {     // because Cube is a pointer, we need a memory address as input
  // ... logic to send a Cube somewhere ...
  return true;
}

int main() {
  // Create a 1,000-valued cube
  Cube c(10);

  // Send the cube to someone
  sendCube(&c);   // send memory address of c into sendCube

  return 0;
}


