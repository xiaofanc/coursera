/**
 * C++ code for creating a Cube of length 2.4 units.
 * - See ../cpp-std/main.cpp for a similar program with print statements.
 * 
 * @author
 *   Wade Fagen-Ulmschneider <waf@illinois.edu>
 */

#include <iostream>
#include "Cube.h"

int main() {
  Cube c;

  c.setLength(3.48);
  double volume = c.getVolume();
  std::cout << "Volume: " << volume << std::endl;

/*
  int i = 0, j = 1;
  int *ptr = &i;
  std::cout << "i: " << i << std::endl;

  i = 2;
  std::cout << "i: " << i << std::endl;
  *ptr = 3;
  std::cout << "i: " << i << std::endl;
  ptr = &j;
  std::cout << "i: " << i << std::endl;
  j = i;
  std::cout << "i: " << i << std::endl;
  *ptr = 4;
  
  std::cout << "i: " << i << std::endl;
*/

  int *i = new int;
  *i = 0;
  int &j = *i;


  std::cout << "*i: " << *i << std::endl;
  std::cout << "&i: " << &i << std::endl;
  std::cout << "j: " << j << std::endl;
  std::cout << "&j: " << &j << std::endl;

  j++; //// Incrementing j will increment both i and j

  std::cout << "*i: " << *i << std::endl;
  std::cout << "&i: " << &i << std::endl;
  std::cout << "j: " << j << std::endl;
  std::cout << "&j: " << &j << std::endl;

  char s1;
  char s2;
  s1 = 0x7fe79dc02710;  // &j
  s2 = 0x7fff54d478c0;  // &i

  if ((s1.compare(s2)) < 0) {std::cout << "address of j is smaller than address of i" << std::endl;}
  else {std::cout << "address of j is larger than address of i" << std::endl;}


  return 0;
}
