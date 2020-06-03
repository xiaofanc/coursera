/**
 * C++ program printing various memory values with references and pointers.
 * 
 * @author
 *   Wade Fagen-Ulmschneider <waf@illinois.edu>
 *
  num: 7
  &num: 0x7fff5c88e8f8
   p: 0x7fff5c88e8f8
  &p: 0x7fff5c88e8f0
  *p: 7
  *p changed to 42
  num: 42
 */

#include <iostream>

int main() {
  int num = 7;
  std::cout << " num: " <<  num << std::endl;
  std::cout << "&num: " << &num << std::endl;

  int *p = &num;
  std::cout << " p: " <<  p << std::endl;
  std::cout << "&p: " << &p << std::endl;
  std::cout << "*p: " << *p << std::endl;

  *p = 42;
  std::cout << "*p changed to 42" << std::endl; 
  std::cout << " num: " <<  num << std::endl;

  return 0;
}

