#include <iostream>
#include <cstdlib>
#include <cstring>
#include <vector>

#include <boost/array.hpp>
#include <boost/bind.hpp>
#include <boost/asio.hpp>
#include <boost/asio/serial_port.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>

const size_t BUFFER_SIZE = 1024;
const boost::int64_t TIMEOUT_MILLISECONDS = 100;

using namespace std;
using namespace boost::asio;
using namespace boost;

io_service ioservice;

class SerialPortReader
{
public:
  explicit
  SerialPortReader (const string& portName, const int baudRate,
		    serial_port::parity::type parity, int dataBits,
		    serial_port::stop_bits::type stopBits, char* bytes,
		    size_t length) :
      port (ioservice, portName), output (ioservice, ::dup (STDOUT_FILENO)), lengthToWrite (
	  length), lengthRead (0), lengthWritten (0), timer (
	  ioservice, boost::posix_time::milliseconds (TIMEOUT_MILLISECONDS))
  {
    memcpy (dataToWrite, bytes, length);

    serial_port::baud_rate rate (baudRate);
    serial_port::parity theParity (parity);
    serial_port::character_size charSize (dataBits);
    serial_port::stop_bits stop_bits (stopBits);

    port.set_option (rate);
    port.set_option (theParity);
    port.set_option (charSize);
    port.set_option (stop_bits);
  }

  virtual
  ~SerialPortReader ()
  {
  }

  void
  read_complete (const boost::system::error_code& error,
		 std::size_t bytes_transferred)
  {
    if (!error)
      {
	timer.cancel ();
	timer.expires_from_now (
	    boost::posix_time::milliseconds (TIMEOUT_MILLISECONDS));
	timer.async_wait (
	    boost::bind (&SerialPortReader::printReceived, this,
			 boost::asio::placeholders::error));
	lengthRead += bytes_transferred;
	port.async_read_some (
	    buffer (dataToRead + lengthRead, BUFFER_SIZE - lengthRead),
	    boost::bind (&SerialPortReader::read_complete, this,
			 boost::asio::placeholders::error,
			 boost::asio::placeholders::bytes_transferred));
      }
    else
      {
	cerr << "Error: " << error << endl;
      }
  }
  void
  write_complete (const boost::system::error_code& error,
		  std::size_t bytes_transferred)
  {
    if (!error)
      {
	lengthWritten += bytes_transferred;
	if (lengthWritten != lengthToWrite)
	  {
	    async_write (
		port,
		buffer (dataToWrite + lengthWritten,
			lengthToWrite - lengthWritten),
		boost::bind (&SerialPortReader::write_complete, this,
			     boost::asio::placeholders::error,
			     boost::asio::placeholders::bytes_transferred));
	  }
	else
	  {
	    cerr << "bytes written: " << lengthToWrite << endl;
	    for (size_t i = 0; i < lengthToWrite; i++)
	      {
		cerr << hex << setw (2) << setfill ('0') << right
		    << unsigned (0xff & dataToWrite[i]) << " ";
	      }
	    cerr << endl;
	    port.async_read_some (
		buffer (dataToRead, BUFFER_SIZE),
		boost::bind (&SerialPortReader::read_complete, this,
			     boost::asio::placeholders::error,
			     boost::asio::placeholders::bytes_transferred));
	  }
      }
    else
      {
	cerr << "Error: " << error << endl;
      }
  }

  void
  printReceived (const boost::system::error_code& e)
  {
    if (!e)
      {
	cerr << "bytes received: " << lengthRead << endl;
	for (size_t i = 0; i < lengthRead; i++)
	  {
	    cerr << hex << setw (2) << setfill ('0') << right
		<< unsigned (0xff & dataToRead[i]) << " ";
	  }
	cerr << endl;
	ioservice.stop ();
      }
  }

  void
  start ()
  {
    timer.expires_from_now (
	boost::posix_time::milliseconds (TIMEOUT_MILLISECONDS));
    timer.async_wait (
	boost::bind (&SerialPortReader::printReceived, this,
		     boost::asio::placeholders::error));
    async_write (
	port,
	buffer (dataToWrite, lengthToWrite),
	boost::bind (&SerialPortReader::write_complete, this,
		     boost::asio::placeholders::error,
		     boost::asio::placeholders::bytes_transferred));
  }

private:
  SerialPortReader&
  operator = (const SerialPortReader& rhs)
  {
    return *this;
  }

  bool
  operator == (const SerialPortReader& rhs) const
  {
    return false;
  }

  serial_port port;
  char dataToRead[BUFFER_SIZE];
  char dataToWrite[BUFFER_SIZE];
  const size_t lengthToWrite;
  size_t lengthRead;
  size_t lengthWritten;
  boost::asio::posix::stream_descriptor output;
  boost::asio::deadline_timer timer;
};

char
parse_from_hex_str (const char* str)
{
  return strtoul (str, 0, 16);
}
int
main (int argc, char* argv[])
{

  if (argc < 3 || argc > 256)
    {
      cerr << "Usage: " << argv[0] << " <com port name> <max 256 bytes in hex>"
	  << endl;
      return EXIT_FAILURE;
    }

  char bytesToWrite[256];
  transform (argv + 2, argv + argc, bytesToWrite, parse_from_hex_str);
  SerialPortReader reader (argv[1], 9600, serial_port::parity::even, 8,
			   serial_port::stop_bits::one, bytesToWrite, argc - 2);

  reader.start ();

  ioservice.run ();

  return EXIT_SUCCESS;
}

