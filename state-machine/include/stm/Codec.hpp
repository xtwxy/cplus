#ifndef _STATE_MACHINE_CODEC_HPP_
#define _STATE_MACHINE_CODEC_HPP_

#include <vector>
#include <vector>

#include <boost/shared_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <boost/noncopyable.hpp>
#include <boost/any.hpp>

namespace stm {

class EventHandler;
class Queue;
class Event;

typedef boost::shared_ptr<EventHandler> EventHandlerPtr;
typedef boost::shared_ptr<Queue> QueuePtr;

class EventHandler {
 public:
  EventHandler() : source_(new Queue()) { }
  virtual ~EventHandler() { }

  virtual void on(const Event&) = 0;
  
  QueuePtr source() const {
    return source_;
  }

 private:
  QueuePtr source_;
};

class Queue : private boost::enable_shared_from_this, 
    private boost::noncopyable {
 public:
  virtual ~Queue() : handlers_() { }

  void enqueue(const Event& e) {
    for(std::list<EventHandlerPtr>::iterator it = handlers_.begin();
        it != handlers_.end(); ++it) {
      it->on(e);
    }
  }
  
  void add(EventHandlerPtr handler) {
    handlers_.push_back(handler);
  }

 private:
  std::list<EventHandlerPtr> handlers_;
};

class Event {
 public:
  Event(QueuePtr source) : source_(source) { }
  virtual ~Event() { }

  QueuePtr source() const {
    return source_;
  }

 private:
  QueuePtr source_;
};

class State {
 public:
  virtual ~State() { }

  virtual void accept(Event&) = 0;
  virtual State& transition() = 0;
  virtual bool guard() = 0;
};

} // namespace stm

#endif //_STATE_MACHINE_CODEC_HPP_
