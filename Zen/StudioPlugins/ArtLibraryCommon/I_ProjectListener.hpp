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
#ifndef ARTLIBRARY_I_PROJECT_LISTENER_HPP_INCLUDED
#define ARTLIBRARY_I_PROJECT_LISTENER_HPP_INCLUDED

#include "Configuration.hpp"

#include <boost/noncopyable.hpp>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace ArtLibrary {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
class I_Project;
class I_ArtAssetExplorerNode;
class I_ArtAssetDocument;

/// A project listener.
/// A project listener is notified of all changes made to a project.
/// Implement this interface and subscribe to a I_Project in order
/// to receive notifications of changes to a ArtAsset::I_Project
/// and all of the child elements.
/// @see ArtAsset::I_Project::subscribe()
class ARTLIBRARYCOMMON_DLL_LINK I_ProjectListener
:   public boost::noncopyable
{
    /// @name Types
    /// @{
public:
    /// @}

    /// @name I_ProjectListener interface
    /// @{
public:
    /// Called when the project is deleted.
    /// During this method call, the listener must dereference the project
    /// subscription and assume the project is going out of scope.
    //virtual void onProjectDeleted(I_Project& _project) = 0;

    /// Called when an art asset is created.
    /// Don't retain a reference to this element.  Instead, use getNode()->getNodeId()
    /// as a unqiue key.
    virtual void artAssetCreated(I_ArtAssetExplorerNode& _artAsset) = 0;

    /// Called when an art asset document is modified.
    virtual void artAssetDocumentModified(I_ArtAssetDocument& _document) = 0;

    /// Called when an art asset is deleted.
    virtual void artAssetDeleted(I_ArtAssetExplorerNode& _artAsset) = 0;
    /// @}

    /// @name 'Structors
    /// @{
protected:
             I_ProjectListener();
    virtual ~I_ProjectListener();
    /// @}

};  // interface I_ProjectListener

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace ArtLibrary
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

#endif // ARTLIBRARY_I_PROJECT_LISTENER_HPP_INCLUDED
