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
#ifndef ZEN_ENTERPRISE_ACCOUNT_MODEL_I_GROUP_DATA_COLLECTION_HPP_INCLUDED
#define ZEN_ENTERPRISE_ACCOUNT_MODEL_I_GROUP_DATA_COLLECTION_HPP_INCLUDED

#include "../Configuration.hpp"

#include <Zen/Core/Memory/managed_ptr.hpp>

#include <boost/noncopyable.hpp>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Zen {
namespace Enterprise {
namespace Account {
namespace Model {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

class I_GroupDomainObject;

class ACCOUNT_SERVICE_DLL_LINK I_GroupDataCollection
:   public boost::noncopyable
{
    /// @name Types
    /// @{
public:
    typedef Zen::Memory::managed_ptr<I_GroupDomainObject> pDomainObject_type;

    /// Declared below
    struct I_CollectionVisitor;
    /// @}

    /// @name I_GroupDataCollection interface.
    /// @{
public:
    /// Push a domain object onto the end of the collection.
    /// @note This is normally only used by the DomainMap implementation.
    virtual void push_back(pDomainObject_type _pDomainObject) = 0;

    /// Get all of the domain objects in this collection using a visitor.
    virtual void getAll(I_CollectionVisitor& _visitor) = 0;

    /// @return true if this collection is empty.
    virtual bool isEmpty() const = 0;
    /// @}

    /// @name Inner structs
    /// @{
public:
    struct I_CollectionVisitor
    {
        virtual void begin() = 0;
        virtual void visit(pDomainObject_type _pDomainObject) = 0;
        virtual void end() = 0;
    };  // interface I_CollectionVisitor
    /// @}

    /// @name 'Structors
    /// @{
protected:
             I_GroupDataCollection();
    virtual ~I_GroupDataCollection();
    /// @}

};  // interface I_GroupDataCollection

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace Model
}   // namespace Account
}   // namespace Enterprise
namespace Memory {
    /// I_GroupDataCollection is a managed by factory
    template<>
    struct is_managed_by_factory<Zen::Enterprise::Account::Model::I_GroupDataCollection> 
    :   public boost::true_type{};
}   // namespace Memory
}   // namespace Zen
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

#endif // ZEN_ENTERPRISE_ACCOUNT_MODEL_I_GROUP_DATA_COLLECTION_HPP_INCLUDED
