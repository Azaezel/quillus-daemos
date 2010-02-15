//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
// Zen Game Engine Framework
//
// Copyright (C) 2001 - 2009 Tony Richards
// Copyright (C) 2008 - 2009 Matthew Alan Gray
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
//  Matthew Alan Gray mgray@indiezen.org
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
#ifndef ZEN_ZINPUT_INPUT_SERVICE_HPP_INCLUDED
#define ZEN_ZINPUT_INPUT_SERVICE_HPP_INCLUDED

#include <Zen/Core/Memory/managed_ptr.hpp>
#include <Zen/Core/Memory/managed_self_ref.hpp>

#include <Zen/Core/Threading/I_Mutex.hpp>
#include <Zen/Engine/Input/I_InputService.hpp>

//#include "KeyMap.hpp"

#include <OISInputManager.h>
#include <OISEvents.h>
#include <OISKeyboard.h>
#include <OISMouse.h>

#include <string>
#include <map>
#include <set>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Zen {
namespace ZInput {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

class InputService
:   public Engine::Input::I_InputService
,   public Memory::managed_self_ref<Engine::Input::I_InputService>
,   public OIS::KeyListener
,   public OIS::MouseListener
{
    /// @name Types
    /// @{
public:
    typedef Memory::managed_weak_ptr<Engine::Input::I_KeyMap>         wpKeyMap_type;
    typedef Memory::managed_ptr<Engine::Input::I_KeyEvent>              pKeyEvent_type;

    typedef std::map<Plugins::I_Service*, wpKeyMap_type>              inputMapMap_type;
    typedef std::map<std::string, std::string>                          config_type;
    /// @}

    /// @name I_InputService implementation
    /// @{
public:
    virtual void setWindowSize(int _width, int _height);
    virtual void pauseEvents();
    virtual void resumeEvents();
    virtual void processEvents();
#if 0
    virtual pKeyMap_type createKeyMap(const std::string& _name);
    virtual void enableKeyMap(const std::string& _name);
    virtual void disableKeyMap(const std::string& _name);
    virtual void enableAllKeyMaps();
    virtual void disableAllKeyMaps();
#endif
protected:
    virtual void changeState(Engine::Input::I_KeyModifierState* _pState);
    /// @}

    /// @name I_ScriptableType implementation
    /// @{
public:
    virtual Scripting::I_ObjectReference* getScriptObject();
    /// @}

    /// @name OIS::KeyListener implementation
    /// @{
public:
    virtual bool keyPressed(const OIS::KeyEvent& _keyEvent);
    virtual bool keyReleased(const OIS::KeyEvent& _keyEvent);
    /// @}

    /// @name OIS::MouseListener implementation
    /// @{
public:
    virtual bool mouseMoved(const OIS::MouseEvent& _mouseEvent);
    virtual bool mousePressed(const OIS::MouseEvent& _mouseEvent, OIS::MouseButtonID _button);
    virtual bool mouseReleased(const OIS::MouseEvent& _mouseEvent, OIS::MouseButtonID _button);
    /// @}

    /// @name InputService implementation
    /// @{
public:
    /// @}

    /// @name Event handlers
    /// @{
public:
    //void destroy(wpKeyMap_type _pKeyMap);
    /// @}

    /// @name 'Structors
    /// @{
public:
             InputService(config_type& _config);
    virtual ~InputService();
    /// @}

    /// @name Member variables
    /// @{
private:
    Threading::I_Mutex*                     m_pMutex;
    Scripting::I_ObjectReference*           m_pScriptObject;

    bool                                    m_isPaused;

    //std::map<std::string, wpKeyMap_type>  m_inputMapMap;
    //std::map<KeyMap*, std::string>        m_inputMapIdx;

    OIS::InputManager*                      m_pInputManager;
    OIS::Keyboard*                          m_pKeyboard;
    OIS::Mouse*                             m_pMouse;

    Engine::Input::I_KeyModifierState*      m_pKeyModifierState;

    /// @}

};  // class InputService

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace ZInput
}   // namespace Zen
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

#endif // ZEN_ZINPUT_INPUT_SERVICE_HPP_INCLUDED
