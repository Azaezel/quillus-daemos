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
#ifndef ARTLIBRARY_EXPLORER_NODE_FACTORY_HPP_INCLUDED
#define ARTLIBRARY_EXPLORER_NODE_FACTORY_HPP_INCLUDED

#include <Zen/Core/Memory/managed_weak_ptr.hpp>

#include <Zen/Studio/WorkbenchCommon/I_ExplorerNodeFactory.hpp>
#include <Zen/Studio/WorkbenchCommon/I_ExplorerNode.hpp>

#include <Zen/Studio/WorkbenchCommon/I_ProjectService.hpp>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace ArtLibrary {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
;

class ExplorerNodeFactory
:   public Zen::Studio::Workbench::I_ExplorerNodeFactory
{
    /// @name Types
    /// @{
public:
    typedef Zen::Memory::managed_weak_ptr<Zen::Studio::Workbench::I_ProjectService> wpProjectService_type;
    /// @}

    /// @name I_ExplorerNodeFactory implementation
    /// @{
public:
    virtual pProject_type createProject(Zen::Studio::Workbench::I_ProjectExplorerController& _controller, pProjectDomainObject_type _pProjectDO, pExplorerNodeDomainObject_type _pExplorerNodeDO);
    virtual pUserData_type createNodeUserData(boost::uint64_t _explorerNodeId, const std::string& _nodeType, Zen::Studio::Workbench::I_ExplorerNode& _parent);
    /// @}

    /// @name Static Methods
    /// @{
public:
    static ExplorerNodeFactory& getSingleton();
    /// @}

    /// @name 'Structors
    /// @{
protected:
             ExplorerNodeFactory();
    virtual ~ExplorerNodeFactory();
    /// @}

};  // class ExplorerNodeFactory

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace ArtLibrary
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
#endif  // ARTLIBRARY_EXPLORER_NODE_FACTORY_HPP_INCLUDED
