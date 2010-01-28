//
// async_udp_echo_server.cpp
// ~~~~~~~~~~~~~~~~~~~~~~~~~
//
// Copyright (c) 2003-2008 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#include <cstdlib>
#include <iostream>
#include <boost/bind.hpp>
#include <boost/asio.hpp>

using boost::asio::ip::udp;

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace ZeeBall {
namespace Client {

class I_NetConnection
{
	I_NetConnection();
	~I_NetConnection();
	boost::asio::ip::address_v4 connectionIP;
	unsigned short ping;
};

class I_NetConnectionList
{
	I_NetConnectionList();
	~I_NetConnectionList();
	I_NetConnection* head;
	void pingsort();
	void forwardList(boost::asio::ip::address_v4 endpoint);
};

class I_NetCommon
{
public:
	I_NetCommon(boost::asio::io_service& _io_service, short _port)
		: m_io_service(_io_service), m_socket(m_io_service, udp::endpoint(udp::v4(), _port))
	{
		m_socket.async_receive_from(
			boost::asio::buffer(m_data, m_max_length), m_senderEndpoint,
			boost::bind(&I_NetCommon::handle_receive_from, this,
			boost::asio::placeholders::error,
			boost::asio::placeholders::bytes_transferred));
	}
		
	void handle_receive_from(const boost::system::error_code& _error, size_t _bytes_recvd)
	{
		if (!_error && _bytes_recvd > 0)
		{
			m_socket.async_send_to(
				boost::asio::buffer(m_data, _bytes_recvd), m_senderEndpoint,
				boost::bind(&I_NetCommon::handle_send_to, this,
				boost::asio::placeholders::error,
				boost::asio::placeholders::bytes_transferred));
		}
		else
		{
			m_socket.async_receive_from(
				boost::asio::buffer(m_data, m_max_length), m_senderEndpoint,
				boost::bind(&I_NetCommon::handle_receive_from, this,
				boost::asio::placeholders::error,
				boost::asio::placeholders::bytes_transferred));
		}
	}
	void handle_send_to(const boost::system::error_code& error, size_t bytes_sent)
	{
		m_socket.async_receive_from(
			boost::asio::buffer(m_data, m_max_length), m_senderEndpoint,
			boost::bind(&I_NetCommon::handle_receive_from, this,
			boost::asio::placeholders::error,
			boost::asio::placeholders::bytes_transferred));
	}

	void sendInfoRequest(char *_packetData, size_t _len, boost::asio::ip::address_v4 _connection);	//for communicated data interpretation
	void recieveInfo(boost::asio::ip::address_v4 endpoint);											//for communicated data interpretation
		
	void packInfo(char* _request, boost::asio::ip::address_v4 _connection);						//datastream communication
	void unpackInfo(char *_packetData, size_t _len);											//datastream communication

public:
	enum { m_max_length = 1024 };
	char m_data[m_max_length];
	boost::asio::io_service& m_io_service;
	udp::socket m_socket;
	udp::endpoint m_senderEndpoint;

};

class I_NetServer : public I_NetCommon
{
	typedef I_NetCommon mParent;  

public:
	I_NetServer(boost::asio::io_service& _io_service, short _port): I_NetCommon(_io_service,_port){};

	void sendInitialBreifing();
	void updatePulse(I_NetConnectionList *_updatelist); //also tack on an object list to this one as a second arg for the 4 frequencies
	int initialiseHosting(unsigned short _port);
};

class I_NetClient : public I_NetCommon
{
	typedef I_NetCommon mParent;
public:
	I_NetClient(boost::asio::io_service& _io_service, short _port):I_NetCommon(_io_service,_port){};
	
	int initialiseListening(char* _ip);
};

/*
//initial server usage: http://www.boost.org/doc/libs/1_37_0/doc/html/boost_asio/example/echo/async_udp_echo_server.cpp

int main(int argc, char* argv[])
{
  try
  {
    if (argc != 2)
    {
      std::cerr << "Usage: async_udp_echo_server <port>\n";
      return 1;
    }

    boost::asio::io_service io_service;

    using namespace std; // For atoi.
    server s(io_service, atoi(argv[1]));

    io_service.run();
  }
  catch (std::exception& e)
  {
    std::cerr << "Exception: " << e.what() << "\n";
  }

  return 0;
}
*/

//initial client usage: http://www.boost.org/doc/libs/1_37_0/doc/html/boost_asio/example/echo/blocking_udp_echo_client.cpp
/*
int main(int argc, char* argv[])
{
  try
  {
    if (argc != 3)
    {
      std::cerr << "Usage: blocking_udp_echo_client <host> <port>\n";
      return 1;
    }

    boost::asio::io_service io_service;

    udp::socket s(io_service, udp::endpoint(udp::v4(), 0));

    udp::resolver resolver(io_service);
    udp::resolver::query query(udp::v4(), argv[1], argv[2]);
    udp::resolver::iterator iterator = resolver.resolve(query);

    using namespace std; // For strlen.
    std::cout << "Enter message: ";
    char request[max_length];
    std::cin.getline(request, max_length);
    size_t request_length = strlen(request);
    s.send_to(boost::asio::buffer(request, request_length), *iterator);

    char reply[max_length];
    udp::endpoint sender_endpoint;
    size_t reply_length = s.receive_from(
        boost::asio::buffer(reply, max_length), sender_endpoint);
    std::cout << "Reply is: ";
    std::cout.write(reply, reply_length);
    std::cout << "\n";
  }
  catch (std::exception& e)
  {
    std::cerr << "Exception: " << e.what() << "\n";
  }

  return 0;
}

*/
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace Client
}   // namespace ZeeBall