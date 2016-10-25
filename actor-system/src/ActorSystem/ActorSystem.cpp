#include <iostream>

#include <ActorSystem/ActorSystem.h>

using namespace std;
using namespace boost;

namespace ActorSystem {

Event::Event(QueuePtr source) : source_(source) {
}

Event::~Event() {
}

QueuePtr Event::source() const {
  return source_;
}

Queue::Queue() { }

Queue::~Queue() { }

EventHandler::EventHandler() : name_(), bind_(), outputs_() {
}

EventHandler::~EventHandler() { }

void EventHandler::name(const std::string name) {
	name_ = name;
}

const std::string EventHandler::name() const {
	return name_;
}

QueuePtr EventHandler::self() const {
  return bind_;
}

QueuePtr EventHandler::bind() const {
  return bind_;
}

void EventHandler::bind(QueuePtr q) {
  bind_ = q;
}

const std::vector<std::string> EventHandler::references() const {
  std::vector<std::string> refs;
  for(std::map<std::string, QueuePtr>::const_iterator it = outputs_.begin();
      it != outputs_.end(); ++it) {
    refs.push_back(it->first);
  }
  return refs;
}

void EventHandler::reference(std::string name, QueuePtr q) {
  outputs_.insert(std::make_pair(name, q));
}

void EventHandler::terminate() {
  bind_->remove(shared_from_this());
  bind_.reset();
  outputs_.clear();
}

ActorSystem::ActorSystem() { }

ActorSystem::~ActorSystem() { }

} // namespace ActorSystem
