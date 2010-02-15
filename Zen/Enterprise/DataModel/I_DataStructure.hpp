//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
// Zen Enterprise Framework
//
// Copyright (C) 2001 - 2009 Tony Richards
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
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
#ifndef ZEN_DATAMODEL_I_DATA_STRUCTURE_HPP_INCLUDED
#define ZEN_DATAMODEL_I_DATA_STRUCTURE_HPP_INCLUDED

#include "Configuration.hpp"

#include <Zen/Core/Memory/managed_ptr.hpp>

#include <boost/noncopyable.hpp>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Zen {
namespace Enterprise {
namespace DataModel {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
;

class DATAMODEL_DLL_LINK I_DataStructure
:   boost::noncopyable
{
    /// @name Types
    /// @{
public:
    /// @}

    /// @name I_DataStructure interface.
    /// @{
public:
    /// Refresh the data from the data source.
    virtual void refreshDataFromSource() = 0;

    /// Commit the data to the data source.
    virtual void commitDataToSource() = 0;

    /// Revert any changes made to the elements in
    /// this structure.
    virtual void revertData() = 0;
    /// @}

    /// @name Events
    /// @{
public:
    /// @}

    /// @name 'Structors
    /// @{
protected:
             I_DataStructure();
    virtual ~I_DataStructure();
    /// @}

};  // interface I_DataStructure

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace DataModel
}   // namespace Enterprise
namespace Memory {
    // I_DataStructure is managed by factory
    template<>
    struct is_managed_by_factory<Zen::Enterprise::DataModel::I_DataStructure> : public boost::true_type{};
}   // namespace Memory
}   // namespace Zen
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

#endif // ZEN_DATAMODEL_I_DATA_STRUCTURE_HPP_INCLUDED
