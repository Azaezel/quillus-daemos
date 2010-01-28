//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
// Art Library
//
// Copyright (C) 2001 - 2009 Tony Richards
//
// Licensed under the Worlds by Sarge Publishing License - See your licensing
// agreement for terms and conditions.
//
// Do not redistribute this source code.
//
// Tony Richards trichards@gamesbysarge.com
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
#ifndef ARTLIBRARY_ART_ASSET_EXPLORER_NODE_HPP_INCLUDED
#define ARTLIBRARY_ART_ASSET_EXPLORER_NODE_HPP_INCLUDED

#include <Zen/Core/Memory/managed_ptr.hpp>
#include <Zen/Core/Event/Event.hpp>

#include <Zen/Studio/WorkbenchCommon/I_SpreadSheetDocument.hpp>

#include <Zen/StudioPlugins/ArtLibraryCommon/I_ArtAssetExplorerNode.hpp>
#include <Zen/StudioPlugins/ArtLibraryModel/I_ArtAssetExplorerNodeDataMap.hpp>

#include <string>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Zen {
    namespace Studio {
        namespace Workbench {
            class I_ExplorerNodeActions;
        }   // namespace Workbench
    }   // namespace Studio
}   // namespace Zen
namespace ArtLibrary {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
class Project;
class ArtAssetDocument;

/// @see Zone
class ArtAssetExplorerNode
:   public I_ArtAssetExplorerNode
{
    /// @name Types
    /// @{
public:
    typedef Zen::Memory::managed_ptr<Zen::Studio::Workbench::I_ExplorerNodeActions> pExplorerNodeActions_type;
    typedef I_ArtAssetExplorerNodeDataMap::pArtAssetExplorerNodeDataMap_type        pArtAssetExplorerNodeDataMap_type;
    typedef I_ArtAssetExplorerNodeDataMap::pArtAssetExplorerNodeDomainObject_type   pArtAssetExplorerNodeDomainObject_type;
    typedef Zen::Event::Event<ArtAssetDocument*>                                    DocumentEvent_type;
    /// @}

    /// @name UserData overrides
    /// @{
public:
    virtual void setName(const std::string& _name);
    virtual Zen::Studio::Workbench::I_ExplorerNodeType& getType() const;
    virtual void insert(pDatabaseConnection_type _pDBConn);
    virtual void load(pDatabaseConnection_type _pDBConn);
    virtual void save(pDatabaseConnection_type _pDBConn);
    virtual void remove(pDatabaseConnection_type _pDBConn);
    /// @}

    /// @name I_ArtAssetExplorerNode implementation
    /// @{
public:
    virtual pDocument_type getDocument();
    virtual boost::uint64_t getArtAssetId() const;
    /// @}

    /// @name ArtAssetExplorerNode implementation
    /// @{
public:
    /// Called by load() to set the m_pArtAssetExplorerNodeDO.
    /// This should be treated as private, but it needs to be public since
    /// it's called by a visitor struct.
    void setArtAssetExplorerNodeDO(pArtAssetExplorerNodeDomainObject_type _pArtAssetExplorerNodeDO);
private:
    /// Static factory destroy method.
    static void destroyDocument(Zen::Memory::managed_weak_ptr<Zen::Studio::Workbench::I_Document> _wpDoc);
    /// @}

    /// @name 'Structors
    /// @{
public:
    /// @todo Should _parent be Folder?  What about other game object types?
    explicit ArtAssetExplorerNode(Zen::Studio::Workbench::I_Project& _parent, const std::string& _name);
    virtual ~ArtAssetExplorerNode();
    /// @}

    /// @name Member Variables
    /// @{
private:
    /// Project which this node is associated.
    Zen::Studio::Workbench::I_Project&          m_parent;
    static class ArtAssetExplorerNodeType       sm_type;
    pArtAssetExplorerNodeDomainObject_type      m_pArtAssetExplorerNodeDO;

private:
    static pExplorerNodeActions_type    sm_pActions;
    pDocument_type                      m_pDocument;
    /// @}

};  // class ArtAssetExplorerNode

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace ArtLibrary
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

#endif  // ARTLIBRARY_ART_ASSET_EXPLORER_NODE_HPP_INCLUDED
