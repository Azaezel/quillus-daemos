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
#ifndef ZEN_OBJECTMODEL_I_OBJECT_COLLECTION_HPP_INCLUDED
#define ZEN_OBJECTMODEL_I_OBJECT_COLLECTION_HPP_INCLUDED

#include "Configuration.hpp"

#include <Zen/Core/Memory/managed_ptr.hpp>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Zen {
namespace Spaces {
namespace ObjectModel {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
class I_Filter;
class I_View;
class I_Subscription;

class OBJECTMODEL_DLL_LINK I_ObjectCollection
:   boost::noncopyable
{
    /// @name Types
    /// @{
public:
    typedef Memory::managed_ptr<I_Filter>                   pFilter_type;
    typedef Memory::managed_ptr<I_View>                     pView_type;
    typedef Memory::managed_ptr<I_Subscription>             pSubscription_type;
    /// @}

    /// @name I_ObjectCollection interface.
    /// @{
public:
    /// Subscribe to the collection applying an optional filter.  
    /// The _pView is notified of all modifications to the data in this
    /// collection.
    /// @param _pFilter an optional data filter.  Pass an invalid pointer
    ///     in order to reset the current filter.
    /// @param _pView an optional data view.  Pass an invalid pointer in
    ///     order to reset the current view.
    virtual pSubscription_type subscribe(pView_type _pView, pFilter_type _pFilter = pFilter_type()) = 0;
    /// @}

    /// @name 'Structors
    /// @{
protected:
             I_ObjectCollection();
    virtual ~I_ObjectCollection();
    /// @}

};  // interface I_ObjectCollection

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace ObjectModel
}   // namespace Spaces
namespace Memory {
    // I_ObjectCollection is managed by factory
    template<>
    struct is_managed_by_factory<Zen::Spaces::ObjectModel::I_ObjectCollection> : public boost::true_type{};
}   // namespace Memory
}   // namespace Zen
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

#endif // ZEN_OBJECTMODEL_I_OBJECT_COLLECTION_HPP_INCLUDED
