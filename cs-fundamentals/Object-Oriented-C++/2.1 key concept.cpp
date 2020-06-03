/* notes for ccp-memory folder */

key concepts:

/* Stack memory: */
// default memory
&: return address of memory
note: lifecycle of any variable created inside of the function is as long as the function is being ran
stack memory always starts from high addresses and grows down

/* Pointers */
// stores the memory address of the data;
// level of indirection from the data;
integer pointer: int * p = &num;

/* dereference operator *   */:
# get the content that p point to;
int num = 7;
int * p = &num;            # p points to num
int value_in_num = * p;    # value_in_num gets the content that * p points to -> value_in_num = 7;
* p = 42;                  # num was updated to 42;

example:
# never return a reference to a local variable
Cube *CreateUnitCube() {
  Cube cube;
  cube.setLength(15);
  return &cube;                 # return address of cube and release that memory
}

int main() {
  Cube *c = CreateUnitCube();   # c points to cube address
  someOtherFunction();          # memory for cube was overwritten since it returns the address of the memory
  double a = c->getSurfaceArea();
  std::cout << "Surface Area: " << a << std::endl;
  double v = c->getVolume();
  std::cout << "Volume: " << v << std::endl;
  return 0;
}

/* note for cpp-Memory folder */
// Stack memory is dependent of the lifecycle of a function
// The address of any memory location in the Stack is larger than the address of any memory location in the heap

example: main.cpp
int main() {
  int num = 7;
  std::cout << " num: " <<  num << std::endl;
  std::cout << "&num: " << &num << std::endl;    // address of num -> large number

  int *p = &num;                                 // p is a pointer that we set to be the address of num
  std::cout << " p: " <<  p << std::endl;        // content of p is address of num
  std::cout << "&p: " << &p << std::endl;        // address of p, which is smaller than the address of num
  std::cout << "*p: " << *p << std::endl;        // *p = 7, which means what p points to

  *p = 42;
  std::cout << "*p changed to 42" << std::endl;  // change num to 42
  std::cout << " num: " <<  num << std::endl;

  return 0;
}

/* Heap memory */
/* note for cpp-heapMemory folder */
// Heap memory allows us to create memory independent of the lifecycle of a function
// Heap memory starts at low addresses and grows up
// new operator return a pointer to the start of the data structure
// delete operator to release this memory
int * numPtr (on the stack) = new int (heap memory)

example: main.cpp
#include <iostream>

int main() {
  int *numPtr = new int;

  std::cout << "*numPtr: " << *numPtr << std::endl;    // right now we have not put any memory into the heap -> random integer
  std::cout << " numPtr: " <<  numPtr << std::endl;    // content of numPtr, which is the address of heap memory -> someone low number
  std::cout << "&numPtr: " << &numPtr << std::endl;    // address of numPtr, which is a very large address since it is on stack

  *numPtr = 42;                                        // dereference value of numPtr
  std::cout << "*numPtr assigned 42." << std::endl;    // heap memory now has a value of 42

  std::cout << "*numPtr: " << *numPtr << std::endl;    // 42
  std::cout << " numPtr: " <<  numPtr << std::endl;    // address of heap memory does not change
  std::cout << "&numPtr: " << &numPtr << std::endl;    // address of numPtr does not change

  return 0;
}

example: heap1.cpp - release heap memory
#include "Cube.h"
using uiuc::Cube;

int main() {
  int *p = new int;             // pointer p on stack points to a new int on heap 
  Cube *c = new Cube;           // pointer c on stack points to a new cube on heap

  *p = 42;                      // dereference of p = 42
  (*c).setLength(4);            // dereference of c has a length of 4

  delete c;  c = nullptr;       // delete heap memory, and make the pointer points to address 0
  delete p;  p = nullptr;       // delete heap memory, and make the pointer points to null
  return 0;
}

example: heap2.cpp              // The arrow operator -> accesses the right operand member in the class at the memory address of its left operand.
int main() {
  Cube *c1 = new Cube;          // c1 points to new cube on heap
  Cube *c2 = c1;                // c2 points to c1, which points to new cube on heap as well 

  c2->setLength( 10 );          // = (*c).setLength(10);  

  delete c2;                    // delete heap memory 
  delete c1;  // !!             // double free memory that does not exist!

  return 0;
}


/* References */
int main() {
  int *x = new int;             // pointer x on stack points to new int on heap 
  int &y = *x;                  // &y: reference varialbe; y is going to alias the dereference value of x -> that heap memory address is called y
  y = 4;                        // new int = 4

// week2 quiz question 8
  int *i = new int;
  *i = 0;
  int &j = *i;  // j is the name of the heap memory that i points to


  std::cout << "*i: " << *i << std::endl;
  std::cout << "&i: " << &i << std::endl;
  std::cout << "j: " << j << std::endl;
  std::cout << "&j: " << &j << std::endl;

  j++; //// Incrementing j will increment both i and j

  std::cout << "*i: " << *i << std::endl;
  std::cout << "&i: " << &i << std::endl;
  std::cout << "j: " << j << std::endl;
  std::cout << "&j: " << &j << std::endl;


