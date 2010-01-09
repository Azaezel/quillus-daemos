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
#ifndef GAME_BUILDER_I_PROJECT_LISTENER_HPP_INCLUDED
#define GAME_BUILDER_I_PROJECT_LISTENER_HPP_INCLUDED

#include "Configuration.hpp"

#include <boost/noncopyable.hpp>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace GameBuilder {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
class I_Project;
class I_GameObjectType;
class I_GameObjectTypeDocument;
class I_GameObjectElement;

/// A project listener.
/// A project listener is notified of all changes made to a project.
/// Implement this interface and subscribe to a I_Project in order
/// to receive notifications of changes to a GameBuilder::I_Project
/// and all of the child elements.
/// @see GameBuilder::I_Project::subscribe()
class GAMEBUILDERCOMMON_DLL_LINK I_ProjectListener
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

    /// Called when a game object type is created.
    /// Don't retain a reference to this element.  Instead, use getNode()->getNodeId()
    /// as a unqiue key.
    virtual void gameObjectTypeCreated(I_GameObjectType& _gameObjectType) = 0;

    /// Called when a game object type document is modified.
    virtual void gameObjectTypeDocumentModified(I_GameObjectTypeDocument& _document) = 0;

    /// Called when a game object type is deleted.
    virtual void gameObjectTypeDeleted(I_GameObjectType& _gameObjectType) = 0;
    /// @}

    /// @name 'Structors
    /// @{
protected:
             I_ProjectListener();
    virtual ~I_ProjectListener();
    /// @}

};  // interface I_ProjectListener

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace GameBuilder
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

#endif // GAME_BUILDER_I_PROJECT_LISTENER_HPP_INCLUDED
