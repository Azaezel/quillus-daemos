//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
// Zen Studio
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
/// @author Tony Richards trichards@indiezen.com
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
#ifndef ZENSTUDIO_WORKBENCHCOMMON_I_SPREAD_SHEET_DOCUMENT_FACTORY_HPP_INCLUDED
#define ZENSTUDIO_WORKBENCHCOMMON_I_SPREAD_SHEET_DOCUMENT_FACTORY_HPP_INCLUDED

#include "Configuration.hpp"

#include <Zen/Core/Plugins/I_ClassFactory.hpp>

#include <Zen/Core/Memory/managed_ptr.hpp>

#include <Zen/Studio/WorkbenchCommon/I_ExplorerNode.hpp>

#include <boost/cstdint.hpp>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Zen {
namespace Studio {
namespace Workbench {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
class I_ProjectExplorerController;
class I_ProjectDomainObject;
class I_ExplorerNodeDomainObject;
class I_Project;

/// Explorer Node User Data Factory.
/// This factory creates the user data for explorer nodes.
/// It's primarily used to load a project from the database.
class WORKBENCHCOMMON_DLL_LINK I_SpreadSheetDocumentFactory
:   public Plugins::I_ClassFactory
{
    /// @name Types
    /// @{
public:
    typedef Memory::managed_ptr<I_Project>                  pProject_type;
    typedef Memory::managed_ptr<I_ProjectDomainObject>      pProjectDomainObject_type;
    typedef Memory::managed_ptr<I_ExplorerNodeDomainObject> pExplorerNodeDomainObject_type;
    typedef I_ExplorerNode::pUserData_type                  pUserData_type;
    /// @}

    /// @name I_ExplorerNodeFactory interface
    /// @{
public:
    /// Create a spread sheet document user data.
    virtual pUserData_type createDocumentUserData(boost::uint64_t _explorerNodeId, const std::string& _documentType) = 0;
    /// @}

    /// @name 'Structors
    /// @{
protected:
             I_ExplorerNodeFactory();
    virtual ~I_ExplorerNodeFactory();
    /// @}

};  // interface I_ExplorerNodeFactory

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace Workbench
}   // namespace Studio
}   // namespace Zen
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

#endif // ZENSTUDIO_WORKBENCHCOMMON_I_EXPLORER_NODE_FACTORY_HPP_INCLUDED
