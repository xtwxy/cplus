#include <iostream>
#include <string>

using namespace std;

template<typename T>
struct MyClass {
  typedef double Y;
  typedef T Type;
};

template<>
struct MyClass<int> {
  static const int Y = 314;
  typedef int Type;
};

int O = 8;

template<typename T>
void SomeFunc() {
  typename MyClass<T>::Y * O;
}

template<>
void SomeFunc<int>() {
  MyClass<int>::Y * O;
}

int main(int argc, char* argv[]) {
	SomeFunc<int>();
	SomeFunc<string>();
	SomeFunc<double>();
	
  return 0;
}

