#ifndef _CODEC_BYTE_STREAM_HPP_
#define _CODEC_BYTE_STREAM_HPP_

#include <boost/asio.hpp>
#include <boost/asio/buffer.hpp>
#include <boost/function.hpp>

template<typename Message>
class UpperStream {
 public:
  virtual ~UpperStream() { }

  virtual void write(Message& msg) = 0; 
  virtual void onClosed() = 0;
  virtual void onTimeout() = 0;

};


template<typename Message>
class DownStream {
 public:
  virtual ~DownStream() { }

  virtual void write(Message& msg) = 0; 
  virtual void onClosed() = 0;
  virtual void onTimeout() = 0;

};


class ByteStream {
 public:
	typedef boost::asio::streambuf::mutable_buffers_type ReadBuffer;
	typedef boost::asio::streambuf::const_buffers_type WriteBuffer;
	typedef boost::function<
			void (const boost::system::error_code&, std::size_t)> IoCompHandler;

  virtual ~ByteStream() { }
  virtual void read(ReadBuffer, IoCompHandler) = 0;
  virtual void write(WriteBuffer, IoCompHandler) = 0;
  virtual void setReadTimeout(unsigned long millis) = 0;
  virtual void setWriteTimeout(unsigned long millis) = 0;
};
#endif //defined _CODEC_BYTE_STREAM_HPP_
