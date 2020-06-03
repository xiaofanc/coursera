/* note for cpp-tower-solution */
/* only move the top of the stack, and moves have a pattern */

// game.cpp
// how to move
void Game::_move(unsigned index1, unsigned index2) {
  Cube cube = stacks_[index1].removeTop(); // get the top cube of stack[index1]
  stacks_[index2].push_back(cube);         // add the cube on the top of stack[index2]
}

// function to judge if the move is _legalMove
void Game::_legalMove(unsigned index1, unsigned index2) {
  // stacks_[index1] is empty
  if (stacks_[index1].size() == 0 && stacks_[index2].size() > 0) {
    _move(index2, index1);
  } 
  // stacks_[index2] is empty
  else if (stacks_[index1].size() > 0 && stacks_[index2].size() == 0) {
    _move(index1, index2);
  } 
  // compare the length of the top cube
  else if (stacks_[index1].size() > 0 && stacks_[index2].size() > 0) {
    if (stacks_[index1].peekTop().getLength() < stacks_[index2].peekTop().getLength() ) {
      _move(index1, index2); // stacks_[index1].peekTop() is small
    } else {
      _move(index2, index1); // stacks_[index2].peekTop() is small
    }
  }
  
  cout << *this << endl;
}

void Game::solve() {
  // loop until the last stack has size 4;
  // moves have a pattern; 0-1, 0-2, 1-2;
  while (stacks_[2].size() != 4) {
    _legalMove(0, 1);
    _legalMove(0, 2);
    _legalMove(1, 2);
  }
}