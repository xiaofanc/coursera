/**
 * Simple C++ class for representing a Cube.
 * 
 * @author
 *   Wade Fagen-Ulmschneider <waf@illinois.edu>
 */

#include "Cube.h"

// set up pa and pb to point to newly allocated memory 
// locations on the heap. The integers at those memory 
// locations should be assigned values according to the 
// constructor's integer arguments a and b.

Pair::Pair(int a, int b) {
    int *pa = new int(a);
    int *pb = new int(b);
};

Pair::Pair(const Pair &other) {
    int *pa = other.pa;
    int *pb = other.pb;
}

Pair::~Pair() {
    delete pa;
    delete pb;
};

/*
double Cube::getVolume() {
  return length_ * length_ * length_;
}

double Cube::getSurfaceArea() {
  return 6 * length_ * length_;
}

void Cube::setLength(double length) {
  length_ = length;
}
*/