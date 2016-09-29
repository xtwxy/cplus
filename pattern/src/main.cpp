#include <iostream>
#include <iomanip>
#include <vector>
#include <map>
#include <algorithm>

#include <boost/endian/buffers.hpp>

using namespace std;

enum State { MATCHED, NOT_MATCH, INSUFFICIENT_BYTES };

template<unsigned short Length>
struct Pattern {
  Pattern(
      unsigned char           addr,
      unsigned char           cmd,
      unsigned short          length,
      unsigned char*          data,
      unsigned short          crc16
      ) : 
      addr(addr),
      cmd(cmd),
      length(length),
      crc16(crc16) { 
    for(size_t i = 0; i != Length; ++i) {
      this->data[i] = data[i];
    }
  }

  State machHeader(const unsigned char *buf, size_t length) const {
    const Pattern& rhs = reinterpret_cast<const Pattern&>(*buf);
    if(length >= sizeof(addr) + sizeof(cmd)) {
      if(addr.value() != rhs.addr.value()) return NOT_MATCH; 
      if(cmd.value() != rhs.cmd.value()) return NOT_MATCH; 
      return MATCHED;
    } else {
      return INSUFFICIENT_BYTES;
    }
  }

  State match(const unsigned char *buf, size_t length) const {
    if(length < sizeof(Pattern)) {
      return INSUFFICIENT_BYTES;
    }
    const Pattern& rhs = reinterpret_cast<const Pattern&>(*buf);
    return match(rhs) ? MATCHED : NOT_MATCH;
  }

  bool match(const Pattern& rhs) const {
    if(addr.value() != rhs.addr.value()) return false; 
    if(cmd.value() != rhs.cmd.value()) return false; 
    if(length.value() != rhs.length.value()) return false;

    for(size_t i = 0; i != Length; ++i) {
      if(data[i].value() != rhs.data[i].value()) return false;
    }

    if(crc16.value() != rhs.crc16.value()) return false; 

    return true;
  }

  bool operator<(const Pattern& rhs) const {
    if(addr.value() >= rhs.addr.value()) return false; 
    if(cmd.value() >= rhs.cmd.value()) return false; 
    if(length.value() >= rhs.length.value()) return false;

    for(size_t i = 0; i != Length; ++i) {
      if(data[i].value() >= rhs.data[i].value()) return false;
    }

    if(crc16.value() >= rhs.crc16.value()) return false; 

    return true;
  }
  boost::endian::big_uint8_buf_t    addr;
  boost::endian::big_uint8_buf_t    cmd;
  boost::endian::big_uint16_buf_t   length;
  boost::endian::big_uint8_buf_t    data[Length];
  boost::endian::big_uint16_buf_t   crc16;
};

template<unsigned short Length>
void print_pattern(const Pattern<Length>& p) {
  const unsigned char * raw = reinterpret_cast<const unsigned char*>(&p);
  for(size_t i = 0; i != sizeof(p); ++i) {
    cout << hex 
        << setw(2) 
        << setfill('0') 
        //<< uppercase
        << static_cast<unsigned>(raw[i]) << ' ';
  }
  cout << endl;
}

int main(int argc, char* argv[]) {
  unsigned char data0[] = { };
  unsigned char data1[] = { 0x01, 'c' };
  unsigned char data2[] = { 'H', 'e', 'l', 'l', 'o', ',', ' ', 'W', 'o', 'r', 'l', 'd', '!' };

  Pattern<sizeof(data0)> p0(0x01, 0x03, 0x0001, data0, 0x00fe);
  Pattern<sizeof(data1)> p1(0x01, 0x03, 0x0001, data1, 0x00fe);
  Pattern<sizeof(data2)> p2(0x01, 0x03, 0x0001, data2, 0x00fe);

  print_pattern(p0);
  print_pattern(p1);
  print_pattern(p2);
 
  map<State, std::string> messages;
  messages.insert(make_pair(MATCHED, "MATCHED")); 
  messages.insert(make_pair(NOT_MATCH, "NOT_MATCH")); 
  messages.insert(make_pair(INSUFFICIENT_BYTES, "INSUFFICIENT_BYTES"));

  cout << "p1.match(p2) = " << messages.at(p1.match(reinterpret_cast<const unsigned char*>(&p2), sizeof(p2))) << endl;

  return 0;
}

