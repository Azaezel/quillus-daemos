//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
// Zen Enterprise Framework
//
// Copyright (C) 2001 - 2010 Tony Richards
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
#ifndef ZEN_APPSERVER_SCRIPTABLE_GENERIC_SERVICE_HPP_INCLUDED
#define ZEN_APPSERVER_SCRIPTABLE_GENERIC_SERVICE_HPP_INCLUDED

#include <Zen/Core/Memory/managed_ptr.hpp>
#include <Zen/Core/Memory/managed_weak_ptr.hpp>
#include <Zen/Core/Memory/managed_self_ref.hpp>

#include <Zen/Core/Threading/I_Mutex.hpp>
#include <Zen/Core/Threading/MutexFactory.hpp>
#include <Zen/Core/Threading/CriticalSection.hpp>

#include <Zen/Core/Scripting.hpp>

#include <Zen/Enterprise/AppServer/I_ApplicationService.hpp>
#include <Zen/Enterprise/AppServer/I_ApplicationServer.hpp>

#include <Zen/Enterprise/AppServer/I_Request.hpp>
#include <Zen/Enterprise/AppServer/I_ResponseHandler.hpp>
#include <Zen/Enterprise/AppServer/I_ProtocolService.hpp>

#include <Zen/Enterprise/Networking/I_Endpoint.hpp>

#include <map>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Zen {
    namespace Threading {
        class ThreadPool;
    } // namespace Threading
namespace Enterprise {
namespace AppServer {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace detail {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
;

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
/// Handler Base.
/// Used to make the destructor visible.
struct handler_base
:   public I_ResponseHandler
{
    virtual ~handler_base()
    {
    }
};

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
template<typename Request_type, typename Payload_type>
struct response_handler
:   public handler_base
{
    typedef typename Request_type::pRequest_type                pRequest_type;
    typedef boost::function<void(pResponse_type, Request_type&, Payload_type)>  Function_type;

    virtual void handleResponse(pResponse_type _pResponse)
    {
        // Use the function to dispatch the handler.
        m_function(_pResponse, *dynamic_cast<Request_type*>(m_pRequest.get()), m_payload);
    }

    response_handler(pRequest_type _pRequest, Payload_type _payload, Function_type _function)
    :   m_function(_function)
    ,   m_pRequest(_pRequest)
    ,   m_payload(_payload)
    {
    }

    virtual ~response_handler()
    {
    }

public:

    Function_type               m_function;
    pRequest_type               m_pRequest;
    Payload_type                m_payload;
};  // struct response_handler

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
} // namespace detail
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

template<typename Request_type, typename Payload_type>
class create_request
{
    /// @name Types
    /// @{
public:
    typedef Zen::Memory::managed_ptr<Zen::Networking::I_Endpoint>   pEndpoint_type;
    typedef typename Request_type::pRequest_type                    pRequest_type;
    /// @}

public:
    inline
    Request_type* operator->()
    {
        return m_pRawRequest;
    }

    /// @name 'Structors
    /// @{
public:
    inline
    create_request(pEndpoint_type _pEndpoint, Payload_type _payload)
    :   m_payload(_payload)
    ,   m_pRequest(Request_type::create(pEndpoint_type(), _pEndpoint))
    {
        m_pRawRequest = dynamic_cast<typename Request_type*>(m_pRequest.get());
    }
    /// @}

    /// @name Member Variables
    /// @{
public:
    Payload_type                m_payload;
    Request_type*               m_pRawRequest;
    pRequest_type               m_pRequest;    
    /// @}
};

template<typename BaseClass_type, typename Class_type>
class scriptable_generic_service
:   public BaseClass_type
,   public Zen::Scripting::I_ScriptableService
,   public Zen::Memory::managed_self_ref<BaseClass_type>
{
    /// @name Types
    /// @{
public:
    typedef Class_type*                                             pScriptObject_type;
    typedef Zen::Scripting::ObjectReference<Class_type>             ScriptObjectReference_type;
    typedef ScriptObjectReference_type                              ScriptWrapper_type;
    typedef ScriptWrapper_type*                                     pScriptWrapper_type;

    typedef Zen::Memory::managed_ptr<Zen::Networking::I_Endpoint>   pEndpoint_type;
    typedef I_ApplicationServer::pResponseHandler_type              pResponseHandler_type;
    typedef I_ApplicationServer::pRequest_type                      pRequest_type;
    typedef I_ApplicationServer::pResponse_type                     pResponse_type;
    typedef I_ApplicationServer::pMessage_type                      pMessage_type;

    typedef Memory::managed_ptr<I_ResponseHandler>                  pResponseHandler_type;
    typedef std::map<unsigned int, pResponseHandler_type>           ResponseHandlers_type;
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
    virtual I_ApplicationServer& getApplicationServer();
    virtual void handleMessage(pMessage_type _pMessage);
    /// @}

    /// @name Implementation
    /// @{
public:
    template<typename Request_type, typename Payload_type>
    void send(create_request<Request_type, Payload_type>& _request, boost::function<void(pResponse_type, Request_type&, Payload_type)> _function);
    /// @}

    /// @name 'Structors
    /// @{
protected:
             scriptable_generic_service(Zen::Enterprise::AppServer::I_ApplicationServer& _appServer);
    virtual ~scriptable_generic_service();
    /// @}

    /// @name Member Variables
    /// @{
private:
    I_ApplicationServer&                                m_appServer;

    /// Map from getRequestId() to the response handler.
    ResponseHandlers_type                                m_responseHandlers;

    /// Mutex to guard m_responseHandlers.
    Zen::Threading::I_Mutex*                            m_pHandlersMutex;

protected:
    /// Thread Pool from the application server.
    /// Use this if you need a thread instead of spawning one yourself.
    Zen::Threading::ThreadPool*                         m_pThreadPool;
    /// @}
};  // template scriptable_generic_service

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
template<typename BaseClass_type, typename Class_type>
inline
scriptable_generic_service<BaseClass_type, Class_type>::scriptable_generic_service(Zen::Enterprise::AppServer::I_ApplicationServer& _appServer)
:	m_appServer(_appServer)
,	m_pHandlersMutex(Zen::Threading::MutexFactory::create())
{
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
template<typename BaseClass_type, typename Class_type>
inline
scriptable_generic_service<BaseClass_type, Class_type>::~scriptable_generic_service()
{
    Zen::Threading::MutexFactory::destroy(m_pHandlersMutex);
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
template<typename BaseClass_type, typename Class_type>
inline
void
scriptable_generic_service<BaseClass_type, Class_type>::setConfiguration(const Zen::Plugins::I_ConfigurationElement& _config)
{
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
template<typename BaseClass_type, typename Class_type>
inline
Zen::Threading::I_Condition*
scriptable_generic_service<BaseClass_type, Class_type>::prepareToStart(Zen::Threading::ThreadPool& _threadPool)
{
    m_pThreadPool = &_threadPool;

    // Ready to go, so don't bother returning a condition variable
    return NULL;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
template<typename BaseClass_type, typename Class_type>
inline
void
scriptable_generic_service<BaseClass_type, Class_type>::start()
{
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
template<typename BaseClass_type, typename Class_type>
inline
Zen::Threading::I_Condition*
scriptable_generic_service<BaseClass_type, Class_type>::prepareToStop()
{
    return NULL;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
template<typename BaseClass_type, typename Class_type>
inline
void
scriptable_generic_service<BaseClass_type, Class_type>::stop()
{
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
template<typename BaseClass_type, typename Class_type>
I_ApplicationServer&
scriptable_generic_service<BaseClass_type, Class_type>::getApplicationServer()
{
    return m_appServer;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
template<typename BaseClass_type, typename Class_type>
inline
void
scriptable_generic_service<BaseClass_type, Class_type>::handleMessage(pMessage_type _pMessage)
{
    // Check to see if it's a response
    Zen::Memory::managed_ptr<I_Response> pResponse(
        _pMessage.as<Zen::Memory::managed_ptr<I_Response> >());

    // If it is, handle it.
    if(pResponse.isValid())
    {
        pResponseHandler_type pHandler;

        // Find the handler
        {
            Zen::Threading::CriticalSection guard(m_pHandlersMutex);
            ResponseHandlers_type::iterator iter = m_responseHandlers.find(pResponse->getRequestMessageId());

            if(iter != m_responseHandlers.end())
            {
                pHandler = iter->second;

                m_responseHandlers.erase(iter);
            }
        }

        // If the handler was found, use it.
        if (pHandler.isValid())
        {
            // Dispatch the response
            pHandler->handleResponse(pResponse);
        }
        else
        {
            // Probably a duplicate response... just ignore it
            // Or maybe it was a message that timed out?
        }
    }
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
inline void destroyHandler(Memory::managed_weak_ptr<I_ResponseHandler> _pHandler)
{
    delete dynamic_cast<detail::handler_base*>(_pHandler.get());
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
template<typename BaseClass_type, typename Class_type>
template<typename Request_type, typename Payload_type>
inline
void
scriptable_generic_service<BaseClass_type, Class_type>::send(create_request<Request_type, Payload_type>& _request, boost::function<void(pResponse_type, Request_type&, Payload_type)> _function)
{
    detail::response_handler<Request_type, Payload_type>* pRawHandler = new detail::response_handler<Request_type, Payload_type>(_request.m_pRequest, _request.m_payload, _function);

    Memory::managed_ptr<I_ResponseHandler> pHandler(pRawHandler, destroyHandler);

    {
        Zen::Threading::CriticalSection guard(m_pHandlersMutex);
        m_responseHandlers[_request.m_pRawRequest->getMessageId()] = pHandler;
    }

    getApplicationServer().handleRequest(_request.m_pRequest, pHandler);
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
template<typename BaseClass_type, typename Class_type>
inline
void
scriptable_generic_service<BaseClass_type, Class_type>::handleRequest(pRequest_type _pRequest, pResponseHandler_type _pResponseHandler)
{
    ResponseHandlers_type::iterator iter = m_responseHandlers.find(_pRequest->getMessageId());
    if(iter == m_responseHandlers.end())
    {
        m_responseHandlers[_pRequest->getMessageId()] = _pResponseHandler;
    }

    _pRequest->getDestinationEndpoint()->getProtocolAdapter().lock()->sendTo(
        _pRequest,
        _pRequest->getDestinationEndpoint()
    );
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace AppServer
}   // namespace Enterprise
}   // namespace Zen
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

#endif // ZEN_APPSERVER_SCRIPTABLE_GENERIC_SERVICE_HPP_INCLUDED
