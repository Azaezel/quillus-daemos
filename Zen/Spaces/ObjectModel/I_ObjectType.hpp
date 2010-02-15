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
#ifndef ZEN_OBJECTMODEL_I_OBJECT_TYPE_HPP_INCLUDED
#define ZEN_OBJECTMODEL_I_OBJECT_TYPE_HPP_INCLUDED

#include "Configuration.hpp"

#include <Zen/Core/Memory/managed_ptr.hpp>

#include <string>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Zen {
namespace Spaces {
namespace ObjectModel {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
class I_ObjectType;

class OBJECTMODEL_DLL_LINK I_ObjectType
:   boost::noncopyable
{
    /// @name Types
    /// @{
public:
    typedef Memory::managed_ptr<I_ObjectType>               pObjectType_type;
    /// @}

    /// @name I_ObjectType interface.
    /// @{
public:
    /// Get the object definition for a child object.
    virtual pObjectType_type getChildObjectType(const std::string& _name) = 0;
    /// @}

    /// @name 'Structors
    /// @{
protected:
             I_ObjectType();
    virtual ~I_ObjectType();
    /// @}

};  // interface I_ObjectType

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace ObjectModel
}   // namespace Spaces
namespace Memory {
    // I_ObjectType is managed by factory
    template<>
    struct is_managed_by_factory<Zen::Spaces::ObjectModel::I_ObjectType> : public boost::true_type{};
}   // namespace Memory
}   // namespace Zen
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

#endif // ZEN_OBJECTMODEL_I_OBJECT_TYPE_HPP_INCLUDED
