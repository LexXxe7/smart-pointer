# Smart Pointer

This repo creates a new pointer type that provides built-in support for garbage collection based on reference counting.

In reference counting, each dynamically allocated piece of memory has associated with it a reference count. This count is incremented each time a reference to the memory is added and decremented each time a reference to the memory is removed. When the reference count drops to zero, the memory is unused and can be released.

## Overview of Code Structure

* At the core of the garbage collector is the template class "smart_ptr", which maintains a list that associates a reference count with each piece of memory allocated for use by a smart_ptr.
* Each entry in this list is encapsulated in an object of type "details" storing the reference count and a pointer to the memory.
* Finally, "iter" is a template class that defines all pointer operations, including pointer arithmetic, and enables us to cycle through the elements of a dynamically allocated array.

## Dependencies for Running Locally

* cmake >= 3.7
  * All OSes: [click here for installation instructions](https://cmake.org/install/)
* make >= 4.1 (Linux, Mac), 3.81 (Windows)
  * Linux: make is installed by default on most Linux distros
  * Mac: [install Xcode command line tools](https://developer.apple.com/xcode/features/)
  * Windows: [click here for installation instructions](http://gnuwin32.sourceforge.net/packages/make.htm)
* gcc/g++ >= 5.4
  * Linux: gcc/g++ is installed by default on most Linux distros
  * Mac: same deal as make - [install Xcode command line tools](https://developer.apple.com/xcode/features/)
  * Windows: recommend using [MinGW](http://www.mingw.org/)

## Basic Build Instructions

1. Clone this repo.
2. Make a build directory in the top level directory: `mkdir build && cd build`
3. Compile: `cmake .. && make`
4. Run it: `./SmartPointer`
