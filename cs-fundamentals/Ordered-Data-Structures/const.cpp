Notes about "const":

// Marking member functions as "const" at the end of their prototype means
// they can not change the class instance. This is useful to keep yourself
// from making a mistake, but also required when a class instance is passed
// as a reference to a const type in a function. In that case, the class
// instance is basically marked "read only", and you can only use its member
// functions that are const.

// When using a pointer to iterate over const objects, you might use a syntax
// like this:
//
// const Node* thisCur = head_;
//
// The type here is pointer to const Node, so the pointer may change to
// point to a different Node, but the actual Nodes that it points to may
// not be edited through it.

// It's important for you to think about what can or should be marked "const"
// and do so as soon as possible as you design. This is called "const
// correctness". https://isocpp.org/wiki/faq/const-correctness

