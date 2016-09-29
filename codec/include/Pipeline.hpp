#ifndef _CODEC_PIPELINE_HPP_
#define _CODEC_PIPELINE_HPP_

#include <boost/function.hpp>

template<typename Error, typename InMessage, typename OutMessage>
class Pipeline {
 public:
  typedef boost::function<
    void (const Error&, InMessage&)> CompHandler;

  virtual ~Pipeline() { }

  virtual void sendMessage(CompleteHandler) = 0;
  virtual void onMessage(CompleteHandler) = 0;

  virtual void call(CompleteHandler) = 0;
  virtual void onCall(CompleteHandler) = 0;
};

#endif //defined _CODEC_PIPELINE_HPP_
