//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
// Zen Enterprise Framework
//
// Copyright (C) 2001 - 2008 Tony Richards
//
//  This software is provided 'as-is', without any express or implied
//  warranty.  In no event will the authors be held liable for any damages
//  arising from the use of this software.
//
//  Permission is granted to anyone to use this software for any purpose,
//  including commercial applications, and to alter it and redistribute it
//  freely, subject to the following restrictions:
//
//  1. The origin of this software must not be misrepresented; you must not
//     claim that you wrote the original software. If you use this software
//     in a product, an acknowledgment in the product documentation would be
//     appreciated but is not required.
//  2. Altered source versions must be plainly marked as such, and must not be
//     misrepresented as being the original software.
//  3. This notice may not be removed or altered from any source distribution.
//
//  Tony Richards trichards@indiezen.com
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
#ifndef ZEN_ENTERPRISE_APPSERVER_ENDPOINT_HPP_INCLUDED
#define ZEN_ENTERPRISE_APPSERVER_ENDPOINT_HPP_INCLUDED

#include <Zen/Enterprise/Networking/I_Endpoint.hpp>

#include <boost/asio.hpp>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Zen {
namespace Enterprise {
namespace AppServer {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

class Endpoint
:   public Zen::Networking::I_Endpoint
{
    /// @name Types
    /// @{
public:
    /// @}

    /// @name I_Endpoint implementation
    /// @{
public:
    virtual wpProtocolService_type getProtocolAdapter() const;
    virtual const std::string& toString() const { return m_endpointString; }
    virtual const Zen::Networking::I_Address& getAddress() const;
    virtual void setIsLocal(bool _isLocal);
    virtual bool isLocal() const;
    /// @}

    /// @name Endpoint implementation
    /// @{
public:
    boost::asio::ip::tcp::endpoint& getEndpoint();
    /// @}

    /// @name 'Structors
    /// @{
protected:
    friend class TransmissionControlProtocolService;
    friend class HyperTextTransportProtocolService;
    explicit Endpoint(wpProtocolService_type _pProtocolAdapter, const boost::asio::ip::tcp::endpoint& _endpoint);
    virtual ~Endpoint();
    /// @}

    /// @name Member Variables
    /// @{
private:
    wpProtocolService_type          m_pProtocolAdapter;
    boost::asio::ip::tcp::endpoint  m_endpoint;
    bool                            m_isLocal;
    std::string                     m_endpointString;
    /// @}

};  // class Endpoint

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace AppServer
}   // namespace Enterprise
namespace Memory 
{
    /// Endpoint is managed by a factory
    template<>
    struct is_managed_by_factory<Enterprise::AppServer::Endpoint> : public boost::true_type{};
}   // namespace Memory
}   // namespace Zen
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

#endif // ZEN_ENTERPRISE_APPSERVER_ENDPOINT_HPP_INCLUDED
