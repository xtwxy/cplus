#ifndef _CODEC_CODEC_HPP_
#define _CODEC_CODEC_HPP_

#include <ByteStream.hpp>

typedef boost::any State;

class Encoder {
 public:
  virtual ~Encoder() { }

  virtual void encode(State& state, Message& in, DownStream& out) = 0; 
  virtual void onClose(State& state, DownStream& out) = 0;
  virtual void onTimeout(State& state, DownStream& out) = 0;
};

class Decoder {
 public:
  virtual ~Decoder() { }

  virtual void decode(State& state, Message& in, UpperStream& out) = 0; 
  virtual void onClose(State& state, UpperStream& out) = 0;
  virtual void onTimeout(State& state, UpperStream& out) = 0;
};

class MessageHandler {
 public:
  virtual ~MessageHandler() { }

  virtual void onMessage(State& state, Message& msg, UpperStream& out) = 0;
  virtual void onClose(State& state, UpperStream& out) = 0;
  virtual void onTimeout(State& state, UpperStream& out) = 0;
};

#endif //defined _CODEC_CODEC_HPP_

