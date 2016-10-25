#ifndef _ACTOR_SYSTEM_BOOST_IMPL_H
#define _ACTOR_SYSTEM_BOOST_IMPL_H

#include <list>
#include <map>
#include <boost/asio.hpp>
#include <ActorSystem/ActorSystem.h>

namespace ActorSystem {
namespace BoostImpl {

class QueueImpl : public ::ActorSystem::Queue {
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

class ActorSystemImpl : public ::ActorSystem::ActorSystem {
public:
  ActorSystemImpl(boost::asio::io_service&);
  virtual ~ActorSystemImpl();

  void bind(std::string qname, EventHandlerPtr handler);
  void bind(std::string qname, QueuePtr q);
  QueuePtr lookupQueue(std::string qname);
  
  void terminate();
private:
  boost::asio::io_service& ios_;
  std::map<std::string, QueuePtr> queues_;
};

} // namespace BoostImpl
} // namespace ActorSystem
#endif //_ACTOR_SYSTEM_BOOST_IMPL_H
