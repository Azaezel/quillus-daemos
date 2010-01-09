//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
// Art Library
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
#ifndef ARTLIBRARY_I_ART_ASSET_EXPLORER_NODE_HPP_INCLUDED
#define ARTLIBRARY_I_ART_ASSET_EXPLORER_NODE_HPP_INCLUDED

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
namespace ArtLibrary {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

/// Art Asset explorer node user data interface
class ARTLIBRARYCOMMON_DLL_LINK I_ArtAssetExplorerNode
:   public Zen::Studio::Workbench::I_ExplorerNode::UserData
{
    /// @name Types
    /// @{
public:
    typedef Zen::Memory::managed_ptr<Zen::Studio::Workbench::I_Document>            pDocument_type;
    typedef Zen::Memory::managed_ptr<Zen::Studio::Workbench::I_ExplorerNodeActions> pExplorerNodeActions_type;
    /// @}

    /// @name I_ArtAssetExplorerNode interface
    /// @{
public:
    /// Get the document associated with this Art Asset.
    /// @todo MG - This should probably be a pFutureDocument_type instead of a
    ///            pDocument_type.  If the document has not been loaded yet, it
    ///            will likely take a while to load.
    virtual pDocument_type getDocument() = 0;

    ///
    virtual boost::uint64_t getArtAssetId() const = 0;
    /// @}

    /// @name Static methods
    /// @{
public:
    static Zen::Studio::Workbench::I_ExplorerNodeType& getNodeType();
    /// @}

    /// @name 'Structors
    /// @{
protected:
             I_ArtAssetExplorerNode(const std::string& _name);
    virtual ~I_ArtAssetExplorerNode();
    /// @}

};  // interface I_ArtAssetExplorerNode

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace ArtLibrary
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

#endif // ARTLIBRARY_I_ART_ASSET_EXPLORER_NODE_HPP_INCLUDED
