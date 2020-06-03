/* note for array folder */

/* array */
// limiatations:
// - store same type value;
// - must store data sequentially;
// - fixed capacity; resize if reaching the capacity
// std::vector has the same characteristics; dynamically expand size !!


// an array of cube 
sizeof(Cube) -> each Cube is 8 bytes large
// what is the offset to index 3?
3 * 8 = 24 Bytes

// ex2
#include <iostream>

int main() {
  // Create an array of 10 primes:
  int values[10] = { 2, 3, 5, 7, 11, 13, 15, 17, 21, 23 };
  
  // how much memory does it take to store an integer?
  // Print the size of each type `int`: either 4 or 8 bytes
  std::cout << sizeof(int) << std::endl;

  // Using pointer arithmetic, ask the computer to calculate
  // the offset from the beginning of the array to [2]:
  // how much memory is between array[2] and array[0]:
  // 4*2 = 8bytes
  // memory address where values[2] stored: &(values[2])
  int offset = (long)&(values[2]) - (long)&(values[0]);
  std::cout << offset << std::endl;

  return 0;
}

// ex3
int offset = (long)&(cubes[2]) - (long)&(cubes[0]); -> 8*2 = 16 Bytes

// ex4 std::vector dynamically expand size
using uiuc::Cube;

int main() {
  std::vector<Cube> cubes{ Cube(11), Cube(42), Cube(400) };

  // Examine capacity:  
  std::cout << "Initial Capacity: " << cubes.capacity() << std::endl;
  cubes.push_back( Cube(800) );
  std::cout << "Size after adding: " << cubes.size() << std::endl;
  std::cout << "Capacity after adding: " << cubes.capacity() << std::endl;
  // "Capacity after adding: " ---------- > 6

  // Using pointer arithmetic, ask the computer to calculate
  // the offset from the beginning of the array to [2]:
  int offset = (long)&(cubes[2]) - (long)&(cubes[0]);
  std::cout << "Memory separation: " << offset << std::endl;

  // Find a specific `target` cube in the array:
  Cube target = Cube(400);
  for (unsigned i = 0; i < cubes.size(); i++) {
    if (target == cubes[i]) {
      std::cout << "Found target at [" << i << "]" << std::endl;
    }
  }

  return 0;
}


