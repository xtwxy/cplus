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
class Repository;
class Reference;

typedef boost::shared_ptr<EventHandler> EventHandlerPtr;
typedef boost::shared_ptr<Queue> QueuePtr;
typedef boost::shared_ptr<EventStore> EventStorePtr;
typedef boost::shared_ptr<Repository> RepositoryPtr;

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

struct Reference {
public:
  std::string name;
  std::string type;
};

class EventHandler :
    public boost::enable_shared_from_this<EventHandler>,
    private boost::noncopyable
{
 public:
  typedef std::map<std::string, std::string> InitParams;

  EventHandler();
  virtual ~EventHandler();

  virtual void on(const Event&) = 0;

  QueuePtr self() const;
  QueuePtr bind() const;
  void bind(QueuePtr);
  virtual std::vector<Reference> references() const = 0;
  virtual void reference(std::string name, QueuePtr) = 0;
  virtual void reference(std::string name, RepositoryPtr) = 0;

 private:
  QueuePtr bind_;
};

class EventStore :
    public boost::enable_shared_from_this<EventStore>,
    private boost::noncopyable
{
public:
  EventStore();
  virtual ~EventStore();

  virtual void bindHandler(std::string qname, EventHandlerPtr handler) = 0;
  virtual void registerQueue(std::string qname, QueuePtr q) = 0;
  virtual QueuePtr lookupQueue(std::string qname) = 0;
};

class Repository :
    public boost::enable_shared_from_this<Repository>,
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
