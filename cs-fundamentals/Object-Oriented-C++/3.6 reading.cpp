/* a.segfault: */
// This code compiles successfully, runs, and CRASHES with a segfault!
int* n = nullptr;
std::cout << *n << std::endl;

/* Undefined behavior */
// Initialization
// h is uninitialized!
int h;
// b will be default initialized since Box is a class
Box b;

int* r = new int(0); // intialize new int as 0

/* Resetting deleted pointers to nullptr */
// Allocate an integer on the heap:
int* x = new int;
// Now x holds some memory address to a valid integer.
// Do some kind of work with the integer.
// We'll just set that integer to 7:
*x = 7;
// Now delete the pointer to deallocate the heap memory:
delete x;
// This destroys the integer on the heap and frees the memory.
// But now x still holds the memory address!
// Set x to nullptr for safety:
x = nullptr;

/* unsigned int */
// v.size() - unsigned int size, no negative value 
// you are comparing a signed int (perhaps an iteration counter) with an unsigned integer size. 
std::vector<int> v = {1,2,3,4};
for (int i=0; i < v.size(); i++) {
    std::cout << v[i] << std::endl;
}

// solution:
// Casting to signed int first helps to ensure that the result
// of subtraction will truly be a signed negative value when size is 0:
for (int i=0; i <= (int)v.size()-1; i++) {
    // ...
}

// Rewriting the algebra to perform addition instead of subtraction
// helps to avoid going below 0:
for (int i=0; i+1 <= v.size(); i++) {
    // ...
}



