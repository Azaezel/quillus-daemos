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
#ifndef ARTLIBRARY_I_ART_LIBRARY_SERVICE_HPP_INCLUDED
#define ARTLIBRARY_I_ART_LIBRARY_SERVICE_HPP_INCLUDED

#include "Configuration.hpp"

#include <Zen/Enterprise/AppServer/I_ApplicationService.hpp>

#include <Zen/Studio/WorkbenchCommon/I_ExplorerNode.hpp>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace ArtLibrary {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

class I_ProjectService;

class ARTLIBRARYCOMMON_DLL_LINK I_ArtLibraryService
:   public Zen::Enterprise::AppServer::I_ApplicationService
{
    /// @name Types
    /// @{
public:
    typedef Zen::Studio::Workbench::I_ExplorerNode::pUserData_type  pUserData_type;
    /// @}

    /// @name I_ArtLibraryService interface
    /// @{
public:
    /// Get the project service associated with this Art Library Service.
    virtual I_ProjectService& getProjectService() = 0;

    /// Create a folder.
    /// @param _pParent - Parent object.  Current only projects are supported
    ///                   as a parent.
    virtual pUserData_type createFolder(pUserData_type _pParent) = 0;

    /// Create an ArtAsset
    /// @param _pParent - Parent object.
    virtual pUserData_type createArtAsset(pUserData_type _pParent) = 0;
    /// @}

    /// @name 'Structors
    /// @{
protected:
             I_ArtLibraryService();
    virtual ~I_ArtLibraryService();
    /// @}

};  // interface I_ArtLibraryService

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace ArtLibrary
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

#endif // ARTLIBRARY_I_ART_LIBRARY_SERVICE_HPP_INCLUDED