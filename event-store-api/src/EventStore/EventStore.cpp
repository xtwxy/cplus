#include <iostream>

#include <EventStore/EventStore.h>

using namespace std;
using namespace boost;

namespace EventStore {

Event::Event(QueuePtr source) : source_(source) {
}

Event::~Event() {
}

QueuePtr Event::source() const {
  return source_;
}

Queue::Queue() { }

Queue::~Queue() { }

EventHandler::EventHandler() : source_() {
}

EventHandler::~EventHandler() { }

QueuePtr EventHandler::source() const {
  return source_;
}

void EventHandler::source(QueuePtr q) {
  source_ = q;
}
void EventHandler::init(const InitParams& params) {
  source_->add(shared_from_this());
}

EventStore::EventStore() { }

EventStore::~EventStore() { }

} // namespace EventStore
