#ifndef _EVENT_STORE_BOOST_IMPL_H
#define _EVENT_STORE_BOOST_IMPL_H

#include <list>
#include <map>
#include <EventStore/EventStore.h>

namespace EventStore {
namespace BoostImpl {

class QueueImpl : public ::EventStore::Queue {
 public:
  QueueImpl();
  virtual ~QueueImpl();

  virtual void enqueue(const Event& e);
  virtual void add(EventHandlerPtr handler);
private:
  std::list<EventHandlerPtr> handlers_;
};

class EventStoreImpl : public ::EventStore::EventStore {
public:
  EventStoreImpl();
  virtual ~EventStoreImpl();

  void bindHandler(std::string qname, EventHandlerPtr handler);
  void registerQueue(std::string qname, QueuePtr q);
  QueuePtr lookupQueue(std::string qname);

private:
  std::map<std::string, QueuePtr> queues_;
};

} // namespace BoostImpl
} // namespace EventStore
#endif //_EVENT_STORE_BOOST_IMPL_H
