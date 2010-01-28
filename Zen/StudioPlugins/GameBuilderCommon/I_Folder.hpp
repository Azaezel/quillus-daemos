//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
// Game Builder
//
// Copyright (C) 2001 - 2009 Tony Richards
// Copyright (C) 2008 - 2009 Matthew Alan Gray
//
// Licensed under the Games by Sarge Publishing License - See your licensing
// agreement for terms and conditions.
//
// Do not redistribute this source code.
//
// Tony Richards trichards@gamesbysarge.com
// Matthew Alan Gray mgray@indiezen.org
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
#ifndef GAMEBUILDER_I_FOLDER_HPP_INCLUDED
#define GAMEBUILDER_I_FOLDER_HPP_INCLUDED

#include "Configuration.hpp"

#include <Zen/Studio/WorkbenchCommon/I_ExplorerNode.hpp>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Zen {
    namespace Studio {
        namespace Workbench {
            class I_ExplorerNodeType;
            class I_ExplorerNodeActions;
        }   // namespace Workbench
    }   // namespace Studio
}   // namespace Zen
namespace GameBuilder {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
class I_Project;

/// A folder inside of a project
class GAMEBUILDERCOMMON_DLL_LINK I_Folder
:   public Zen::Studio::Workbench::I_ExplorerNode::UserData
{
    /// @name Types
    /// @{
public:
    typedef Zen::Memory::managed_ptr<Zen::Studio::Workbench::I_ExplorerNodeActions> pExplorerNodeActions_type;
    /// @}

    /// @name I_Folder interface
    /// @{
public:
    /// Create a new object that is a child of the selected node.
    virtual void newObjectType(Zen::Studio::Workbench::I_ExplorerNode& _selectedNode) = 0;
    /// @}

    /// @name Static method
    /// @{
public:
    static Zen::Studio::Workbench::I_ExplorerNodeType& getNodeType();
    /// @}

    /// @name 'Structors
    /// @{
protected:
             I_Folder(const std::string& _name);
    virtual ~I_Folder();
    /// @}

};  // interface I_Folder

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace GameBuilder
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

#endif // GAMEBUILDER_I_FOLDER_HPP_INCLUDED
