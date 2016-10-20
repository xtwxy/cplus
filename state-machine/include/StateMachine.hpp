#ifdef _STATE_MACHINE_HPP_
#define _STATE_MACHINE_HPP_

#include <vector>

#include <boost/shared_ptr.hpp>
#include <boost/noncopyable.hpp>

class Queue;
class Repository;

typedef std:vector<char> Bytes;

class Decoder : private boost::noncopyable {
 public:
  Decoder() { }
  virtual ~Decoder() { }

  virtual void on(const Bytes&) = 0;
  virtual void onClose() = 0;
  virtual void onTimeout() = 0;
};

class Queue private boost::noncopyable {
 public:
  Queue() { }
  virtual ~Queue() { }

  virtual void enqueue(const Bytes&) = 0;
  virtual void close() = 0;
  virtual void timeout() = 0;
};

class Repository {
 public:
  virtual ~Repository() { }
};

class State {
 public:
  State() { }
  virtual ~State() { }

  virtual void accept(EventPtr) = 0;
  virtual State& transition() = 0;
  virtual bool guard() = 0;
};


#endif //_STATE_MACHINE_HPP_
