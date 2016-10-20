#include <cstdlib>
#include <iostream>

#include <stm/Codec.hpp>

using namespace std;
using namespace stm;
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
  TheEventHandler(QueuePtr output) : output_(output) { }
  virtual ~TheEventHandler() { }

  void on(const Event& data) {
    const TheEvent& event = dynamic_cast<const TheEvent&>(data); 
    string msg = "hello, ";
    msg.append(event.data());

    const TheEvent reply(queue_, msg); 

    cout << __func__ 
        << "(" 
        << event.data() 
        << ")" << endl;

    queue_->enqueue(reply); 
  }

 private:
  QueuePtr output_;
};

int main(int argc, char* argv[]) {
  QueuePtr decoderQueue(new TheQueue());
  QueuePtr source(new TheQueue());
  QueuePtr output(new TheQueue());
  EventHandlerPtr decoder(new TheEventHandler(output));

  TheEvent e(source, string("wangxy"));

  decoder->on(e);
  
  return EXIT_SUCCESS;
}

