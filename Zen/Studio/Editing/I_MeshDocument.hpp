//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
// IndieZen Game Engine Framework
//
// Copyright (C) 2001 - 2008 Tony Richards
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
#ifndef INDIEZEN_EDITING_I_MESH_DOCUMENT_HPP_INCLUDED
#define INDIEZEN_EDITING_I_MESH_DOCUMENT_HPP_INCLUDED

#include "Configuration.hpp"

#include <IndieZen/framework/Editing/I_MeshDocumentEvent.hpp>
#include <IndieZen/framework/Editing/I_MeshSelectionSet.hpp>

#include <IndieZen/framework/ResourceManager/I_Resource.hpp>

#include <IndieZen/framework/EventModeling/Event.hpp>

#include <boost/shared_ptr.hpp>
#include <boost/function.hpp>

#include <string>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace IndieZen {
namespace Editing {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
class I_MeshDocumentGroup;
class I_MeshEdgeLoop;

class EDITING_DLL_LINK I_MeshDocument
:   public ResourceManager::I_Resource
{
    /// @name Types
    /// @{
public:
	typedef EventModeling::Event<I_MeshDocumentEvent&>  event;
    /// @}

    /// @name I_MeshDocument interface
    /// @{
public:
    /// Create an event
    /// Once an event is created, you must either dispatch it through dispatchEvent()
    /// or abandon it through abandonEvent().
    virtual I_MeshDocumentEvent* createEvent(int _operation, I_MeshDocumentElement* _pElement) = 0;

    /// Dispatch an event.
    /// The document takes ownership of the event, so after this method is called
    /// assume _pEvent is destroyed and no longer valid.
    virtual void dispatchEvent(I_MeshDocumentEvent* _pEvent) = 0;

    /// Abandon an event.
    /// If you create an event and do not dispatch it, you will leak memory
    /// unless you call abandonEvent().
    virtual void abandonEvent(I_MeshDocumentEvent* _pEvent) = 0;

    /// Add a child group.
    virtual void addGroup(I_MeshDocumentGroup* _pGroup) = 0;

    virtual I_MeshDocumentGroup* createGroup(const std::string& _name, int _type) = 0;

    /// Save this mesh from the place where it was loaded.
    virtual void save() = 0;

    /// Set the active mesh selection set.  This will reset the current selection.
    virtual void setActiveSelectionSet(I_MeshSelectionSet* _pSelectionSet) = 0;

    /// Set the active edge loop.  This will reset the current selection.
    virtual void setActiveEdgeLoop(I_MeshEdgeLoop* _pEdgeLoop) = 0;

    /// DEBUG - Expand the current selection set using the edge loop
    /// detection algorithm.
    virtual void expandCurrentSelection() = 0;
    /// @}

	/// @name Events
	/// @{
public:
	event onModifiedEvent;
	/// @}

    /// @name 'Structors
    /// @{
protected:
             I_MeshDocument();
    virtual ~I_MeshDocument();
    /// @}

};  // interface I_MeshDocument

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace Editing
}   // namespace IndieZen
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

#endif // INDIEZEN_EDITING_I_MESH_DOCUMENT_HPP_INCLUDED
