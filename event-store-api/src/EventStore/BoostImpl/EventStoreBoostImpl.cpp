#include <boost/make_shared.hpp>
#include <EventStore/BoostImpl/EventStoreBoostImpl.h>

namespace EventStore {
namespace BoostImpl {

QueueImpl::QueueImpl() : handlers_() { }

QueueImpl::~QueueImpl() { }

void QueueImpl::enqueue(const Event& e) {
  for(std::list<EventHandlerPtr>::iterator it = handlers_.begin();
      it != handlers_.end(); ++it) {
    (*it)->on(e);
  }
}

void QueueImpl::add(EventHandlerPtr handler) {
  handlers_.push_back(handler);
}

EventStoreImpl::EventStoreImpl() : queues_() { }

EventStoreImpl::~EventStoreImpl() { }

void EventStoreImpl::registerHandler(
  std::string qname,
  EventHandlerPtr handler) {
    QueuePtr q = lookupQueue(qname);
    if(!q) {
      q = boost::make_shared<QueueImpl>();
      queues_.insert(std::make_pair(qname, q));
    }
    q->add(handler);
}

void EventStoreImpl::registerQueue(std::string qname, QueuePtr q) {
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

} // namespace BoostImpl
} // namespace EventStore
