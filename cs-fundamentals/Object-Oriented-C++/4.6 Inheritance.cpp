/* note for cpp-inheritance  */

// Inheritance allows for a class to inherit all member functions 
// and data from a base class into a derived class

#pragma once

// base class
// class shape only has basic things that are shared between shapes
// shape.h
class Shape {
  public:
    Shape();
    Shape(double width);
    double getWidth() const; // get access to private member variable

  private:
    double width_;
};

//cube.h
#pragma once

#include "Shape.h"  // include base class
#include "HSLAPixel.h"

namespace uiuc {
  // declare that shape is the base class
  // class cube inherits class shape
  // public inheritance
  class Cube : public Shape {    // declare that shape is the base class
    public:
      Cube(double width, uiuc::HSLAPixel color);
      double getVolume() const;  // specific to cube itself

    private:
      // do not need length since base class shape covers that
      // only cube has color
      uiuc::HSLAPixel color_;
  };
}

// cube.cpp constructing 
#include "Cube.h"
#include "Shape.h"

namespace uiuc {
  // cube constructor takes two parameters
  // initalize shape class by using the shape constructor that
  // takes in width as a parameter
  
  // custom constructor can be called with an initialization list
  // shape gets constructed first, then the cube constructor
  Cube::Cube(double width, uiuc::HSLAPixel color) : Shape(width) {
    // unique to cube classes
    color_ = color;
  }

  double Cube::getVolume() const {
    // Cannot access Shape::width_ due to it being `private`
    // ...instead we use the public Shape::getWidth(), a public function
    
    // access control:
    // - can access all public members of the base class, not the private var
    return getWidth() * getWidth() * getWidth();
  }
}

// initializer list:
// - initialize a base class
// - build the current class using another constructor
// - initialize the default values of member variables
// shape.cpp
#include "Shape.h"

// default constructor of shape is using one-parameter construtor
// to initilize our object
Shape::Shape() : Shape(1) {   
  // Nothing.
}
// one parameter constructor initialize private member variable(width)
Shape::Shape(double width) : width_(width) {
  // Nothing.
}

double Shape::getWidth() const {
  return width_;
}

//main.cpp
#include <iostream>

#include "Cube.h"
#include "HSLAPixel.h"

int main() {
  uiuc::Cube c(4, uiuc::HSLAPixel::PURPLE);
  std::cout << "Created a Purple cube!" << std::endl;
  return 0;
}








