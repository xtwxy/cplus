#ifndef _STATE_MACHINE_CODEC_HPP_
#define _STATE_MACHINE_CODEC_HPP_

#include <vector>

#include <boost/shared_ptr.hpp>

namespace stm {

class Decoder;
class Queue;

typedef boost::shared_ptr<Decoder> DecoderPtr;
typedef boost::shared_ptr<Queue> QueuePtr;
typedef std::vector<char> Bytes;

class Decoder {
 public:
  virtual ~Decoder() { }

  virtual void on(const Bytes&) = 0;
  virtual void onClose() = 0;
  virtual void onTimeout() = 0;
};

class Queue {
 public:
  virtual ~Queue() { }

  virtual void enqueue(const Bytes&) = 0;
  virtual void close() = 0;
  virtual void timeout() = 0;
};

class State {
 public:
  virtual ~State() { }

  virtual void accept(Event) = 0;
  virtual void on(const Bytes&) = 0;
  virtual void onClose() = 0;
  virtual void onTimeout() = 0;
  
  virtual State& transition() = 0;
  virtual bool guard() = 0;
} // namespace stm

#endif //_STATE_MACHINE_CODEC_HPP_
