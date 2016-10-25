#include <cstdlib>
#include <iostream>
#include <iomanip>

#include <boost/lexical_cast.hpp>
#include <ActorSystem/ActorSystem.h>
#include <ActorSystem/BoostImpl/ActorSystemBoostImpl.h>

using namespace std;
using namespace ActorSystem;
using namespace ::ActorSystem::BoostImpl;
using namespace boost;

class TheEvent : public Event {
 public:
  TheEvent(QueuePtr source, const string data) : Event(source), data_(data) { }
  virtual ~TheEvent() { }

  string data() const {
    return data_;
  }

 private:
  string data_;
};

typedef boost::shared_ptr<TheEvent> TheEventPtr;

class TheEventHandler : public EventHandler {
 public:
  TheEventHandler(std::size_t count = 1) :count_(count) { }
  virtual ~TheEventHandler() { }

  void on(const EventPtr e) {
    count_ -= 1;
    const TheEventPtr event = boost::dynamic_pointer_cast<TheEvent>(e);
    string ping = "ping";
    string pong = "pong";

    cout //<< hex << showbase << internal << setw(16) << setfill('0')
    	<< this
    	<< ": ["
		<< count_
		<< "]"
        << ": "
		<< name()
    	<< "::"
        << __func__
        << "(\""
        << event->data()
        << "\")" << endl;

    if(ping == event->data()) {
      const TheEventPtr reply(new TheEvent(self(), pong));
      e->source()->post(reply);
    } else if(pong == event->data()) {
    } else {
      const TheEventPtr reply(new TheEvent(self(), event->data()));
      e->source()->post(reply);
    }
    if(count_ == 0) {
      terminate();
      return;
    }
  }

 protected:
  void stop() {
  }
 private:
  std::size_t count_;
};

int main(int argc, char* argv[]) {
  boost::asio::io_service ios;

  std::size_t count = 1;
  string msg = "ping";
  if(argc > 1) {
    msg = argv[1];
  }
  if(argc > 2) {
	 count = boost::lexical_cast<std::size_t>(argv[2]);
  }
  EventHandlerPtr handler1(new TheEventHandler(count));
  EventHandlerPtr handler2(new TheEventHandler(count));

  ActorSystemPtr store(new ActorSystemImpl(ios));
  store->bind("handler1", handler1);
  store->bind("handler2", handler2);
  TheEventPtr e(new TheEvent(handler1->bind(), msg));
  handler2->bind()->post(e);

  ios.run();

  return EXIT_SUCCESS;
}
