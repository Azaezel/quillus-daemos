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
#ifndef ZEN_ENTERPRISE_SESSION_MODEL_I_SESSION_DATA_MAP_HPP_INCLUDED
#define ZEN_ENTERPRISE_SESSION_MODEL_I_SESSION_DATA_MAP_HPP_INCLUDED

#include "../Configuration.hpp"

#include <Zen/Core/Memory/managed_ptr.hpp>

#include <Zen/Enterprise/Database/I_DatabaseConnection.hpp>
#include <Zen/Enterprise/Database/I_DatabaseService.hpp>
#include <Zen/Enterprise/Database/I_DatabaseTransaction.hpp>

#include <boost/cstdint.hpp>

#include <boost/noncopyable.hpp>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Zen {
namespace Enterprise {
namespace Session {
namespace Model {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

class I_SessionDomainObject;
class I_SessionDataCollection;

class SESSION_SERVICE_DLL_LINK I_SessionDataMap
:   public boost::noncopyable
{
    /// @name Types
    /// @{
public:
    typedef Zen::Memory::managed_ptr<I_SessionDataMap>                  pDataMap_type;

    typedef Zen::Database::I_DatabaseService::pDatabaseConnection_type  pDatabaseConnection_type;

    typedef Zen::Memory::managed_ptr<I_SessionDataCollection>           pDataCollection_type;
    typedef Zen::Event::future_return_value<pDataCollection_type>       FutureDataCollection_type;
    typedef Zen::Memory::managed_ptr<FutureDataCollection_type>         pFutureDataCollection_type;

    typedef Zen::Memory::managed_ptr<I_SessionDomainObject>             pDomainObject_type;
    typedef Zen::Event::future_return_value<pDomainObject_type>         FutureDomainObject_type;
    typedef Zen::Memory::managed_ptr<FutureDomainObject_type>           pFutureDomainObject_type;
    /// @}

    /// @name I_SessionDataMap interface
    /// @{
public:
    /// Create a new domain object.
    virtual pDomainObject_type createNew() = 0;

    /// Get an object by primary key.
    virtual pFutureDomainObject_type getByKey(boost::uint64_t _primaryKeyValue) = 0;

    /// Delete an object by primary key.
    virtual void deleteByKey(boost::uint64_t _primaryKeyValue) = 0;

    /// Get all of the records.
    virtual pFutureDataCollection_type getAll() = 0;

    /// Update a domain object.
    /// If the domain object hasn't been inserted yet, it will be inserted.
    virtual Zen::Database::I_DatabaseTransaction::pFutureKey_type update(pDomainObject_type _pDomainObject) = 0;
    /// @}

    /// @name Static class factory
    /// @{
public:
    static pDataMap_type create(pDatabaseConnection_type _pDatabaseConnection);
    /// @}

    /// @name 'Structors
    /// @{
protected:
             I_SessionDataMap();
    virtual ~I_SessionDataMap();
    /// @}

};  // interface I_SessionDataMap

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace Model
}   // namespace Session
}   // namespace Enterprise
namespace Memory {
    /// I_SessionDataMap is a managed by factory
    template<>
    struct is_managed_by_factory<Zen::Enterprise::Session::Model::I_SessionDataMap> 
    :   public boost::true_type{};
}   // namespace Memory
}   // namespace Zen
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

#endif // ZEN_ENTERPRISE_SESSION_MODEL_I_SESSION_DATA_MAP_HPP_INCLUDED
