#include <boost/make_shared.hpp>
#include <boost/bind.hpp>
#include <EventStore/BoostImpl/EventStoreBoostImpl.h>

namespace EventStore {
namespace BoostImpl {

QueueImpl::QueueImpl(boost::asio::io_service& ios) : ios_(ios), handlers_() { }

QueueImpl::~QueueImpl() { }

void QueueImpl::send(const EventPtr e) {
  for(std::list<EventHandlerPtr>::iterator it = handlers_.begin();
      it != handlers_.end(); ++it) {
    (*it)->on(e);
  }
}

void QueueImpl::post(const EventPtr e) {
  for(std::list<EventHandlerPtr>::iterator it = handlers_.begin();
      it != handlers_.end(); ++it) {
    ios_.post(boost::bind(&EventHandler::on, (*it), e));
  }
}

void QueueImpl::add(EventHandlerPtr handler) {
  handlers_.push_back(handler);
}

void QueueImpl::remove(EventHandlerPtr handler) {
  for(std::list<EventHandlerPtr>::iterator it = handlers_.begin();
      it != handlers_.end(); ++it) {
	  if(handler == (*it)) {
		  handlers_.remove(handler);
		  return;
	  }
  }
}

void QueueImpl::terminate() {
  for(std::list<EventHandlerPtr>::iterator it = handlers_.begin();
      it != handlers_.end(); ++it) {
    (*it)->terminate();
  }
}

EventStoreImpl::EventStoreImpl(boost::asio::io_service& ios) : ios_(ios), queues_() { }

EventStoreImpl::~EventStoreImpl() { }

void EventStoreImpl::bind(
  std::string qname,
  EventHandlerPtr handler) {
    QueuePtr q = lookupQueue(qname);
    if(!q) {
      q.reset(new QueueImpl(ios_));
      queues_.insert(std::make_pair(qname, q));
    }
    q->add(handler);
    handler->name(qname);
    handler->bind(q);
}

void EventStoreImpl::bind(std::string qname, QueuePtr q) {
    queues_.insert(std::make_pair(qname, q));
}

QueuePtr EventStoreImpl::lookupQueue(std::string qname) {
  std::map<std::string, QueuePtr>::iterator pos = queues_.find(qname);
  if(pos != queues_.end()) {
    return pos->second;
  } else {
    return QueuePtr();
  }
}

void EventStoreImpl::terminate() {
  std::map<std::string, QueuePtr>::iterator pos = queues_.begin();
  for(; pos != queues_.end(); ++pos) {
    pos->second->terminate();
  }
}

} // namespace BoostImpl
} // namespace EventStore
