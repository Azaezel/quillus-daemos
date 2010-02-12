//
// async_udp_echo_server.cpp
// ~~~~~~~~~~~~~~~~~~~~~~~~~
//
// Copyright (c) 2003-2008 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

// general refference sheet: http://www.boost.org/doc/libs/1_37_0/doc/html/boost_asio/reference.html
#include "I_networking.hpp"

#include <boost/cstdint.hpp>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace ZeeBall {
namespace Client {

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
I_NetCommon::sendInfoRequest(char *_packetData, size_t _len, boost::asio::ip::address_v4 _connection)
{
    boost::asio::io_service io_service;
    udp::socket s(io_service, udp::endpoint(udp::v4(), 0));
    udp::resolver resolver(io_service);
    udp::resolver::query query(udp::v4(), _connection.to_string());
    udp::resolver::iterator iterator = resolver.resolve(query);

	boost::system::error_code ignored_error;
    s.send_to(boost::asio::buffer(_packetData), *iterator, 0, ignored_error);
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
I_NetCommon::recieveInfo(boost::asio::ip::address_v4 _connection)
{
	
	boost::array<char, 1500> recieve_buf;
    boost::asio::io_service io_service;
    udp::socket s(io_service, udp::endpoint(udp::v4(), 0));

    udp::endpoint sender_endpoint;
	boost::system::error_code error;
	size_t reply_length = s.receive_from(boost::asio::buffer(recieve_buf), sender_endpoint, 0, error);

	if (reply_length>0)
		unpackInfo(recieve_buf.data(), reply_length);
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
I_NetCommon::packInfo(char* _request, boost::asio::ip::address_v4 _connection)
{
	//placeholder
    using namespace std; // For strlen.
    std::cout << "Send: ";
	std::cin.getline(_request, m_max_length);
    size_t request_length = strlen(_request);
	// end placeholder

	sendInfoRequest(_request, request_length, _connection);
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
I_NetCommon::unpackInfo(char *_packetData, size_t _len)
{
	//placeholder
    std::cout << "Recieve: ";
    std::cout.write(_packetData, _len);
    std::cout << "\n";
	// end placeholder
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
int
I_NetServer::initialiseHosting(unsigned short _port)
{
  try
  {
    boost::asio::io_service io_service;

    using namespace std; // For atoi.
    I_NetServer s(io_service, _port);

    io_service.run();
  }
  catch (std::exception& e)
  {
    std::cerr << "Exception: " << e.what() << "\n";
  }

  return 0;
}
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
int
I_NetClient::initialiseListening(char* _ip)
{
  try
  {
	char* currentmessage = "Login";

	packInfo("Login", boost::asio::ip::address_v4::from_string(_ip));
  }
  catch (std::exception& e)
  {
    std::cerr << "Exception: " << e.what() << "\n";
  }

  return 0;
}
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace Client
}   // namespace ZeeBall