//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
// Zen Spaces
//
// Copyright (C) 2001 - 2009 Tony Richards
//
// Licensed under the Games by Sarge Publishing License - See your licensing
// agreement for terms and conditions.
//
// Do not redistribute this source code.
//
// Tony Richards trichards@gamesbysarge.com
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
#ifndef ZEN_OBJECTMODEL_I_OBJECT_HPP_INCLUDED
#define ZEN_OBJECTMODEL_I_OBJECT_HPP_INCLUDED

#include "Configuration.hpp"

#include <Zen/Core/Memory/managed_ptr.hpp>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Zen {
namespace Spaces {
namespace ObjectModel {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
class I_ObjectType;
class I_ObjectCollection;
class I_Filter;
class I_Subscription;
class I_View;

class OBJECTMODEL_DLL_LINK I_Object
:   boost::noncopyable
{
    /// @name Types
    /// @{
public:
    typedef Memory::managed_ptr<I_ObjectType>               pObjectType_type;
    typedef Memory::managed_ptr<I_View>                     pView_type;
    typedef Memory::managed_ptr<I_Filter>                   pFilter_type;
    typedef Memory::managed_ptr<I_ObjectCollection>         pObjectCollection_type;
    typedef Memory::managed_ptr<I_Subscription>             pSubscription_type;
    /// @}

    /// @name I_Object interface.
    /// @{
public:
    /// Get the child objects
    virtual pObjectCollection_type getChildObjects(pObjectType_type _pType, pFilter_type _pFilter) = 0;
    /// @}

    /// @name 'Structors
    /// @{
protected:
             I_Object();
    virtual ~I_Object();
    /// @}

};  // interface I_Object

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace ObjectModel
}   // namespace Spaces
namespace Memory {
    // I_Object is managed by factory
    template<>
    struct is_managed_by_factory<Zen::Spaces::ObjectModel::I_Object> : public boost::true_type{};
}   // namespace Memory
}   // namespace Zen
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

#endif // ZEN_OBJECTMODEL_I_OBJECT_HPP_INCLUDED
