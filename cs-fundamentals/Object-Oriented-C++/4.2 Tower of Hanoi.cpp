/* note for cpp-tower */
// game - stack - cube
// game: move all cubes on stack[0] to the last stack, and during
// the process, only small cube can be put on top of the large cube

// uiuc cube.h for class cube
// cube has length and color 
#pragma once

#include "HSLAPixel.h"

namespace uiuc {
  class Cube {
    public:
      Cube(double length, HSLAPixel color);

      double getLength() const;
      void setLength(double length);

      double getVolume() const;
      double getSurfaceArea() const;

    private:
      double length_;
      HSLAPixel color_; // store color into a new private variable
  };
}

// new class must be created to represent each of the stacks in the Tower of Hanoi
// stack.h
// every single stack needs to contain:
// a. a vector of cubes; 
// b. operations to interact with the top of the stack
   // - push_back: to add element on the top
   // - removeTop() -> return a cube by value (copy)
   // - peekTop()   -> return the memory without removing it
   // - size()

#pragma once

#include <vector>
#include "uiuc/Cube.h"
using uiuc::Cube;

class Stack {
  public:
    void push_back(const Cube & cube);
    Cube removeTop();
    Cube & peekTop();
    unsigned size() const;

    // An overloaded operator<<, allowing us to print the stack via `cout<<`:
    // an ostream as the first argument, and a constant reference of its own type as the second parameter
    // same as tostring 
    friend std::ostream& operator<<(std::ostream & os, const Stack & stack);

  private:
    std::vector<Cube> cubes_;
};

// build the game class
// game.h 
// game needs to contain:
// a. an array of three stacks
// b. the initial state has four cubes in the first stack

#pragma once

#include "Stack.h" // for private member variable
#include <vector> 

class Game {
  public:
    Game();  // set up the initial state of the class
    void solve(); // solve the game programatically 

    // An overloaded operator<<, allowing us to print the stack via `cout<<`:
    // print out game as an object
    friend std::ostream& operator<<(std::ostream & os, const Game & game);

  private:
    std::vector<Stack> stacks_;
};

// game.cpp;
// Create the an array of three empty stacks
// Create the four cubes, and push all cubes to the stack[0] -> initial state
#include "Game.h"
#include "Stack.h"
#include "uiuc/Cube.h"
#include "uiuc/HSLAPixel.h"

#include <iostream>
using std::cout;
using std::endl;

// Solves the Tower of Hanoi puzzle.
// (Feel free to call "helper functions" to help you solve the puzzle.)
void Game::solve() {
  // Prints out the state of the game:
  cout << *this << endl;

  // @TODO -- Finish solving the game!
}

// Default constructor to create the initial state:
Game::Game() {
  // Create the three empty stacks:
  for (int i = 0; i < 3; i++) {
    Stack stackOfCubes;
    // this stack is going to be added in the back of the stacks array
    // an array of three stacks
    stacks_.push_back( stackOfCubes );
  }

  // Create the four cubes, placing each on the [0]th stack:
  // push all cubes to stack[0]
  Cube blue(4, uiuc::HSLAPixel::BLUE);
  stacks_[0].push_back(blue);

  Cube orange(3, uiuc::HSLAPixel::ORANGE);
  stacks_[0].push_back(orange);

  Cube purple(2, uiuc::HSLAPixel::PURPLE);
  stacks_[0].push_back(purple);

  Cube yellow(1, uiuc::HSLAPixel::YELLOW);
  stacks_[0].push_back(yellow);
}

std::ostream& operator<<(std::ostream & os, const Game & game) {
  for (unsigned i = 0; i < game.stacks_.size(); i++) {
    os << "Stack[" << i << "]: " << game.stacks_[i];
  }
  return os;
}

// main.cpp -> run the game
#include "Game.h"
#include <iostream>

int main() {
  Game g;        // create a game called g

  std::cout << "Initial game state: " << std::endl;
  std::cout << g << std::endl;   // works since friend ostream operator

  g.solve();     // solve the game

  std::cout << "Final game state: " << std::endl;
  std::cout << g << std::endl;

  return 0;
}



