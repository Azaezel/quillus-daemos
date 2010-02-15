//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
// World Builder
//
// Copyright (C) 2001 - 2009 Tony Richards
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
#ifndef WORLDBUILDER_OGITORS_CONTROLLER_HPP_INCLUDED
#define WORLDBUILDER_OGITORS_CONTROLLER_HPP_INCLUDED

#include <Zen/Core/Memory/managed_ptr.hpp>

#include <Zen/Studio/Workbench/I_SceneView.hpp>

#include <Ogitors.h>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace WorldBuilder {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
class ZenOgitorsSystem;

class OgitorsController
{
    /// @name Typedefs
    /// @{
public:
    typedef Zen::Studio::Workbench::I_SceneView::KeyEvent                   KeyEvent;
    typedef Zen::Studio::Workbench::I_SceneView::KeyEventPayload_type       KeyEventPayload_type;
    /// @}

    /// @name OgitorsController implementation
    /// @{
public:
    /// @}

    /// @name Event handlers
    /// @{
public:
    void handleKeyDown(KeyEventPayload_type _event);
    void handleKeyUp(KeyEventPayload_type _event);
    /// @}

    /// @name 'Structors
    /// @{
public:
             OgitorsController(ZenOgitorsSystem& _system, Zen::Studio::Workbench::I_SceneView& _sceneView);
    virtual ~OgitorsController();
    /// @}

    /// @name Member Variables
    /// @{
private:
    Ogitors::OgitorsRoot&                   m_root;
    ZenOgitorsSystem&                       m_system;
    Zen::Studio::Workbench::I_SceneView&    m_sceneView;
    KeyEvent::pConnection_type              m_pKeyDownConnection;
    KeyEvent::pConnection_type              m_pKeyUpConnection;
    /// @}

};  // class OgitorsController
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace WorldBuilder
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

#endif // WORLDBUILDER_OGITORS_CONTROLLER_HPP_INCLUDED
