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
#ifndef ZEN_ENTERPRISE_SESSION_MODEL_SESSION_DOMAIN_OBJECT_HPP_INCLUDED
#define ZEN_ENTERPRISE_SESSION_MODEL_SESSION_DOMAIN_OBJECT_HPP_INCLUDED

#include "../../Model/I_SessionDomainObject.hpp"

#include <map>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Zen {
namespace Enterprise {
namespace Session {
namespace Model {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

class SessionDomainObject
:   public I_SessionDomainObject
{
    /// @name Types
    /// @{
public:
    /// @}

    /// @name Getter / Setter methods
    /// @{
public:
    virtual ElementReference_type getSessionId() const;
    virtual void setSessionId(const boost::any& _value);
    virtual ElementReference_type getAccountId() const;
    virtual void setAccountId(const boost::any& _value);
    virtual ElementReference_type getEndpoint() const;
    virtual void setEndpoint(const boost::any& _value);
    virtual ElementReference_type getLocation() const;
    virtual void setLocation(const boost::any& _value);
    virtual ElementReference_type getStatus() const;
    virtual void setStatus(const boost::any& _value);
    virtual ElementReference_type getStartTime() const;
    virtual void setStartTime(const boost::any& _value);
    virtual ElementReference_type getEndTime() const;
    virtual void setEndTime(const boost::any& _value);
    virtual Zen::Enterprise::DataModel::I_DataElement& getElement(const std::string& _fieldName);
    /// @}

    /// @name 'Structors
    /// @{
public:
             SessionDomainObject();
    virtual ~SessionDomainObject();
    /// @}

    /// @name Member Variables
    /// @{
private:
    pElement_type                                   m_pSessionId;
    pElement_type                                   m_pAccountId;
    pElement_type                                   m_pEndpoint;
    pElement_type                                   m_pLocation;
    pElement_type                                   m_pStatus;
    pElement_type                                   m_pStartTime;
    pElement_type                                   m_pEndTime;

    typedef std::map<std::string, pElement_type>    FieldElementMap_type;
    FieldElementMap_type                            m_elements;
    /// @}

};  // class SessionDomainObject

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace Model
}   // namespace Session
}   // namespace Enterprise
}   // namespace Zen
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

#endif // ZEN_ENTERPRISE_SESSION_MODEL_SESSION_DOMAIN_OBJECT_HPP_INCLUDED
