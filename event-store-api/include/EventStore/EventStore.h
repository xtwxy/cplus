#ifndef _EVENT_STORE_API_H_
#define _EVENT_STORE_API_H_

#include <string>
#include <vector>
#include <map>

#include <boost/shared_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <boost/noncopyable.hpp>
#include <boost/any.hpp>

namespace EventStore {

class Event;
class EventHandler;
class Queue;
class EventStore;
class Repository;
class Reference;

typedef boost::shared_ptr<Event> EventPtr;
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

  virtual void send(const EventPtr e) = 0;
  virtual void post(const EventPtr e) = 0;
  virtual void add(EventHandlerPtr handler) = 0;
  virtual void remove(EventHandlerPtr handler) = 0;
  
  virtual void terminate() = 0;
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

  virtual void on(const EventPtr) = 0;

  void name(const std::string name);
  const std::string name() const;

  QueuePtr self() const;
  QueuePtr bind() const;
  void bind(QueuePtr);
  virtual const std::vector<std::string> references() const;
  virtual void reference(std::string name, QueuePtr);

  void terminate();
 protected:
  virtual void stop() = 0;
 private:
  std::string name_;
  QueuePtr bind_;
  std::map<std::string, QueuePtr> outputs_;

};

class EventStore :
    public boost::enable_shared_from_this<EventStore>,
    private boost::noncopyable
{
public:
  EventStore();
  virtual ~EventStore();

  virtual void bind(std::string qname, EventHandlerPtr handler) = 0;
  virtual void bind(std::string qname, QueuePtr q) = 0;
  virtual QueuePtr lookupQueue(std::string qname) = 0;
  
  virtual void terminate() = 0;
};

class Repository :
    public boost::enable_shared_from_this<Repository>,
    private boost::noncopyable
{
public:
  Repository();
  virtual ~Repository();

  virtual void registerQueue(std::string qname, QueuePtr q) = 0;
  virtual QueuePtr lookupQueue(std::string qname) = 0;
};

} // namespace stm

#endif //_EVENT_STORE_API_H_
