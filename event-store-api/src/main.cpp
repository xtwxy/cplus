#include <cstdlib>
#include <iostream>

#include <EventStore/EventStore.h>
#include <EventStore/BoostImpl/EventStoreBoostImpl.h>

using namespace std;
using namespace EventStore;
using namespace ::EventStore::BoostImpl;
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
  TheEventHandler(std::size_t count = 10000000) :count_(count) { }
  virtual ~TheEventHandler() { }

  void on(const EventPtr e) {
    count_ -= 1;
    if(count_ == 0) {
      terminate();
      return;
    }
    const TheEventPtr event = boost::dynamic_pointer_cast<TheEvent>(e);
    string ping = "ping";
    string pong = "pong";

    /*
    cout << this
        << ": "
        << __func__
        << "("
        << event->data()
        << ")" << endl;
    */
    if(ping == event->data()) {
      const TheEventPtr reply(new TheEvent(self(), pong));
      e->source()->post(reply);
    } else if(pong == event->data()) {
    } else {
      const TheEventPtr reply(new TheEvent(self(), event->data()));
      e->source()->post(reply);
    }
  }

  void terminate() {
    EventHandler::terminate();
  }
 private:
  std::size_t count_;
};

int main(int argc, char* argv[]) {
  boost::asio::io_service ios;

  EventHandlerPtr handler1(new TheEventHandler());
  EventHandlerPtr handler2(new TheEventHandler());

  EventStorePtr store(new EventStoreImpl(ios));
  store->bind("handler1", handler1);
  store->bind("handler2", handler2);
  string msg = "ping";
  if(argc > 1) {
    msg = argv[1];
  }
  TheEventPtr e(new TheEvent(handler1->bind(), msg));
  handler2->bind()->post(e);

  //handler2->on(e);
  ios.run();

  return EXIT_SUCCESS;
}
