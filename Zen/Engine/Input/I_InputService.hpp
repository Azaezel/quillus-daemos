//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
// Zen Game Engine Framework
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
#ifndef ZEN_ENGINE_INPUT_MANAGER_I_INPUT_SERVICE_HPP_INCLUDED
#define ZEN_ENGINE_INPUT_MANAGER_I_INPUT_SERVICE_HPP_INCLUDED

#include "Configuration.hpp"

#include <Zen/Core/Memory/managed_ptr.hpp>
#include <Zen/Core/Event/Event.hpp>
#include <Zen/Core/Plugins/I_Service.hpp>
#include <Zen/Core/Scripting/I_ScriptableType.hpp>
#include <Zen/Core/Scripting/ObjectReference.hpp>

#include <string>
#include <map>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Zen {
namespace Engine {
namespace Input {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
class I_InputServiceFactory;
class I_InputMap;

class I_ButtonEvent;
class I_ScalarEvent;
class I_VectorEvent;
class I_KeyEvent;
class I_KeyModifierState;
class I_MouseMoveEvent;
class I_MouseClickEvent;

/// I'm still deciding on how the events should work.  Should I create
/// some sort of FocusManager that directs where the input goes, or 
/// should everything that wants the events decide if they have focus
/// and process the event accordingly, or should they subscribe / 
/// unsubscribe as they gain and lose focus, or some sort of combination?
/// For now everything subscribes and each subscriber determines if the
/// event is intended for them.
class INPUTMANAGER_DLL_LINK I_InputService
:   public virtual Zen::Scripting::I_ScriptableType
{
    /// @name Friend declarations
    /// @{
protected:
    friend class I_KeyModifierState;
    /// @}

    /// @name Types
    /// @{
public:
    typedef std::string                                     index_type;
    typedef I_InputServiceFactory                           factory_type;

    typedef Memory::managed_ptr<I_InputService>             pScriptObject_type;
    typedef Scripting::ObjectReference<I_InputService>      ScriptObjectReference_type;

    typedef Zen::Memory::managed_ptr<Zen::Plugins::I_Service>    pAbstractService_type;

    typedef Zen::Memory::managed_ptr<I_InputMap>            pInputMap_type;
    typedef Zen::Memory::managed_ptr<I_InputService>        pService_type;
    typedef Zen::Memory::managed_weak_ptr<I_InputService>   wpService_type;
    typedef Zen::Event::Event<wpService_type>               service_event;

    typedef Zen::Memory::managed_ptr<I_ButtonEvent>         pButton_type;
    typedef Zen::Event::Event<pButton_type>                 button_event;

    typedef Zen::Memory::managed_ptr<I_ScalarEvent>         pScalar_type;
    typedef Zen::Event::Event<pScalar_type>                 scalar_event;

    typedef Zen::Memory::managed_ptr<I_VectorEvent>         pVector_type;
    typedef Zen::Event::Event<pVector_type>                 vector_event;

    typedef Zen::Memory::managed_ptr<I_KeyEvent>            pKeyEventPayload_type;
    typedef Zen::Event::Event<pKeyEventPayload_type>        key_event;

    typedef Zen::Event::Event<I_MouseMoveEvent&>            MouseMoveEvent_type;
    typedef Zen::Event::Event<I_MouseClickEvent&>           MouseClickEvent_type;
    /// @}

    /// @name I_InputService interface
    /// @{
public:
    /// Pass in the size of the render window.
    /// @param _width Width of the render window.
    /// @param _height Height of the render window.
    virtual void setWindowSize(int _width, int _height) = 0;

    /// Temporarily pauses event notifications.  
    /// Generally you should call this method when your control
    /// loses focus.
    virtual void pauseEvents() = 0;

    /// Resume events after pauseEvents was called.
    /// Generally you should call this method when your control
    /// regains focus.
    virtual void resumeEvents() = 0;

    /// Some devices require you to call this for every 
    /// frame that is rendered.
    virtual void processEvents() = 0;
 
    /// Create an input map.
    /// @note This is a key input map and eventually should be renamed as such.
    virtual pInputMap_type createInputMap(const std::string& _name) = 0;

    /// Enable an input map.
    virtual void enableInputMap(const std::string& _name) = 0;

    /// Disable an input map.
    virtual void disableInputMap(const std::string& _name) = 0;

    /// Enable all input maps.
    virtual void enableAllInputMaps() = 0;

    /// Disable all input maps.
    virtual void disableAllInputMaps() = 0;

protected:
    /// Change the key modifier state (shift,ctrl,alt,win,etc.)
    virtual void changeState(I_KeyModifierState* _pState) = 0;
    /// @}

    /// @name I_ScriptableType implementation
    /// @{
public:
    /// This is implemented to return "InputService"
    /// Override this method if you create a derived type
    const std::string& getScriptTypeName();
    /// @}

    /// @name Static methods
    /// @{
public:
    static const std::string& getNamespace();
    static const std::string& getExtensionPointName();
    /// @}

    /// @name Events
    /// @{
public:
    service_event           onDestroyEvent;
    button_event            onButtonEvent;
    scalar_event            onScalarEvent;
    vector_event            onVectorEvent;
    key_event               onKeyEvent;
    MouseMoveEvent_type     onMouseMoveEvent;
    MouseClickEvent_type    onMouseClickEvent;
    /// @}

    /// @name 'Structors
    /// @{
protected:
             I_InputService();
    virtual ~I_InputService();
    /// @}

};  // interface I_InputService

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace Input
}   // namespace Engine
namespace Memory 
{
    /// I_InputService is managed by a factory
    template<>
	struct is_managed_by_factory<Zen::Engine::Input::I_InputService> : public boost::true_type{};
}   // namespace Memory
}   // namespace Zen
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

#endif // ZEN_ENGINE_INPUT_MANAGER_I_INPUT_SERVICE_HPP_INCLUDED
