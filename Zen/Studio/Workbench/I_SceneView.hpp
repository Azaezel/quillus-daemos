//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
// Zen Studio
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
#ifndef ZENSTUDIO_WORKBENCH_I_SCENE_VIEW_HPP_INCLUDED
#define ZENSTUDIO_WORKBENCH_I_SCENE_VIEW_HPP_INCLUDED

#include "Configuration.hpp"

#include <Zen/Core/Event/Event.hpp>
#include <Zen/Core/Math/Math.hpp>

#include <Zen/Engine/Rendering/I_View.hpp>
#include <Zen/Engine/Rendering/I_Context.hpp>

#include <Zen/Studio/WorkbenchCommon/I_View.hpp>

#include <boost/cstdint.hpp>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Zen {
namespace Studio {
namespace Workbench {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
;

/// Scene view.
/// A scene view subscribes to a I_SceneModel and recieves
/// notifications when the model changes.  Use this interface to populate
/// a GUI or for anything else requiring notifications of a scene model
/// modification.
/// @see I_SceneModel::subscribe()
/// TODO - MG - Does this need to be moved to WorkbenchCommon?
class WORKBENCH_DLL_LINK I_SceneView
:   public virtual I_View
{
    /// @name Types
    /// @{
public:
    /// Untranslated key code.
    /// @todo Define a more appropriate payload for key events.
    ///     The payload should include shift values, raw code, translated 
    ///     ASCII and Unicode characters, etc.
    typedef boost::uint32_t                         KeyEventPayload_type;
    typedef Zen::Event::Event<KeyEventPayload_type> KeyEvent;

    typedef struct 
    {
        int x;
        int y;
        unsigned int buttons;
    } MouseEventPayload_type;

    typedef struct
    {
        int x;
        int y;
        unsigned int buttons;
        Math::Real delta;
    } MouseWheelEventPayload_type;

    typedef Zen::Event::Event<const MouseEventPayload_type&>        MouseEvent;
    typedef Zen::Event::Event<const MouseWheelEventPayload_type&>   MouseWheelEvent;

    typedef std::map<std::string, std::string>                      config_type;
    /// @}

    /// @name I_SceneView interface
    /// @{
public:
    /// Get the Zen Engine scene view.
    virtual Zen::Engine::Rendering::I_View* getSceneView() = 0;

    /// Get the Zen Engine rendering context.
    virtual Zen::Engine::Rendering::I_Context* getContext() = 0;

    /// Document name modified notification.
    virtual void onNameModified(const std::string& _shortName, const std::string& _longName) = 0;

    /// Set the skybox properties
    virtual void setSkyboxProperties(const std::string& _type, config_type _config) = 0;

    /// Add a resource location
    virtual void addResourceLocation(const std::string& _path, const std::string& _type, const std::string& _group, bool _recursive) = 0;
    /// @}

    /// @name Events
    /// @{
public:
    KeyEvent        onKeyDown;
    KeyEvent        onKeyUp;
    MouseEvent      onMouseMove;
    MouseEvent      onLeaveWindow;
    MouseEvent      onLeftDown;
    MouseEvent      onLeftUp;
    MouseEvent      onRightDown;
    MouseEvent      onRightUp;
    MouseEvent      onMiddleDown;
    MouseEvent      onMiddleUp;
    MouseWheelEvent onMouseWheel;
    /// @}

    /// @name 'Structors
    /// @{
protected:
             I_SceneView();
    virtual ~I_SceneView();
    /// @}

};  // interface I_SceneView

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace Workbench
}   // namespace Studio
namespace Memory 
{
    /// I_SceneView is managed by a factory
    template<>
    struct is_managed_by_factory<Zen::Studio::Workbench::I_SceneView> : public boost::true_type{};
}   // namespace Memory
}   // namespace Zen
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

#endif // ZENSTUDIO_WORKBENCH_I_SCENE_VIEW_HPP_INCLUDED
