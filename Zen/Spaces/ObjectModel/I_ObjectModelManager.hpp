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
#ifndef ZEN_OBJECTMODEL_I_OBJECT_MODEL_MANAGER_HPP_INCLUDED
#define ZEN_OBJECTMODEL_I_OBJECT_MODEL_MANAGER_HPP_INCLUDED

#include "Configuration.hpp"

#include <Zen/Core/Memory/managed_ptr.hpp>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Zen {
namespace Spaces {
namespace ObjectModel {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
class I_DataSource;
class I_ObjectCollection;

class OBJECTMODEL_DLL_LINK I_ObjectModelManager
:   boost::noncopyable
{
    /// @name Types
    /// @{
public:
    typedef Memory::managed_ptr<I_ObjectCollection>         pObjectCollection_type;
    typedef Memory::managed_ptr<I_DataSource>               pDataSource_type;
    /// @}

    /// @name I_ObjectModelManager interface.
    /// @{
public:
    /// Get the objects for a subject area given a data provider.
    virtual pObjectCollection_type getObjects(const std::string& _subjectArea, pDataSource_type _pDataSource) = 0;
    /// @}

    /// @name Static methods
    /// @{
public:
    /// Get the one and only object model manager.
    static I_ObjectModelManager& getSingleton();
    /// @}

    /// @name 'Structors
    /// @{
protected:
             I_ObjectModelManager();
    virtual ~I_ObjectModelManager();
    /// @}

};  // interface I_Object

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace ObjectModel
}   // namespace Spaces
}   // namespace Zen
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

#endif // ZEN_OBJECTMODEL_I_OBJECT_MODEL_MANAGER_HPP_INCLUDED
