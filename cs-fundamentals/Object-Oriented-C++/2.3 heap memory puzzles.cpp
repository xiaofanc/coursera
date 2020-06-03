/* note for cpp-heapPuzzles folder */

&: ampersand

puzzle1: stack memory only
#include <iostream>     # preprocessor directives

using std::cout;
using std::endl;

int main() {
  int  i =  2,  j =  4,  k =  8;
  int *p = &i, *q = &j, *r = &k;                    // these are all on stack memory, pointers p, q, r

  k = i;                                            // k = 2
  cout << i << j << k << *p << *q << *r << endl;    // 242242

  p = q;                                            // p takes on the value of q, q points to j, so p = 4
  cout << i << j << k << *p << *q << *r << endl;    // 242442

  *q = *r;                                          // dereference value of q takes on the dereference value of r, so j changes to 2
  cout << i << j << k << *p << *q << *r << endl;    // 222222, since p, q points to j, so p = q = 2

  return 0;
}

puzzle2: stack memory and heap memory
#include <iostream>

using std::cout;
using std::endl;

int main() {
  int *x = new int;             // pointer x on stack points to new int on heap 
  int &y = *x;                  // &y: reference varialbe; y is going to alias the dereference value of x -> that heap memory address is called y
  y = 4;                        // new int = 4

  cout << &x << endl;           // address of x on stack  -> large value
  cout << x << endl;            // content of x           -> fairly small number since its value is a pointer to heap memory
  cout << *x << endl;           // dereference value of x -> new int = 4

  cout << &y << endl;           // address of y = memory address on heap -> x
  cout << y << endl;            // value of y = 4
  // cout << *y << endl;        // dereference value of a non pointer, // represents comment out

  return 0;
}

puzzle3: stack memory and heap memory
#include <iostream>

using std::cout;
using std::endl;

int main() {
  int *p, *q;                        // pointers p and q
  p = new int;                       // p points to new int on heap
  q = p;                             // q points to p, so q points to new int on heap as well
  *q = 8;                            // dereference value of q is 8   -> new int = 8
  cout << *p << endl;                // dereference value of p        -> 8

  q = new int;                       // q not longer points to the memory pointed before, instead it points to a new int_2 on heap
  *q = 9;                            // dereference value of q = 9    -> new int_2 = 9
  cout << *p << endl;                // dereference value of p        -> new_int = 8
  cout << *q << endl;                // dereference value of q        -> 9

  return 0;
}

puzzle4: arrays
#include <iostream>

using std::cout;
using std::endl;

int main() {
  int *x;                            // pointer x on stack
  int size = 3;                      // size 3 on stack
  x = new int[size];                 // x = new integer of size 3      -> an array of memory

  for (int i = 0; i < size; i++) {
    x[i] = i + 3;
  }

  delete[] x;                        // delete brackets
}



