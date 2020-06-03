
/* if-else */
#include <iostream>

int main() {
  int x = 2; // This line assigns a value.
  int y = 5;
  
  if (x == y) {
    // Note that we had to use "==" to check equality!
    // That's two equal signs!
    std::cout << "x and y are equal" << std::endl;
  }
  else {
    std::cout << "x and y are not equal" << std::endl;
  }
  
  return 0;
}

/* ternary operator */
// Since (5<10) is true, the expression before the colon will be selected, which is 1.
int x = 5 < 10 ? 1 : 2;
// Now x is equal to 1.

// Note, the following syntax is NOT allowed in C++, which is why the ternary operator can be useful in these cases:
int y = if (5<10) {1;} else {2;}

/* Type Casting */
#include <iostream>
int main() {
  int x = 2;
  double y = 3.5;
  std::cout << "This result will be a double with value 5.5: " << (x+y) << std::endl;
  
  int z = x+y; // This expression is calculated as a double, but then it's cast back to int!
  std::cout << "This result will be an int with value 5: " << z << std::endl;
  return 0;
}

//nonzero numeric values will be considered true, and zero will be considered false.
#include <iostream>
int main() {
  if (0) {
    std::cout << "You won't see this text." << std::endl;
  }
  if (-1) {
    std::cout << "You WILL see this text!" << std::endl;
  }
  return 0;
}

/* block scope */
#include <iostream>
int main() {
  
  // You can actually make an inner scope block anywhere in a function.
  // Let's do it to show how scope works.
  
  // In the initial, outer scope:
  int x = 2;
  std::cout << "Outer scope value of x (should be 2): " << x << std::endl;
  
  // Create an inner scope and make a new variable with the name "x".
  // This is not an error! We can redeclare x because of the inner scope.
  // Also, make an extra variable named "y".
  {
    int x = 3;
    int y = 4;
    std::cout << "Inner scope vaue of x (should be 3): " << x << std::endl;
    std::cout << "Inner scope vaue of y (should be 4): " << y << std::endl;
  }
  
  // Now that the inner block has closed, the inner x and y are gone.
  // The original x variable is still on the stack, and it has its old value:
  std::cout << "Outer scope value of x (should be 2): " << x << std::endl;
  
  // We can't refer to y here, because it doesn't exist in this scope at all!
  // If you un-comment this line, there will be a compile error.
  // std::cout << "This line causes an error because y doesn't exist: " << y << std::endl;
  
  return 0;
}

/* loops */
#include <iostream>
int main() {
  
  // outer scope version of "x" to show the for-loop block scoping
  int x = -1;
  
  std::cout << "[Outside the loop] x is now (should be -1): " << x << std::endl;
  
  // The for loop lets us declare a variable in the first part of the
  // loop statement, which will belong to the inner block scope:
  for (int x = 0; x <= 2; x++) {
    std::cout << "[Inside the loop] x is now: " << x << std::endl;
  }
  
  // The outer scope x is still -1
  std::cout << "[Outside the loop] x is now (should be -1): " << x << std::endl;
  
  // This version doesn't redeclare x, so it just inherits access to the
  // same x variable from the outer scope. This modifies the outer x directly!
  for (x = 0; x <= 2; x++) {
    std::cout << "[Inside the loop] x is now: " << x << std::endl;
  }
  
  // In the last iteration where the condition x<=2 was true, x had the value 2.
  // After that iteration, x was incremented one more time and became 3.
  // Then the condition was false and the loop body did not execute.
  // Afterwards, the outer scope x is still 3 because the loop modified it.
  std::cout << "[Outside the loop] x is now (should be 3): " << x << std::endl;
  
  return 0;
}

/* while */
#include <iostream>
int main() {
  int x = 0;
  std::cout << "This should show 0, 1, 2, 3:" << std::endl;
  while (x <= 3) {
    std::cout << "x is now: " << x << std::endl;
    x++; // increment x by 1
  }
  return 0;
}





