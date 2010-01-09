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
#include "SessionService.hpp"
#include "Session.hpp"
#include "Attribute.hpp"

#include <Zen/Core/Threading/I_Mutex.hpp>
#include <Zen/Core/Threading/MutexFactory.hpp>
#include <Zen/Core/Threading/CriticalSection.hpp>

#include <Zen/Core/Memory/managed_ptr.hpp>

#include <Zen/Core/Utility/runtime_exception.hpp>

#include <Zen/Enterprise/Networking/I_Endpoint.hpp>

#include <Zen/Enterprise/AppServer/I_Request.hpp>
#include <Zen/Enterprise/AppServer/I_ResponseHandler.hpp>
#include <Zen/Enterprise/AppServer/I_ProtocolService.hpp>

#include <Zen/Community/SessionProtocol/I_SessionProtocolManager.hpp>
#include <Zen/Community/SessionProtocol/I_LoginRequest.hpp>
#include <Zen/Community/SessionProtocol/I_LoginResponse.hpp>
#include <Zen/Community/SessionProtocol/I_DataRequest.hpp>
#include <Zen/Community/SessionProtocol/I_DataResponse.hpp>

#include <Zen/Community/SessionCommon/I_Session.hpp>
#include <Zen/Community/SessionCommon/I_Attribute.hpp>

#include <boost/bind.hpp>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Zen {
namespace Community {
namespace Client {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
SessionService::SessionService(Zen::Enterprise::AppServer::I_ApplicationServer& _appServer)
:   m_appServer(_appServer)
,   m_pThreadPool(NULL)
,	m_pHandlersMutex(Zen::Threading::MutexFactory::create())
{
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
SessionService::~SessionService()
{
	Zen::Threading::MutexFactory::destroy(m_pHandlersMutex);
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
SessionService::setConfiguration(const Zen::Plugins::I_ConfigurationElement& _config)
{
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
Zen::Threading::I_Condition*
SessionService::prepareToStart(Zen::Threading::ThreadPool& _threadPool)
{
    m_pThreadPool = &_threadPool;

    Zen::Community::Protocol::I_SessionProtocolManager::getSingleton().install(m_appServer);

    // Ready to go, so don't bother returning a condition variable
    return NULL;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
SessionService::start()
{
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
Zen::Threading::I_Condition*
SessionService::prepareToStop()
{
    return NULL;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
SessionService::stop()
{
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
SessionService::handleMessage(pMessage_type _pMessage)
{
	// Should always be a I_Response
	Zen::Memory::managed_ptr<Protocol::I_Response> pResponse(
		_pMessage.as<Zen::Memory::managed_ptr<Protocol::I_Response> >());

	if(pResponse.isValid())
	{
		Zen::Threading::CriticalSection guard(m_pHandlersMutex);
		Handlers_type::iterator iter = m_responseHandlers.find(pResponse->getRequestMessageId());

		if(iter != m_responseHandlers.end())
		{
			// Dispatch the response
			iter->second.get()->handleResponse(pResponse);

			m_responseHandlers.erase(iter);
		}
		else
		{
			// Probably a duplicate response... just ignore it
		}
	}
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
SessionService::handleRequest(pRequest_type _pRequest, pResponseHandler_type _pResponseHandler)
{
    Protocol::I_LoginRequest* pRequest = 
        dynamic_cast<Protocol::I_LoginRequest*>(_pRequest.get());

    if( pRequest != NULL )
    {
        Handlers_type::iterator iter = m_responseHandlers.find(pRequest->getRequestId());
        if( iter == m_responseHandlers.end() )
        {
            m_responseHandlers[pRequest->getRequestId()] = _pResponseHandler;
        }

        _pRequest->getDestinationEndpoint()->getProtocolAdapter().lock()->sendTo(
            _pRequest,
            _pRequest->getDestinationEndpoint()
        );
    }
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
Zen::Enterprise::AppServer::I_ApplicationServer&
SessionService::getApplicationServer()
{
    return m_appServer;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
#if 0   // deprecated
void
SessionService::requestSession(pEndpoint_type _pDestinationEndpoint, 
                           const std::string& _name, 
                           const std::string& _password,
                           pResponseHandler_type _pResponseHandler)
{
    pRequest_type pRequest = Zen::Community::Protocol::I_LoginRequest::create(pEndpoint_type(),
                                                                              _pDestinationEndpoint,
                                                                              _name,
                                                                              _password);

    handleRequest(pRequest, _pResponseHandler);
}
#endif  // deprecated

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
class LoginResponseHandler
:   public Zen::Enterprise::AppServer::I_ResponseHandler
{
    /// @name Types
    /// @{
public:
    typedef Zen::Memory::managed_weak_ptr<Zen::Enterprise::AppServer::I_ResponseHandler>    wpResponseHandler_type;
    typedef Zen::Memory::managed_weak_ptr<Zen::Community::Common::I_Session>                wpSession_type;
    /// @}

    /// @name I_ResponseHandler implementation
    /// @{
public:
    virtual void handleResponse(pResponse_type _pResponse)
    {
        Zen::Community::Protocol::I_LoginResponse* 
            pResponse = dynamic_cast<Zen::Community::Protocol::I_LoginResponse*>(_pResponse.get());

        Session* pRawSession = new Session(
            m_pParent,
            pResponse->getSessionId(),
            pResponse->getSessionState(),
            pResponse->getSourceEndpoint()
        );

        SessionService::pSession_type pSession(pRawSession, LoginResponseHandler::destroySession);

        m_pFutureSession->setValue(pSession);
    }
    /// @}

    /// @name LoginResponseHandler implementation
    /// @{
public:
    SessionService::pFutureSession_type getSession()
    {
        return m_pFutureSession;
    }

    /// @name Static methods
    /// @{
public:
    static void destroy(wpResponseHandler_type _wpResponseHandler)
    {
        LoginResponseHandler* pResponseHandler = 
            dynamic_cast<LoginResponseHandler*>(_wpResponseHandler.get());

        if( pResponseHandler != NULL )
        {
            delete pResponseHandler;
        }
        else
        {
            throw Zen::Utility::runtime_exception("LoginResponseHandler::destroy() : Invalid type.");
        }
    }

    static void destroySession(wpSession_type _wpSession)
    {
        Session* pSession =
            dynamic_cast<Session*>(_wpSession.get());

        if( pSession != NULL )
        {
            delete pSession;
        }
        else
        {
            throw Zen::Utility::runtime_exception("LoginResponseHandler::destroySession() : Invalid type.");
        }
    }
    /// @}

    /// @name 'Structors
    /// @{
public:
             LoginResponseHandler(SessionService::pService_type _pParent) : m_pParent(_pParent), m_pFutureSession(new SessionService::FutureSession_type()) {}
    virtual ~LoginResponseHandler() {}
    /// @}

    /// @name Member variables
    /// @{
private:
    SessionService::pFutureSession_type           m_pFutureSession;
    SessionService::pService_type                 m_pParent;
    /// @}

};  // class LoginResponseHandler

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
SessionService::pFutureSession_type
SessionService::requestSession(pEndpoint_type _pDestinationEndpoint, 
                           const std::string& _name, 
                           const std::string& _password)
{
    pRequest_type pRequest = Zen::Community::Protocol::I_LoginRequest
        ::create(pEndpoint_type(),
                _pDestinationEndpoint,
                _name,
                _password);

    LoginResponseHandler* pRawResponseHandler(new LoginResponseHandler(getSelfReference().lock()));

    pResponseHandler_type pResponseHandler(pRawResponseHandler, &LoginResponseHandler::destroy);
    handleRequest(pRequest, pResponseHandler);

    return pRawResponseHandler->getSession();
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
class DataResponseHandler
:   public Zen::Enterprise::AppServer::I_ResponseHandler
{
    /// @name Types
    /// @{
public:
    typedef Zen::Memory::managed_weak_ptr<Zen::Enterprise::AppServer::I_ResponseHandler>    wpResponseHandler_type;
    typedef Zen::Memory::managed_weak_ptr<Zen::Community::Common::I_Attribute>              wpAttribute_type;
    /// @}

    /// @name I_ResponseHandler implementation
    /// @{
public:
    virtual void handleResponse(pResponse_type _pResponse)
    {
        Zen::Community::Protocol::I_DataResponse* 
            pResponse = dynamic_cast<Zen::Community::Protocol::I_DataResponse*>(_pResponse.get());

        Attribute* pRawAttribute = new Attribute(
            pResponse->getKey(),
            pResponse->getValue()
        );

        SessionService::pAttribute_type pAttribute(pRawAttribute, DataResponseHandler::destroyAttribute);

        m_pFutureAttribute->setValue(pAttribute);
    }
    /// @}

    /// @name DataResponseHandler implementation
    /// @{
public:
    SessionService::pFutureAttribute_type getAttribute()
    {
        return m_pFutureAttribute;
    }

    /// @name Static methods
    /// @{
public:
    static void destroy(wpResponseHandler_type _wpResponseHandler)
    {
        DataResponseHandler* pResponseHandler = 
            dynamic_cast<DataResponseHandler*>(_wpResponseHandler.get());

        if( pResponseHandler != NULL )
        {
            delete pResponseHandler;
        }
        else
        {
            throw Zen::Utility::runtime_exception("DataResponseHandler::destroy() : Invalid type.");
        }
    }

    static void destroyAttribute(wpAttribute_type _wpAttribute)
    {
        Attribute* pAttribute =
            dynamic_cast<Attribute*>(_wpAttribute.get());

        if( pAttribute != NULL )
        {
            delete pAttribute;
        }
        else
        {
            throw Zen::Utility::runtime_exception("DataResponseHandler::destroyAttribute() : Invalid type.");
        }
    }
    /// @}

    /// @name 'Structors
    /// @{
public:
             DataResponseHandler() {}
    virtual ~DataResponseHandler() {}
    /// @}

    /// @name Member variables
    /// @{
private:
    SessionService::pFutureAttribute_type     m_pFutureAttribute;
    /// @}

};  // class DataResponseHandler

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
SessionService::pFutureAttribute_type
SessionService::requestAttribute(const Common::I_Session& _session,
                               const std::string& _key)
{
    pRequest_type pRequest = Zen::Community::Protocol::I_DataRequest
        ::create(
            pEndpoint_type(),
            _session.getEndpoint(),
            _session.getSessionId(),
            _key
        );
                 
    DataResponseHandler* pRawResponseHandler(new DataResponseHandler());

    pResponseHandler_type pResponseHandler(pRawResponseHandler, &DataResponseHandler::destroy);
    handleRequest(pRequest, pResponseHandler);

    return pRawResponseHandler->getAttribute();

}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace Client
}   // namespace Community
}   // namespace Zen
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
