#ifndef _CODEC_PIPELINE_HPP_
#define _CODEC_PIPELINE_HPP_

#include <vector>
#include <boost/function.hpp>
#include <Codec.hpp>

typedef boost::any Error;

class Pipeline {
 public:
  typedef boost::function<
    void (const Error&)> PushCompleteHandler;
  typedef boost::function<
    void (const Error&, Message&)> CallCompleteHandler;
  typedef boost::function<
    void (const Message&, UpperStream&)> OnCallHandler;

  Pipeline() { }
  virtual ~Pipeline() { }

  void sendMessage(Message&, PushCompleteHandler) = 0;
  void onMessage(Message&) = 0;
  void onError(const Error&) = 0;

  void call(CallCompleteHandler) = 0;
  void onCall(CallCompleteHandler) = 0;

  void setHandler(MessageHandler::Ptr handler) {
    msgHandler = handler;
  }

  void addEncoder(Encoder::Ptr encoder) {
    encoders.push_back(encoder);
  }

  void addDecoder(Decoder::Ptr) {
    decoders.push_back(decoder);
  }

 private:
  MessageHandler::Ptr msgHandler;
  std::vector<Encoder::Ptr> encoders;
  std::vector<Decoder::Ptr> decoders;
};

#endif //defined _CODEC_PIPELINE_HPP_
