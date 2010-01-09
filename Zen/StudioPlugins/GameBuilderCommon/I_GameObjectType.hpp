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
#ifndef GAME_BUILDER_I_GAME_OBJECT_TYPE_HPP_INCLUDED
#define GAME_BUILDER_I_GAME_OBJECT_TYPE_HPP_INCLUDED

#include "Configuration.hpp"

#include <Zen/Studio/WorkbenchCommon/I_ExplorerNode.hpp>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Zen {
    namespace Studio {
        namespace Workbench {
            class I_ExplorerNodeType;
            class I_ExplorerNodeActions;
            class I_Document;
        }   // namespace Workbench
    }   // namespace Studio
}   // namespace Zen
namespace GameBuilder {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
;

/// Game Object Type explorer node user data interface
/// @note Possibly should rename this to I_GameObjectTypeNode or 
///     I_GameObjectTypeExplorerNode but for now I'll leave it as-is.
class GAMEBUILDERCOMMON_DLL_LINK I_GameObjectType
:   public Zen::Studio::Workbench::I_ExplorerNode::UserData
{
    /// @name Types
    /// @{
public:
    typedef Zen::Memory::managed_ptr<Zen::Studio::Workbench::I_Document>            pDocument_type;
    typedef Zen::Memory::managed_ptr<Zen::Studio::Workbench::I_ExplorerNodeActions> pExplorerNodeActions_type;
    /// @}

    /// @name I_GameObjectType interface
    /// @{
public:
    /// Get the document associated with this Game Object Type.
    /// @todo TR - This should probably be a pFutureDocument_type instead of a 
    ///     pDocument_type.  If the document has not been loaded yet, it will 
    ///     likely take awhile to load.
    virtual pDocument_type getDocument() = 0;

    /// Create a new object that is a child of the selected node.
    virtual void addChild(Zen::Studio::Workbench::I_ExplorerNode& _selectedNode) = 0;

    /// Get the database primary key for this game object type.
    virtual boost::uint64_t getGameObjectTypeId() const = 0;
    /// @}

    /// @name Static methods
    /// @{
public:
    static Zen::Studio::Workbench::I_ExplorerNodeType& getNodeType();
    /// @}

    /// @name 'Structors
    /// @{
protected:
             I_GameObjectType(const std::string& _name);
    virtual ~I_GameObjectType();
    /// @}

};  // interface I_GameObjectType

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace GameBuilder
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

#endif // GAME_BUILDER_I_GAME_OBJECT_TYPE_HPP_INCLUDED
