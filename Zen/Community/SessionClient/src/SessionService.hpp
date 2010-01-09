//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
// Zen Community Framework
//
// Copyright (C) 2001 - 2009 Tony Richards
// Copyright (C) 2008 - 2009 Matthew Alan Gray
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
//	Matthew Alan Gray mgray@indiezen.org
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
#ifndef ZEN_COMMUNITY_SESSIONCLIENT_SESSION_SERVICE_HPP_INCLUDED
#define ZEN_COMMUNITY_SESSIONCLIENT_SESSION_SERVICE_HPP_INCLUDED

#include <Zen/Core/Memory/managed_ptr.hpp>
#include <Zen/Core/Memory/managed_weak_ptr.hpp>
#include <Zen/Core/Memory/managed_self_ref.hpp>

#include <Zen/Enterprise/AppServer/I_ApplicationService.hpp>
#include <Zen/Enterprise/AppServer/I_ApplicationServer.hpp>

#include <Zen/Community/SessionCommon/I_SessionService.hpp>

#include <map>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Zen {
	namespace Threading {
		class I_Mutex;
	}	// namespace Threading
    namespace Networking {
        class I_Endpoint;
    }   // namespace Networking
namespace Community {
    namespace Common {
        class I_Attribute;
    }   // namespace Common
namespace Client {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
;

class SessionService
:   public Zen::Community::Common::I_SessionService
,   public Zen::Memory::managed_self_ref<Common::I_SessionService>
{
    /// @name Types
    /// @{
public:
    typedef Zen::Memory::managed_ptr<Zen::Networking::I_Endpoint>   pEndpoint_type;

	typedef std::map<unsigned int, pResponseHandler_type>		    Handlers_type;

    typedef Zen::Memory::managed_ptr<Common::I_Attribute>           pAttribute_type;
    typedef Zen::Event::future_return_value<pAttribute_type>        FutureAttribute_type;
    typedef Zen::Memory::managed_ptr<FutureAttribute_type>          pFutureAttribute_type;
    /// @}

    /// @name I_StartupShutdownParticipant implementation
    /// @{
public:
    virtual void setConfiguration(const Zen::Plugins::I_ConfigurationElement& _config);
    virtual Zen::Threading::I_Condition* prepareToStart(Zen::Threading::ThreadPool& _threadPool);
    virtual void start();
    virtual Zen::Threading::I_Condition* prepareToStop();
    virtual void stop();    
    /// @}

    /// @name I_RequestHandler implementation
    /// @{
public:
    virtual void handleRequest(pRequest_type _pRequest, pResponseHandler_type _pResponseHandler);
    /// @}

    /// @name I_ApplicationService implementation
    /// @{
public:
    virtual Zen::Enterprise::AppServer::I_ApplicationServer& getApplicationServer();
    virtual void handleMessage(pMessage_type _pMessage);
    /// @}

    /// @name I_SessionService implementation
    /// @{
public:
#if 0   // deprecated
    virtual void requestSession(pEndpoint_type _pDestinationEndpoint, 
                              const std::string& _name, 
                              const std::string& _password,
                              pResponseHandler_type _pResponseHandler);
#endif  // deprecated

    virtual pFutureSession_type requestSession(pEndpoint_type _pDestinationEndpoint, 
                                             const std::string& _name, 
                                             const std::string& _password);
    /// @}

	/// @name SessionService implementation
	/// @{
public:
    pFutureAttribute_type requestAttribute(const Common::I_Session& _session,
                                           const std::string& _key);
	/// @}

    /// @name 'Structors
    /// @{
protected:
    friend class SessionServiceFactory;
             SessionService(Zen::Enterprise::AppServer::I_ApplicationServer& _appServer);
    virtual ~SessionService();
    /// @}

    /// @name Member Variables
    /// @{
private:
    Zen::Enterprise::AppServer::I_ApplicationServer&    m_appServer;
    Zen::Threading::ThreadPool*                         m_pThreadPool;

	/// Map from MessageID() to the response handler
	Handlers_type										m_responseHandlers;
	Zen::Threading::I_Mutex*							m_pHandlersMutex;
    /// @}

};  // class SessionService

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace Client
}   // namespace Community
}   // namespace Zen
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

#endif // ZEN_COMMUNITY_SESSIONCLIENT_SESSION_SERVICE_HPP_INCLUDED
