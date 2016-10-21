#include <cstdlib>
#include <iostream>

#include <EventStore/EventStore.h>

using namespace std;
using namespace EventStore;
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

class TheEventHandler : public EventHandler {
 public:
  TheEventHandler() { }
  virtual ~TheEventHandler() { }

  void on(const Event& e) {
    const TheEvent& event = dynamic_cast<const TheEvent&>(e);
    string ping = "ping";
    string pong = "pong";

    cout << this
        << ": "
        << __func__
        << "("
        << event.data()
        << ")" << endl;

    if(ping == event.data()) {
      const TheEvent reply(source(), pong);
      e.source()->enqueue(reply);
    } else if(pong == event.data()) {
    } else {
      const TheEvent reply(source(), ping);
      e.source()->enqueue(reply);
    }
  }
};

int main(int argc, char* argv[]) {
  EventHandlerPtr handler1(new TheEventHandler());
  EventHandlerPtr handler2(new TheEventHandler());

  TheEvent e(handler1->source(), string("wangxy"));
  handler2->source()->enqueue(e);

  //handler2->on(e);

  return EXIT_SUCCESS;
}
