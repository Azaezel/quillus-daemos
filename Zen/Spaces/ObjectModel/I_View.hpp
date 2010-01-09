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
#ifndef ZEN_OBJECTMODEL_I_VIEW_HPP_INCLUDED
#define ZEN_OBJECTMODEL_I_VIEW_HPP_INCLUDED

#include "Configuration.hpp"

#include <Zen/Core/Memory/managed_ptr.hpp>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Zen {
namespace Spaces {
namespace ObjectModel {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

class I_Object;

class OBJECTMODEL_DLL_LINK I_View
:   boost::noncopyable
{
    /// @name Types
    /// @{
public:
    typedef Memory::managed_ptr<I_Object>   pObject_type;
    /// @}

    /// @name I_View interface.
    /// @{
public:
    /// An object was added to an object collection.
    virtual void onObjectCreated(pObject_type _pObject) = 0;

    /// An object was removed from an object collection.
    virtual void onObjectDeleted(pObject_type _pObject) = 0;

    /// This object or a child object was modified.
    /// During this call, the elements will have the wasModified() flag
    /// set to true if that particular element was modified.  After
    /// the end of the execution of this method, all wasModified() flags
    /// will be reset to false.
    virtual void onObjectModified(pObject_type _pObject) = 0;

    /// This method is called when the object is initially attached to
    /// a view.
    virtual void onObjectRefreshed(pObject_type _pObject) = 0;
    /// @}

    /// @name 'Structors
    /// @{
protected:
             I_View();
    virtual ~I_View();
    /// @}

};  // interface I_View

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace ObjectModel
}   // namespace Spaces
namespace Memory {
    // I_View is managed by factory
    template<>
    struct is_managed_by_factory<Zen::Spaces::ObjectModel::I_View> : public boost::true_type{};
}   // namespace Memory
}   // namespace Zen
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

#endif // ZEN_OBJECTMODEL_I_VIEW_HPP_INCLUDED
