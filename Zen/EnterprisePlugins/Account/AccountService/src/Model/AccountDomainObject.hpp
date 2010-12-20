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
#ifndef ZEN_ENTERPRISE_ACCOUNT_MODEL_ACCOUNT_DOMAIN_OBJECT_HPP_INCLUDED
#define ZEN_ENTERPRISE_ACCOUNT_MODEL_ACCOUNT_DOMAIN_OBJECT_HPP_INCLUDED

#include "../../Model/I_AccountDomainObject.hpp"

#include <map>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Zen {
namespace Enterprise {
namespace Account {
namespace Model {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

class AccountDomainObject
:   public I_AccountDomainObject
{
    /// @name Types
    /// @{
public:
    /// @}

    /// @name Getter / Setter methods
    /// @{
public:
    virtual ElementReference_type getAccountId() const;
    virtual void setAccountId(const boost::any& _value);
    virtual ElementReference_type getUserId() const;
    virtual void setUserId(const boost::any& _value);
    virtual ElementReference_type getPassword() const;
    virtual void setPassword(const boost::any& _value);
    virtual Zen::Enterprise::DataModel::I_DataElement& getElement(const std::string& _fieldName);
    /// @}

    /// @name 'Structors
    /// @{
public:
             AccountDomainObject();
    virtual ~AccountDomainObject();
    /// @}

    /// @name Member Variables
    /// @{
private:
    pElement_type                                   m_pAccountId;
    pElement_type                                   m_pUserId;
    pElement_type                                   m_pPassword;

    typedef std::map<std::string, pElement_type>    FieldElementMap_type;
    FieldElementMap_type                            m_elements;
    /// @}

};  // class AccountDomainObject

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace Model
}   // namespace Account
}   // namespace Enterprise
}   // namespace Zen
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

#endif // ZEN_ENTERPRISE_ACCOUNT_MODEL_ACCOUNT_DOMAIN_OBJECT_HPP_INCLUDED
