#ifndef _CODEC_CODEC_HPP_
#define _CODEC_CODEC_HPP_

#include <ByteStream.hpp>

template<typename State, typename InMessage, typename OutMessage>
class Encoder {
 public:
  virtual ~Encoder() { }

  virtual void encode(State&, InMessage&, DownStream<OutMessage>&) = 0; 
  virtual void onClose(State&, DownStream<OutMessage>&) = 0;
  virtual void onTimeout(State&, DownStream<OutMessage>&) = 0;
};

template<typename State, typename InMessage, typename OutMessage>
class Decoder {
 public:
  virtual ~Decoder() { }

  virtual void decode(State&, InMessage&, UpperStream<OutMessage>&) = 0; 
  virtual void onClose(State&, UpperStream<OutMessage>&) = 0;
  virtual void onTimeout(State&, UpperStream<OutMessage>&) = 0;
};

template<typename State, typename Message>
class MessageHandler {
 public:
  virtual ~MessageHandler() { }

  virtual void onMessage(State&, Message&) = 0;
  virtual void onClose(State&, UpperStream<Message>&) = 0;
  virtual void onTimeout(State&, UpperStream<Message>&) = 0;
};

#endif //defined _CODEC_CODEC_HPP_

