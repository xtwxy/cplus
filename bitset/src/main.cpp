#include <iostream>
#include <bitset>

using namespace std;

int main(int argc, char* argv[]) {

  bitset<5> five_bits;
  five_bits[0] = 1;
  five_bits[1] = 0;
  five_bits[2] = 0;
  five_bits[3] = 1;
  five_bits[4] = 1;
	
  bitset<3> three_bits;
  three_bits[0] = 1;
  three_bits[1] = 0;
  three_bits[2] = 0;

  cout << five_bits.size() << endl;
  /*
   * bitset<five_bits.size() + three_bits> all_bits;
  cout << all_bits << endl;
  cout << all_bits.flip() << endl;
  cout << all_bits.to_string() << endl;
  cout << all_bits.reset(2) << endl;
  cout << all_bits.reset() << endl;
*/

  return 0;
}

