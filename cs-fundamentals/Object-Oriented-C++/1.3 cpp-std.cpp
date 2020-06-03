/* notes for ccp-std folder */

iostream: cout 

example:
#include <iostream>

int main() {
    std::cout << "Hello world!" << std::endl;
    return 0;
}


# using std:: cout to avoid std next time
example:
#include <iostream>

using std::cout;
using std::endl;

int main() {
    cout << "Hello world!" << endl;
    return 0;
}


#namespace to make class unique
namespace uiuc {
  class Cube {
    public:
      double getVolume();
      double getSurfaceArea();
      void setLength(double length);

    private:
      double length_;
  };
}