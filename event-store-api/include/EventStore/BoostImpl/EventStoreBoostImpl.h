#ifndef _EVENT_STORE_BOOST_IMPL_H
#define _EVENT_STORE_BOOST_IMPL_H

#include <list>
#include <map>
#include <boost/asio.hpp>
#include <EventStore/EventStore.h>

namespace EventStore {
namespace BoostImpl {

class QueueImpl : public ::EventStore::Queue {
 public:
  QueueImpl(boost::asio::io_service&);
  virtual ~QueueImpl();

  void send(const EventPtr e);
  void post(const EventPtr e);
  void add(EventHandlerPtr handler);
  void remove(EventHandlerPtr handler);

  void terminate();
private:
  boost::asio::io_service& ios_;
  std::list<EventHandlerPtr> handlers_;
};

class EventStoreImpl : public ::EventStore::EventStore {
public:
  EventStoreImpl(boost::asio::io_service&);
  virtual ~EventStoreImpl();

  void bind(std::string qname, EventHandlerPtr handler);
  void bind(std::string qname, QueuePtr q);
  QueuePtr lookupQueue(std::string qname);
  
  void terminate();
private:
  boost::asio::io_service& ios_;
  std::map<std::string, QueuePtr> queues_;
};

} // namespace BoostImpl
} // namespace EventStore
#endif //_EVENT_STORE_BOOST_IMPL_H
