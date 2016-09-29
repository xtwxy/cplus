#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

void print_bit(bool b) {
  cout << b;
}

void print_bits(vector<bool> bits) {
  cout << "bits.size() = " << bits.size() << endl;
  cout << "sizeof(bits) = " << sizeof(bits) << endl;

  for_each(bits.begin(), bits.end(), print_bit);
  cout << endl;
}

int main(int argc, char* argv[]) {

  vector<bool> bits;
  bits.push_back(true);
  bits.push_back(false);
  bits.push_back(false);
  bits.push_back(true);
  bits.push_back(true);

  print_bits(bits);

  return 0;
}

