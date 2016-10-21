#ifndef _STATE_MACHINE_CODEC_H_
#define _STATE_MACHINE_CODEC_H_

#include <vector>
#include <list>

#include <boost/shared_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <boost/noncopyable.hpp>
#include <boost/any.hpp>

namespace stm {

class EventHandler;
class Queue;

typedef boost::shared_ptr<EventHandler> EventHandlerPtr;
typedef boost::shared_ptr<Queue> QueuePtr;

class Event {
 public:
  Event(QueuePtr source);
  virtual ~Event();

  QueuePtr source() const;
 private:
  QueuePtr source_;
};

class Queue :
    public boost::enable_shared_from_this<Queue>,
    private boost::noncopyable
{
 public:
  Queue();
  virtual ~Queue();

  virtual void enqueue(const Event& e) = 0;
  virtual void add(EventHandlerPtr handler) = 0;
};

class EventHandler :
    public boost::enable_shared_from_this<EventHandler>,
    private boost::noncopyable
{
 public:
  EventHandler();
  virtual ~EventHandler();

  virtual void init();
  virtual void on(const Event&) = 0;

  QueuePtr source() const;
  void source(QueuePtr) const;

 private:
  QueuePtr source_;
};

class EventStore :
    public boost::enable_shared_from_this<EventStore>,
    private boost::noncopyable
{
public:
  EventStore();
  virtual ~EventStore();

  virtual void register(std::string qname, EventHandlerPtr handler) = 0;
  virtual QueuePtr lookup(std::string qname) = 0;
};

class State {
 public:
  State();
  virtual ~State();

  virtual void accept(Event&) = 0;
  virtual State& transition() = 0;
  virtual bool guard() = 0;
};

} // namespace stm

#endif //_STATE_MACHINE_CODEC_H_
