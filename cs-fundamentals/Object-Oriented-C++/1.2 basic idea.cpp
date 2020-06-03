/* The variables which are declared in any class by using 
any fundamental data types (like int, char, float etc) or 
derived data type (like class, structure, pointer etc.) 
are known as Data Members. And the functions which are 
declared either in private section of public section are 
known as Member functions. */

/* header file: call functions */
cube.h
#define functions that you are going to use
#pragma once
class Cube {   ...};
public functions...
private functions...

/* implementation file: define functions */
cube.cpp
#define function details
#include header file (“Cube.h”)
double Cube::getVolume() {…}

/* main file: use functions */
main.cpp
# create a cube im memory, set one side of this cude to 3.48, call
# getVolume function to get the volume;
#include header file (“Cube.h”)
int main() {
    Cube c; # create a new Cube

    c.setlength(3.48);
    double volume = c.getVolume();
    std::cout << "Volume: " << volume << std::ebdl;

    return 0;
}








