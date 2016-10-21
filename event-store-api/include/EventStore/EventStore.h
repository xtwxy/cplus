#ifndef _EVENT_STORE_API_H_
#define _EVENT_STORE_API_H_

#include <string>
#include <map>

#include <boost/shared_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <boost/noncopyable.hpp>
#include <boost/any.hpp>

namespace EventStore {

class EventHandler;
class Queue;
class EventStore;
class StateRepository;

typedef boost::shared_ptr<EventHandler> EventHandlerPtr;
typedef boost::shared_ptr<Queue> QueuePtr;
typedef boost::shared_ptr<EventStore> EventStorePtr;
typedef boost::shared_ptr<StateRepository> StateRepositoryPtr;

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
  typedef std::map<std::string, std::string> InitParams;

  EventHandler();
  virtual ~EventHandler();

  virtual void init(EventStorePtr, StateStorePtr, const InitParams&);
  virtual void on(const Event&) = 0;

  QueuePtr source() const;
  void source(QueuePtr);

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

  virtual void registerHandler(std::string qname, EventHandlerPtr handler) = 0;
  virtual void registerQueue(std::string qname, QueuePtr q) = 0;
  virtual QueuePtr lookupQueue(std::string qname) = 0;
};

class StateRepository :
    public boost::enable_shared_from_this<StateRepository>,
    private boost::noncopyable
{
public:
  StateRepository();
  virtual ~StateRepository();

  virtual void registerQueue(std::string qname, QueuePtr q) = 0;
  virtual QueuePtr lookupQueue(std::string qname) = 0;
};

} // namespace stm

#endif //_EVENT_STORE_API_H_
