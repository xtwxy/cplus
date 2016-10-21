#include <iostream>

#include <stm/Codec.h>

using namespace std;
using namespace boost;

namespace stm {

Event::Event(QueuePtr source) : source_(source) {
}

Event::~Event() {
}

QueuePtr Event::source() const {
  return source_;
}

Queue::Queue() { }

Queue::~Queue() { }

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

EventHandler::EventHandler() : source_() {
}

EventHandler::~EventHandler() { }

QueuePtr EventHandler::source() const {
  return source_;
}

void EventHandler::init(QueuePtr source) {
  source_ = source;
  source_->add(shared_from_this());
}

State::State() { }

State::~State() { }

} // namespace stm
