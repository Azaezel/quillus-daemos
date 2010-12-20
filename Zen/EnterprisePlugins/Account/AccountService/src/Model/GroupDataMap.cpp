//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
// Zen Enterprise Framework
//
// Copyright (C) 2001 - 2010 Tony Richards
// Copyright (C) 2008 - 2010 Matthew Alan Gray
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

#include "GroupDataMap.hpp"

#include "GroupDomainObject.hpp"
#include "GroupDataCollection.hpp"

#include <Zen/Core/Memory/managed_weak_ptr.hpp>

#include <Zen/Enterprise/Database/I_DatabaseStaticQuery.hpp>
#include <Zen/Enterprise/Database/I_DatabaseResult.hpp>
#include <Zen/Enterprise/Database/I_DatabaseColumn.hpp>
#include <Zen/Enterprise/Database/I_DatabaseRow.hpp>

#include <Zen/Core/Utility/runtime_exception.hpp>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Zen {
namespace Enterprise {
namespace Account {
namespace Model {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
GroupDataMap::GroupDataMap(pDatabaseConnection_type _pDatabaseConnection)
:   m_pDatabaseConnection(_pDatabaseConnection)
{
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
GroupDataMap::~GroupDataMap()
{
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
static void
destroyObject(Zen::Memory::managed_weak_ptr<I_GroupDomainObject> _wpObject);
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace {
struct FieldHandler
:   public Zen::Database::I_DatabaseRow::I_FieldVisitor
{
    virtual void begin()    {}

    virtual void visit(const Zen::Database::I_DatabaseColumn& _column, const boost::any& _value)
    {
        m_pValue->getElement(_column.getFieldName()).setAnyValue(_value);
    }

    virtual void end()      {}

    FieldHandler(GroupDataMap::pDomainObject_type _pValue)
    : m_pValue(_pValue)
    {
    }

    virtual ~FieldHandler()
    {
    }

private:
    GroupDataMap::pDomainObject_type       m_pValue;
};

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
struct RowHandler
:   public Zen::Database::I_DatabaseResult::I_ResultVisitor
{
    virtual void begin()
    {
    }

    virtual void visit(const Zen::Database::I_DatabaseRow& _databaseRow)
    {
        FieldHandler fieldHandler(m_pValue);

        _databaseRow.getFields(fieldHandler);
    }

    virtual void end()
    {
    }

    RowHandler(GroupDataMap::pDomainObject_type _pValue)
    : m_pValue(_pValue)
    {
    }

    virtual ~RowHandler()
    {
    }
    
private:
    GroupDataMap::pDomainObject_type           m_pValue;
};

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
class ResultHandler
:   public Zen::Database::I_DatabaseTransaction::I_QueryResultHandler
{
public:
    virtual void handleResult(pDatabaseResult_type _pResult)
    {
        RowHandler rowHandler(m_pValue);

        _pResult->getResults(rowHandler);

        m_pFutureValue->setValue(m_pValue);

        // Done, so commit the transaction.
        m_pTransaction->commit();
    }

    /// @param _pTransaction - We want to keep a reference to the transaction so it does 
    ///         not go out of scope until the results have been fully handled.
    ResultHandler(Zen::Database::I_DatabaseConnection::pDatabaseTransaction_type _pTransaction,
                 GroupDataMap::pFutureDomainObject_type _pFutureValue,
                 GroupDataMap::pDomainObject_type _pValue)
    :   m_pTransaction(_pTransaction)
    ,   m_pFutureValue(_pFutureValue)
    ,   m_pValue(_pValue)
    {
    }

    virtual ~ResultHandler()
    {
    }

private:
    Zen::Database::I_DatabaseConnection::pDatabaseTransaction_type  m_pTransaction;
    GroupDataMap::pFutureDomainObject_type     m_pFutureValue;
    GroupDataMap::pDomainObject_type           m_pValue;
};

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
struct CollectionRowHandler
:   public Zen::Database::I_DatabaseResult::I_ResultVisitor
{
    virtual void begin()
    {
    }

    virtual void visit(const Zen::Database::I_DatabaseRow& _databaseRow)
    {
        GroupDataMap::pDomainObject_type pValue(new GroupDomainObject, &destroyObject);
        
        m_pCollection->push_back(pValue);

        FieldHandler fieldHandler(pValue);

        _databaseRow.getFields(fieldHandler);
    }

    virtual void end()
    {
    }

    CollectionRowHandler(GroupDataMap::pDataCollection_type _pCollection)
    : m_pCollection(_pCollection)
    {
    }

    virtual ~CollectionRowHandler()
    {
    }
    
private:
    GroupDataMap::pDataCollection_type     m_pCollection;
};

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
class CollectionResultHandler
:   public Zen::Database::I_DatabaseTransaction::I_QueryResultHandler
{
public:
    virtual void handleResult(pDatabaseResult_type _pResult)
    {
        CollectionRowHandler rowHandler(m_pCollection);

        _pResult->getResults(rowHandler);

        m_pFutureCollection->setValue(m_pCollection);

        // Done, so commit the transaction.
        m_pTransaction->commit();
    }

    /// @param _pTransaction - We want to keep a reference to the transaction so it does 
    ///         not go out of scope until the results have been fully handled.
    CollectionResultHandler(Zen::Database::I_DatabaseConnection::pDatabaseTransaction_type _pTransaction,
                 GroupDataMap::pFutureDataCollection_type _pFutureCollection,
                 GroupDataMap::pDataCollection_type _pCollection)
    :   m_pTransaction(_pTransaction)
    ,   m_pFutureCollection(_pFutureCollection)
    ,   m_pCollection(_pCollection)
    {
    }

    virtual ~CollectionResultHandler()
    {
    }

private:
    Zen::Database::I_DatabaseConnection::pDatabaseTransaction_type  m_pTransaction;
    GroupDataMap::pFutureDataCollection_type     m_pFutureCollection;
    GroupDataMap::pDataCollection_type     m_pCollection;
};

} // namespace
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
static void
destroyObject(Zen::Memory::managed_weak_ptr<I_GroupDomainObject> _wpObject)
{
    GroupDomainObject* pObject = dynamic_cast<GroupDomainObject*>(_wpObject.get());
    if (pObject != NULL)
    {
        delete pObject;
    }
    else
    {
        // TODO Error?
    }
}
 
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
static void
destroyCollection(Zen::Memory::managed_weak_ptr<I_GroupDataCollection> _wpCollection)
{
    GroupDataCollection* pCollection = dynamic_cast<GroupDataCollection*>(_wpCollection.get());
    if (pCollection != NULL)
    {
        delete pCollection;
    }
    else
    {
        // TODO Error?
    }
}
 
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
GroupDataMap::pDomainObject_type 
GroupDataMap::createNew()
{
    return pDomainObject_type(new GroupDomainObject, &destroyObject);
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
GroupDataMap::pFutureDomainObject_type
GroupDataMap::getByKey(boost::uint64_t _primaryKeyValue)
{
    // Start a transaction
    Zen::Database::I_DatabaseConnection::pDatabaseTransaction_type 
        pTransaction = getDatabaseConnection()->beginTransaction();

    // Create a query
    Zen::Database::I_DatabaseTransaction::pStaticQuery_type 
        pQuery = pTransaction->createStaticQuery();

	const boost::uint64_t primaryKeyValue(_primaryKeyValue);

    // Get a record from the Account.
    std::stringstream query;
    query << "SELECT * FROM Group WHERE groupId = " << primaryKeyValue;

    pQuery->setQuery(query.str());
    
	pFutureDomainObject_type pReturnValue(new FutureDomainObject_type);
    pDomainObject_type pValue(new GroupDomainObject, &destroyObject);

    // Create a result handler to handle the execution results
    Zen::Database::I_DatabaseTransaction::pQueryResultHandler_type pResultHandler(new ResultHandler(pTransaction, pReturnValue, pValue));

    // Execute the query.  
    pTransaction->executeQuery(pQuery, pResultHandler);
    
    return pReturnValue;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
GroupDataMap::deleteByKey(boost::uint64_t _primaryKeyValue)
{
    // Don't do anything if the key value is 0.  But should this be an error?
    if (_primaryKeyValue == 0)
    {
        // TODO Throw an exception or return an error?
        return;
    }

    // Start a transaction
    Zen::Database::I_DatabaseConnection::pDatabaseTransaction_type 
        pTransaction = getDatabaseConnection()->beginTransaction();

    // Create a query
    Zen::Database::I_DatabaseTransaction::pStaticQuery_type 
        pQuery = pTransaction->createStaticQuery();

	const boost::uint64_t primaryKeyValue(_primaryKeyValue);

    // Delete a record in the Account.
    std::stringstream query;
    query << "DELETE FROM Group WHERE groupId = " << primaryKeyValue;

    pQuery->setQuery(query.str());

    // Execute the query.  
    pTransaction->executeQuery(pQuery);
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
Zen::Database::I_DatabaseTransaction::pFutureKey_type
GroupDataMap::update(pDomainObject_type _pDomainObject)
{
    // Start a transaction
    Zen::Database::I_DatabaseConnection::pDatabaseTransaction_type 
        pTransaction = getDatabaseConnection()->beginTransaction();

    // Create a query
    Zen::Database::I_DatabaseTransaction::pStaticQuery_type 
        pQuery = pTransaction->createStaticQuery();

    std::stringstream query;

    if (_pDomainObject->getGroupId().isNull())
    {
        // Construct the query to insert a record into the Account table.

        query << "INSERT into Group ( "
            << getNonPKFieldNames() << " ) VALUES ( ";

        // Append all of the values
        std::string escapedStringValue;
        
        // Final closing parenthesis
        query << ")";

        pQuery->setQuery(query.str());

        // Execute the query.
        Zen::Database::I_DatabaseTransaction::pFutureKey_type 
            pKey(pTransaction->executeInsert(pQuery));

        _pDomainObject->getGroupId() = pKey->getValue();
        
        // Done, so commit the transaction.
        pTransaction->commit();

        return pKey;
    }
    else
    {
        // Construct the query to update a record into the Account table.

        // Count the number of dirty fields.
        int dirtyFields = 0;

        query << "UPDATE Group SET ";

        std::string separator("");

        // Append all of the field names and values
        std::string escapedStringValue;

        // No need to proceed if none of the fields were dirty.
        if (dirtyFields > 0)
        {
            // WHERE clause to update the record
            query << " WHERE groupId = " << 
                _pDomainObject->getGroupId().getInt64Value();

            pQuery->setQuery(query.str());

            // Execute the query.  
            pTransaction->executeQuery(pQuery);

            // Done, so commit the transaction.
            pTransaction->commit();
        }
        
        Zen::Database::I_DatabaseTransaction::pFutureKey_type 
            pKey(new Zen::Database::I_DatabaseTransaction::FutureKey_type);

        pKey->setValue(_pDomainObject->getGroupId().getInt64Value());

        return pKey;
    }

}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
GroupDataMap::pFutureDataCollection_type
GroupDataMap::getAll()
{
    pFutureDataCollection_type pFutureCollection(new FutureDataCollection_type);
    pDataCollection_type pCollection(new GroupDataCollection, &destroyCollection);

    // Start a transaction
    Zen::Database::I_DatabaseConnection::pDatabaseTransaction_type 
        pTransaction = getDatabaseConnection()->beginTransaction();

    // Create a query
    Zen::Database::I_DatabaseTransaction::pStaticQuery_type 
        pQuery = pTransaction->createStaticQuery();

    // Get all of the records from the Account.
    pQuery->setQuery("SELECT * FROM Group");

    // Create a result handler to handle the execution results
    Zen::Database::I_DatabaseTransaction::pQueryResultHandler_type pResultHandler(new CollectionResultHandler(pTransaction, pFutureCollection, pCollection));

    // Execute the query.  
    pTransaction->executeQuery(pQuery, pResultHandler);    
    
    return pFutureCollection;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
        
std::string
GroupDataMap::escapeString(const std::string& _value) const
{
	return getDatabaseConnection()->escapeString(_value);
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
static std::string sm_fieldNames = "groupId";
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
static std::string sm_nonPKFieldNames = "";
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
const std::string&
GroupDataMap::getFieldNames()
{
    return sm_fieldNames;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
const std::string&
GroupDataMap::getNonPKFieldNames()
{
    return sm_nonPKFieldNames;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
static std::string sm_tableName = "Group";
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
const std::string&
GroupDataMap::getTableName()
{
    return sm_tableName;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
GroupDataMap::pDatabaseConnection_type
GroupDataMap::getDatabaseConnection() const
{
	return m_pDatabaseConnection;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace Model
}   // namespace Account
}   // namespace Enterprise
}   // namespace Zen
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
