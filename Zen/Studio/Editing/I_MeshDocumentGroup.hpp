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
#ifndef INDIEZEN_EDITING_I_MESH_DOCUMENT_GROUP_HPP_INCLUDED
#define INDIEZEN_EDITING_I_MESH_DOCUMENT_GROUP_HPP_INCLUDED

#include "Configuration.hpp"

#include <IndieZen/framework/Editing/I_MeshDocumentEvent.hpp>

#include <IndieZen/framework/EventModeling/Event.hpp>

#include <string>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace IndieZen {
namespace Editing {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
class I_MeshDocument;

class EDITING_DLL_LINK I_MeshDocumentGroup
{
    /// @name Types
    /// @{
public:
	typedef EventModeling::Event<I_MeshDocumentEvent&> event;
    /// @}

    /// @name I_MeshDocumentGroup interface
    /// @{
public:
    /// Get the document that this group belongs
    virtual I_MeshDocument& getDocument() =  0;

    virtual const std::string& getName() const = 0;

    virtual int getType() const = 0;

    virtual I_MeshDocumentElement* createChild(const std::string& _name) = 0;
    /// @}

	/// @name Events
	/// @{
public:
	event onModifiedEvent;
	/// @}

    /// @name 'Structors
    /// @{
protected:
             I_MeshDocumentGroup();
    virtual ~I_MeshDocumentGroup();
    /// @}

};  // interface I_MeshSelectionSet

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace Editing
}   // namespace IndieZen
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

#endif // INDIEZEN_EDITING_I_MESH_DOCUMENT_GROUP_HPP_INCLUDED
