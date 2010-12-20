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
#ifndef ZEN_ENTERPRISE_SESSION_MODEL_I_SESSION_DOMAIN_OBJECT_HPP_INCLUDED
#define ZEN_ENTERPRISE_SESSION_MODEL_I_SESSION_DOMAIN_OBJECT_HPP_INCLUDED

#include "../Configuration.hpp"

#include <Zen/Core/Memory/managed_ptr.hpp>

#include <Zen/Enterprise/DataModel/I_DataElement.hpp>

#include <boost/noncopyable.hpp>
#include <boost/any.hpp>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Zen {
namespace Enterprise {
namespace Session {
namespace Model {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

class SESSION_SERVICE_DLL_LINK I_SessionDomainObject
:   public boost::noncopyable
{
    /// @name Types
    /// @{
public:
    typedef Zen::Memory::managed_ptr<Zen::Enterprise::DataModel::I_DataElement> pElement_type;
    typedef Zen::Enterprise::DataModel::I_DataElement&                          ElementReference_type;
    /// @}

    /// @name Getter / Setter methods
    /// @{
public:
    /// Get the SessionId element.
    /// Use this return value to check to see if the column is NULL in the database,
    /// or you can use it to modify the value instead of calling setSessionId().
    virtual ElementReference_type getSessionId() const = 0;

    /// Set the SessionId element value.
    /// This method does not directly update the database.
    /// You must use the I_SessionDataMap::update() method for the modifications
    /// to be persisted.
    virtual void setSessionId(const boost::any& _value) = 0;

    /// Get the AccountId element value.
    /// Use this return value to check to see if the column is NULL in the database,
    /// or you can use it to modify the value instead of calling setAccountId().
    virtual ElementReference_type getAccountId() const = 0;

    /// Set the AccountId element value.
    /// This method does not directly update the database.
    /// You must use the I_SessionDataMap::update() method for the modifications
    /// to be persisted.
    virtual void setAccountId(const boost::any& _value) = 0;

    /// Get the Endpoint element value.
    /// Use this return value to check to see if the column is NULL in the database,
    /// or you can use it to modify the value instead of calling setEndpoint().
    virtual ElementReference_type getEndpoint() const = 0;

    /// Set the Endpoint element value.
    /// This method does not directly update the database.
    /// You must use the I_SessionDataMap::update() method for the modifications
    /// to be persisted.
    virtual void setEndpoint(const boost::any& _value) = 0;

    /// Get the Location element value.
    /// Use this return value to check to see if the column is NULL in the database,
    /// or you can use it to modify the value instead of calling setLocation().
    virtual ElementReference_type getLocation() const = 0;

    /// Set the Location element value.
    /// This method does not directly update the database.
    /// You must use the I_SessionDataMap::update() method for the modifications
    /// to be persisted.
    virtual void setLocation(const boost::any& _value) = 0;

    /// Get the Status element value.
    /// Use this return value to check to see if the column is NULL in the database,
    /// or you can use it to modify the value instead of calling setStatus().
    virtual ElementReference_type getStatus() const = 0;

    /// Set the Status element value.
    /// This method does not directly update the database.
    /// You must use the I_SessionDataMap::update() method for the modifications
    /// to be persisted.
    virtual void setStatus(const boost::any& _value) = 0;

    /// Get the StartTime element value.
    /// Use this return value to check to see if the column is NULL in the database,
    /// or you can use it to modify the value instead of calling setStartTime().
    virtual ElementReference_type getStartTime() const = 0;

    /// Set the StartTime element value.
    /// This method does not directly update the database.
    /// You must use the I_SessionDataMap::update() method for the modifications
    /// to be persisted.
    virtual void setStartTime(const boost::any& _value) = 0;

    /// Get the EndTime element value.
    /// Use this return value to check to see if the column is NULL in the database,
    /// or you can use it to modify the value instead of calling setEndTime().
    virtual ElementReference_type getEndTime() const = 0;

    /// Set the EndTime element value.
    /// This method does not directly update the database.
    /// You must use the I_SessionDataMap::update() method for the modifications
    /// to be persisted.
    virtual void setEndTime(const boost::any& _value) = 0;

    virtual Zen::Enterprise::DataModel::I_DataElement& getElement(const std::string& _fieldName) = 0;
    /// @}

    /// @name 'Structors
    /// @{
protected:
             I_SessionDomainObject();
    virtual ~I_SessionDomainObject();
    /// @}

};  // interface I_SessionDomainObject

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace Model
}   // namespace Session
}   // namespace Enterprise
namespace Memory {
    /// I_SessionDomainObject is a managed by factory
    template<>
    struct is_managed_by_factory<Zen::Enterprise::Session::Model::I_SessionDomainObject> 
    :   public boost::true_type{};
}   // namespace Memory
}   // namespace Zen
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

#endif // ZEN_ENTERPRISE_SESSION_MODEL_I_SESSION_DOMAIN_OBJECT_HPP_INCLUDED
