#include <cstdlib>
#include <iostream>

#include <stm/Codec.hpp>

using namespace std;
using namespace stm;

class TheDecoder : public Decoder {
 public:
  TheDecoder(QueuePtr queue) { }
  virtual ~TheDecoder() { }

  void on(const Bytes& data) {
   queue_->enqueue(data); 
  }

  void onClose() {
   cout << __func__ << endl; 
  }
  
  void onTimeout() {
   cout << __func__ << endl; 
  }
 private:
  QueuePtr queue_;
};

class TheQueue : public Queue {
 public:
  virtual ~TheQueue() { }

  void enqueue(const Bytes&) {

  }

  void close() {

  }

  void timeout() {

  }
};

int main(int argc, char* argv[]) {
  QueuePtr queue(new TheQueue());
  DecoderPtr decoder(new TheDecoder(queue));

  decoder->onTimeout();
  decoder->onClose();
  return EXIT_SUCCESS;
}

