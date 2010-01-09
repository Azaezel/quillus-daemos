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
//  Matthew Alan Gray mgray@indiezen.org
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
#include "SessionService.hpp"
#include "Session.hpp"
#include "Attribute.hpp"

#include <Zen/Core/Utility/runtime_exception.hpp>

#include <Zen/Core/Threading/I_Mutex.hpp>
#include <Zen/Core/Threading/MutexFactory.hpp>
#include <Zen/Core/Threading/CriticalSection.hpp>

#include <Zen/Enterprise/Networking/I_Endpoint.hpp>

#include <Zen/Enterprise/AppServer/I_Request.hpp>
#include <Zen/Enterprise/AppServer/I_ResponseHandler.hpp>
#include <Zen/Enterprise/AppServer/I_ProtocolService.hpp>

#include <Zen/Enterprise/Database/I_DatabaseManager.hpp>
#include <Zen/Enterprise/Database/I_DatabaseService.hpp>
#include <Zen/Enterprise/Database/I_DatabaseConnection.hpp>
#include <Zen/Enterprise/Database/I_DatabaseStaticQuery.hpp>
#include <Zen/Enterprise/Database/I_DatabaseTransaction.hpp>
#include <Zen/Enterprise/Database/I_DatabaseResult.hpp>
#include <Zen/Enterprise/Database/I_DatabaseColumn.hpp>
#include <Zen/Enterprise/Database/I_DatabaseRow.hpp>

#include <Zen/Community/SessionProtocol/I_SessionProtocolManager.hpp>
#include <Zen/Community/SessionProtocol/I_LoginRequest.hpp>
#include <Zen/Community/SessionProtocol/I_LoginResponse.hpp>
#include <Zen/Community/SessionProtocol/I_DataRequest.hpp>
#include <Zen/Community/SessionProtocol/I_DataResponse.hpp>

#include <Zen/Community/SessionCommon/I_Session.hpp>
#include <Zen/Community/SessionCommon/I_Attribute.hpp>

#include <boost/bind.hpp>

#include <iostream>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Zen {
namespace Community {
namespace Server {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
struct ColumnHandler
:   Zen::Database::I_DatabaseResult::I_ColumnVisitor
{
    virtual void begin()
    {
    }

    virtual void visit(const Zen::Database::I_DatabaseColumn& _databaseColumn)
    {
    }

    virtual void end()
    {
    }

};  // struct ColumnHandler
    
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
struct SessionFieldHandler
:   Zen::Database::I_DatabaseRow::I_FieldVisitor
{
    virtual void begin()
    {
    }

    virtual void visit(const Zen::Database::I_DatabaseColumn& _column, const boost::any& _value)
    {
        std::cout << "DEBUG: User " << boost::any_cast<std::string>(_value) << " has successfully logged in." << std::endl;
    }

    virtual void end()
    {
    }

};  // struct SessionFieldHandler

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
struct DataFieldHandler
:   Zen::Database::I_DatabaseRow::I_FieldVisitor
{
    virtual void begin()
    {
    }

    virtual void visit(const Zen::Database::I_DatabaseColumn& _column, const boost::any& _value)
    {
        if( _column.getFieldName() == "key" )
        {
            m_attribute.setKey(boost::any_cast<std::string>(_value));
        }
        else
        if( _column.getFieldName() == "value" )
        {
            m_attribute.setValue(boost::any_cast<std::string>(_value));
        }
    }

    virtual void end()
    {
    }

    Attribute& getAttribute()
    {
        return m_attribute;
    }

private:
    Attribute   m_attribute;

};  // struct DataFieldHandler

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
struct SessionRowHandler
:   Zen::Database::I_DatabaseResult::I_ResultVisitor
{
    virtual void begin()
    {
        m_success = false;
    }

    virtual void visit(const Zen::Database::I_DatabaseRow& _databaseRow)
    {
        SessionFieldHandler fieldHandler;

        _databaseRow.getFields(fieldHandler);

        m_success = true;
    }

    virtual void end()
    {
    }

    bool isSuccess()
    {
        return m_success;
    }

private:
    bool    m_success;

};  // struct SessionRowHandler

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
struct DataRowHandler
:   Zen::Database::I_DatabaseResult::I_ResultVisitor
{
    virtual void begin()
    {
        m_success = false;
    }

    virtual void visit(const Zen::Database::I_DatabaseRow& _databaseRow)
    {
        _databaseRow.getFields(m_fieldHandler);

        m_success = true;
    }

    virtual void end()
    {
    }

    bool isSuccess()
    {
        return m_success;
    }

    Attribute& getAttribute()
    {
        return m_fieldHandler.getAttribute();
    }

private:
    bool                m_success;
    DataFieldHandler    m_fieldHandler;

};  // struct DataRowHandler

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
class SessionResultHandler
:   public Zen::Database::I_DatabaseTransaction::I_QueryResultHandler
{
public:

    typedef Zen::Memory::managed_ptr<Zen::Networking::I_Endpoint>   pEndpoint_type;

    virtual void handleResult(pDatabaseResult_type _pResult)
    {
        ColumnHandler columnHandler;
        SessionRowHandler rowHandler;

        _pResult->getColumns(columnHandler);
        _pResult->getResults(rowHandler);

        // Done, so commit the transaction
        m_pTransaction->commit();

        // Get the source endpoint so we can send the response back
		Protocol::I_LoginRequest::pEndpoint_type pEndpoint = m_request.getSourceEndpoint();

        if( rowHandler.isSuccess() )
        {
            typedef Zen::Enterprise::AppServer::I_ProtocolService::pMessage_type pMessage_type;
            pEndpoint->getProtocolAdapter().lock()->sendTo(Protocol::I_LoginResponse::create(pEndpoint_type().as<pMessage_type>(), pEndpoint, m_request.getRequestId(), Protocol::I_LoginResponse::SUCCESS, "Session successful."), pEndpoint);
        }
        else
        {
            pEndpoint->getProtocolAdapter().lock()->sendTo(Protocol::I_LoginResponse::create(pEndpoint_type(), pEndpoint, m_request.getRequestId(), Protocol::I_LoginResponse::FAIL, "Session unsuccessful."), pEndpoint);
        }
    }

    /// @param _pTransaction - We want to keep a reference to the transaction so it does 
    ///         not go out of scope until the results have been fully handled.
    SessionResultHandler(Protocol::I_LoginRequest& _request,
                       Zen::Database::I_DatabaseConnection::pDatabaseTransaction_type _pTransaction)
    :   m_request(_request)
    ,   m_pTransaction(_pTransaction)
    {
    }

    virtual ~SessionResultHandler()
    {
    }

private:
    Protocol::I_LoginRequest&                                       m_request;
    Zen::Database::I_DatabaseConnection::pDatabaseTransaction_type  m_pTransaction;
};  // class SessionResultHandler

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
class DataResultHandler
:   public Zen::Database::I_DatabaseTransaction::I_QueryResultHandler
{
public:

    typedef Zen::Memory::managed_ptr<Zen::Networking::I_Endpoint>   pEndpoint_type;

    virtual void handleResult(pDatabaseResult_type _pResult)
    {
        ColumnHandler columnHandler;
        DataRowHandler rowHandler;

        _pResult->getColumns(columnHandler);
        _pResult->getResults(rowHandler);

        // Done, so commit the transaction
        m_pTransaction->commit();

        // Get the source endpoint so we can send the response back
		Protocol::I_LoginRequest::pEndpoint_type pEndpoint = m_request.getSourceEndpoint();

        if( rowHandler.isSuccess() )
        {
            pEndpoint->getProtocolAdapter().lock()->sendTo(Protocol::I_DataResponse::create(pEndpoint_type(), pEndpoint, m_request.getRequestId(), Protocol::I_DataResponse::SUCCESS, rowHandler.getAttribute()), pEndpoint);
        }
        else
        {
            pEndpoint->getProtocolAdapter().lock()->sendTo(Protocol::I_DataResponse::create(pEndpoint_type(), pEndpoint, m_request.getRequestId(), Protocol::I_DataResponse::FAIL, rowHandler.getAttribute()), pEndpoint);
        }
    }

    /// @param _pTransaction - We want to keep a reference to the transaction so it does 
    ///         not go out of scope until the results have been fully handled.
    DataResultHandler(Protocol::I_DataRequest& _request,
                       Zen::Database::I_DatabaseConnection::pDatabaseTransaction_type _pTransaction)
    :   m_request(_request)
    ,   m_pTransaction(_pTransaction)
    {
    }

    virtual ~DataResultHandler()
    {
    }

private:
    Protocol::I_DataRequest&                                        m_request;
    Zen::Database::I_DatabaseConnection::pDatabaseTransaction_type  m_pTransaction;
};  // class DataResultHandler

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
SessionService::SessionService(Zen::Enterprise::AppServer::I_ApplicationServer& _appServer)
:   m_appServer(_appServer)
,   m_pThreadPool(NULL)
,   m_pHandlersMutex(Zen::Threading::MutexFactory::create())
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
    pConfig_type pDatabaseConfig = _config.getChild("database");
    m_databaseName = pDatabaseConfig->getAttribute("name");

    pConfig_type pDatabaseTypeConfig = pDatabaseConfig->getChild("type");
    m_databaseType = pDatabaseTypeConfig->getAttribute("name");

    DatabaseConfigVisitor visitor(*this);
    pDatabaseConfig->getChildren("config",visitor);
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
Zen::Threading::I_Condition*
SessionService::prepareToStart(Zen::Threading::ThreadPool& _threadPool)
{
    m_pThreadPool = &_threadPool;

    Zen::Community::Protocol::I_SessionProtocolManager::getSingleton().install(m_appServer);

    m_pDatabaseService = Zen::Database::I_DatabaseManager::getSingleton()
        .createDatabaseService(
            m_databaseType, 
            m_databaseConfig
        );

    if( !m_pDatabaseService.isValid() )
    {
        throw Zen::Utility::runtime_exception("SessionService::prepareToStart() : Database service could not be loaded.");
    }

    // Ready to go, so don't bother returning a condition variable
    return NULL;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
SessionService::start()
{
    m_pDatabaseConnection = m_pDatabaseService->connect(m_databaseName, m_databaseConfig);

    if( !m_pDatabaseConnection )
    {
        throw Zen::Utility::runtime_exception("SessionService::start() : Connection to database could not be established.");
    }
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
Zen::Threading::I_Condition*
SessionService::prepareToStop()
{
    if( m_pDatabaseConnection != NULL )
    {
        m_pDatabaseConnection.reset();
    }

    return NULL;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
SessionService::stop()
{
    m_pDatabaseService.reset();
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
SessionService::handleMessage(pMessage_type _pMessage)
{
	Protocol::I_LoginRequest* pLoginRequest = dynamic_cast<Protocol::I_LoginRequest*>(_pMessage.get());

    if( pLoginRequest != NULL )
    {
        // Start a transaction
        Zen::Database::I_DatabaseConnection::pDatabaseTransaction_type pTransaction = 
            m_pDatabaseConnection->beginTransaction();

        // Create a query
        Zen::Database::I_DatabaseTransaction::pStaticQuery_type pQuery = 
            pTransaction->createStaticQuery();

        std::stringstream query;
        query << "SELECT userId FROM Account WHERE ";
        query << "userId = '" << pLoginRequest->getUserId() << "' AND ";
        query << "password = '" << pLoginRequest->getPassword() << "'";

        // Set the query - select all of the records from the User table
        pQuery->setQuery(query.str());

        // Create a result handler to handle the execution results
        Zen::Database::I_DatabaseTransaction::pQueryResultHandler_type pResultHandler(new SessionResultHandler(*pLoginRequest, pTransaction));

        // Execute the query.  
        // TR - Is this asynchronous?  What should it be?
        pTransaction->executeQuery(pQuery, pResultHandler);

        return;
    }

    Protocol::I_DataRequest* pDataRequest = dynamic_cast<Protocol::I_DataRequest*>(_pMessage.get());

    if( pDataRequest != NULL )
    {
        // Start a transaction
        Zen::Database::I_DatabaseConnection::pDatabaseTransaction_type pTransaction = 
            m_pDatabaseConnection->beginTransaction();

        // Create a query
        Zen::Database::I_DatabaseTransaction::pStaticQuery_type pQuery = 
            pTransaction->createStaticQuery();

        std::stringstream query;
        query << "SELECT key,value FROM Properties WHERE ";
        query << "key = '" << pDataRequest->getKey() << "'";

        // Set the query - select all of the records from the Properties table
        pQuery->setQuery(query.str());

        // Create a result handler to handle the execution results
        Zen::Database::I_DatabaseTransaction::pQueryResultHandler_type pResultHandler(new DataResultHandler(*pDataRequest, pTransaction));

        // Execute the query.  
        // TR - Is this asynchronous?  What should it be?
        pTransaction->executeQuery(pQuery, pResultHandler);

        return;
    }

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

    // TODO Error?
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

        pEndpoint_type pEndpoint = _pRequest->getDestinationEndpoint();

        if( pEndpoint.isValid() )
        {
            _pRequest->getDestinationEndpoint()->getProtocolAdapter().lock()->sendTo(
                _pRequest,
                _pRequest->getDestinationEndpoint()
            );
        }
        else
        {
            handleMessage(_pRequest.as<pMessage_type>());
        }
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
	throw Zen::Utility::runtime_exception("Not implemented");
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
             LoginResponseHandler(SessionService::pService_type _pParent) : m_pParent(_pParent) {}
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

        Attribute* pRawAttribute = new Attribute();
        pRawAttribute->setKey(pResponse->getKey());
        pRawAttribute->setValue(pResponse->getValue());

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
void
SessionService::addConfig(const Zen::Plugins::I_ConfigurationElement& _element)
{
    m_databaseConfig[_element.getAttribute("name")] = _element.getAttribute("value");
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
SessionService::DatabaseConfigVisitor::DatabaseConfigVisitor(Zen::Community::Server::SessionService& _service)
:   m_service(_service)
{
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
SessionService::DatabaseConfigVisitor::~DatabaseConfigVisitor()
{
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
SessionService::DatabaseConfigVisitor::begin()
{
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
SessionService::DatabaseConfigVisitor::visit(const Zen::Plugins::I_ConfigurationElement& _element)
{
    m_service.addConfig(_element);
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
SessionService::DatabaseConfigVisitor::end()
{
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace Server
}   // namespace Community
}   // namespace Zen
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
