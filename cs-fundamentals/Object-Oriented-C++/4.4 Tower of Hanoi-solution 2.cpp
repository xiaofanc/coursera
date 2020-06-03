/* note for cpp-tower-solution */
/* take off everything but the bottom layer, move it to a spare spot,
then move then bottom layer..
we cannot move a series of cubes together, so we need to repeat the process
*/
// moves planned: move(source[start...end] -> target) [0-3]
// a. move(source[(start+1)...end] -> spare) -> ...   [1-3]
// b. move(source[start] -> target) -> ...            [0]
// c. move(spare[(start+1)...end] -> target) -> ...   [1-3]

// game.cpp
// Move a Cube from Stack `s1` to Stack `s2`:
void Game::_moveCube(Stack & s1, Stack & s2) {
  // move top cube of s1 to s2
  Cube cube = s1.removeTop();
  s2.push_back(cube);
}

// Move the cubes in the range [start...end] from `source` to `target`, using spare as a spare spot:
void Game::_move(
  unsigned start, unsigned end,
  Stack & source, Stack & target, Stack & spare, // a reference to source, spare, and target stack
  unsigned depth     // how deep we are in the recursion
) {
  cout << "Planning (depth=" << depth++ << "): Move [" << start << ".." << end << "] from Stack@" << &source << " -> Stack@" << &target << ", Spare@" << &spare << "]" << endl;

  // Check if we are only moving one cube !!!!:
  if (start == end) {      
    // If so, move it directly:
    _moveCube( source, target );   // from source to target
    cout << *this << endl;
  } else {
    // Otherwise, use our move strategy:
    // recursion, remember to swap spare and target stack
    // swap the spare and the target stack

    // put [start+1, end] from source to spare
    _move(start + 1, end  , source, spare , target, depth); 
    // put [start] from source to target
    _move(start    , start, source, target, spare , depth);
    // put[start+1, end] from spare to target
    _move(start + 1, end  , spare , target, source, depth);
  }
}

// initialization
void Game::solve() {
  _move(
    0, stacks_[0].size() - 1,  //< Move the entire set of cubes, [0 .. size-1]
    stacks_[0], //< Source stack is [0]
    stacks_[2], //< Target stack is [2]
    stacks_[1], //< Spare stack is [1]
    0   //< Initial depth (for printouts only) is 0
  );
}