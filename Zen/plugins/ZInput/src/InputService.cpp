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

#include "InputService.hpp"
#include "InputMap.hpp"
#include "KeyEvent.hpp"
#include "MouseClickEvent.hpp"
#include "MouseMoveEvent.hpp"
#include "KeyModifierState.hpp"

#include <Zen/Core/Utility/runtime_exception.hpp>

#include <Zen/Core/Threading/MutexFactory.hpp>
#include <Zen/Core/Threading/CriticalSection.hpp>

#include <Zen/Core/Scripting/I_ScriptType.hpp>

#include <Zen/Engine/Input/I_InputServiceManager.hpp>

#include <boost/bind.hpp>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Zen {
namespace ZInput {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
InputService::InputService(config_type &_config)
//:   m_keyInput(*new KeyInput(_config))
:   m_pScriptObject(NULL)
,   m_isPaused(true)
,   m_pKeyModifierState(&NullKeyModifierState::instance())
{
    m_pMutex = Threading::MutexFactory::create();

    // TODO Move this back to KeyInput?
    OIS::ParamList paramList;

    for (config_type::iterator iter = _config.begin(); iter != _config.end(); iter++)
    {
        paramList.insert(OIS::ParamList::value_type(iter->first, iter->second));
    }

#if defined OIS_WIN32_PLATFORM
	//
	// Define Mouse Cooperative Level:
	//
	// Default mouse behavior is DISCL_EXCLUSIVE + DISCL_FOREGROUND.

	// can receive mouse input while window not in focus:
	//paramList.insert(std::make_pair(std::string("w32_mouse"), std::string("DISCL_BACKGROUND")));
	// requesting Exclusive access to the mouse (makes cursor invisible):
	//paramList.insert(std::make_pair(std::string("w32_mouse"), std::string("DISCL_EXCLUSIVE")));
	// only receive mouse events when window has focus:
	paramList.insert(std::make_pair(std::string("w32_mouse"), std::string("DISCL_FOREGROUND")));
	// share mouse access:
	paramList.insert(std::make_pair(std::string("w32_mouse"), std::string("DISCL_NONEXCLUSIVE")));

	//
	// Define Keyboard Cooperative Level:
	//
	// Default keyboard behavior is DISCL_NONEXCLUSIVE + DISCL_FOREGROUND + DISCL_NOWINKEY.

	// only receive keyboard events when window has focus:
	paramList.insert(std::make_pair(std::string("w32_keyboard"), std::string("DISCL_FOREGROUND")));
	// share keyboard access:
	paramList.insert(std::make_pair(std::string("w32_keyboard"), std::string("DISCL_NONEXCLUSIVE")));
	// disables Windows key:
	paramList.insert(std::make_pair(std::string("w32_keyboard"), std::string("DISCL_NOWINKEY")));

#elif defined OIS_LINUX_PLATFORM
	paramList.insert(std::make_pair(std::string("x11_mouse_grab"), std::string("false")));
	paramList.insert(std::make_pair(std::string("x11_mouse_hide"), std::string("false")));
	paramList.insert(std::make_pair(std::string("x11_keyboard_grab"), std::string("false")));
	paramList.insert(std::make_pair(std::string("XAutoRepeatOn"), std::string("true")));
#endif

    m_pInputManager = OIS::InputManager::createInputSystem(paramList);

    m_pKeyboard = static_cast<OIS::Keyboard*>(m_pInputManager->createInputObject(OIS::OISKeyboard, true));

    m_pKeyboard->setEventCallback(this);

    m_pKeyboard->setTextTranslation(OIS::Keyboard::Ascii);    

    m_pMouse = static_cast<OIS::Mouse*>(m_pInputManager->createInputObject(OIS::OISMouse, true));
    m_pMouse->setEventCallback(this);
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
InputService::~InputService()
{
    Threading::MutexFactory::destroy(m_pMutex);
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
InputService::setWindowSize(int _width, int _height)
{
	const OIS::MouseState &ms = m_pMouse->getMouseState();
	ms.width = _width;
	ms.height = _height;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
InputService::pauseEvents()
{
    m_isPaused = true;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
InputService::resumeEvents()
{
    m_isPaused = false;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
InputService::processEvents()
{
    if (!m_isPaused)
    {
        m_pKeyboard->capture();
        m_pMouse->capture();
    }
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
bool
InputService::keyPressed(const OIS::KeyEvent& _keyEvent)
{
    Engine::Input::KeyCode_enum code = (Engine::Input::KeyCode_enum)_keyEvent.key;

    KeyModifierState::KeyModifierMap_type::const_iterator iter = 
        KeyModifierState::sm_keyModifierPressedMap.find(code);
    if( iter != KeyModifierState::sm_keyModifierPressedMap.end() )
    {
        iter->second(m_pKeyModifierState, *this);
    }

    pKeyEvent_type pKeyEvent(new KeyEvent(_keyEvent, true, KeyState(code,m_pKeyModifierState)));

    onKeyEvent(pKeyEvent);

    return true;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
bool
InputService::keyReleased(const OIS::KeyEvent& _keyEvent)
{
    Engine::Input::KeyCode_enum code = (Engine::Input::KeyCode_enum)_keyEvent.key;

    KeyModifierState::KeyModifierMap_type::const_iterator iter = 
        KeyModifierState::sm_keyModifierReleasedMap.find(code);
    if( iter != KeyModifierState::sm_keyModifierReleasedMap.end() )
    {
        iter->second(m_pKeyModifierState, *this);
    }

    pKeyEvent_type pKeyEvent(new KeyEvent(_keyEvent, false, KeyState(code,m_pKeyModifierState)));

    onKeyEvent(pKeyEvent);

    return true;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
bool
InputService::mouseMoved(const OIS::MouseEvent& _mouseEvent)
{
    MouseMoveEvent mouseEvent(_mouseEvent, *m_pKeyModifierState);

    onMouseMoveEvent(mouseEvent);

	// ALWAYS return TRUE from this function, unless you want the input buffer to be cleared by OIS
    return true;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
bool
InputService::mousePressed(const OIS::MouseEvent& _mouseEvent, OIS::MouseButtonID _button)
{
    MouseClickEvent mouseEvent(_mouseEvent, true, _button, *m_pKeyModifierState);

    onMouseClickEvent(mouseEvent);

    return true;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
bool
InputService::mouseReleased(const OIS::MouseEvent& _mouseEvent, OIS::MouseButtonID _button)
{
    MouseClickEvent mouseEvent(_mouseEvent, false, _button, *m_pKeyModifierState);

    onMouseClickEvent(mouseEvent);

    return true;
}
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
InputService::pInputMap_type
InputService::createInputMap(const std::string& _name)
{
    std::map< std::string, wpInputMap_type >::iterator iter = m_inputMapMap.find(_name);
    if( iter != m_inputMapMap.end() )
    {
        return iter->second.lock();
    }
    else
    {
        InputMap* pRawInputMap(new InputMap(getSelfReference()));

        pInputMap_type pInputMap(pRawInputMap, 
                                 boost::bind(&InputService::destroy, this, _1));

        m_inputMapMap[_name] = wpInputMap_type(pInputMap);
        m_inputMapIdx[pRawInputMap] = _name;

        return pInputMap;
    }

    return pInputMap_type();
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void 
InputService::enableInputMap(const std::string& _name)
{
    std::map< std::string, wpInputMap_type >::iterator iter = m_inputMapMap.find(_name);
    if( iter != m_inputMapMap.end() )
    {
        iter->second->focus();
    }
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void 
InputService::disableInputMap(const std::string& _name)
{
    std::map< std::string, wpInputMap_type >::iterator iter = m_inputMapMap.find(_name);
    if( iter != m_inputMapMap.end() )
    {
        iter->second->unfocus();
    }
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void 
InputService::enableAllInputMaps()
{
    std::map< std::string, wpInputMap_type >::iterator iter = m_inputMapMap.begin();
    while( iter != m_inputMapMap.end() )
    {
        iter->second->focus();
        iter++;
    }
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void 
InputService::disableAllInputMaps()
{
    std::map< std::string, wpInputMap_type >::iterator iter = m_inputMapMap.begin();
    while( iter != m_inputMapMap.end() )
    {
        iter->second->unfocus();
        iter++;
    }
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
InputService::changeState(Engine::Input::I_KeyModifierState* _pState)
{
    m_pKeyModifierState = _pState;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
InputService::destroy(wpInputMap_type _pInputMap)
{
    /// Fire the InputMap onDestroyEvent
    _pInputMap->onDestroyEvent(_pInputMap.get());

    /// Delete the InputMap
    InputMap* const pInputMap = dynamic_cast<InputMap*>(_pInputMap.get());
    if( pInputMap != NULL )
    {
        /// Find the InputMap cache index
        std::map< InputMap*, std::string >::iterator index = m_inputMapIdx.find(pInputMap);
        if( index != m_inputMapIdx.end() )
        {
            /// Find the InputMap cache entry
            std::map< std::string, wpInputMap_type >::iterator inputMap = m_inputMapMap.find(index->second);
            if( inputMap != m_inputMapMap.end() )
            {
                /// Erase the InputMap cache entry
                m_inputMapMap.erase(inputMap);
            }
            else
            {
                throw Zen::Utility::runtime_exception("Zen::ZInput::InputService::destroy() : Cannot locate InputMap in m_inputMapMap.");
            }

            /// Erase the InputMap cache index entry
            m_inputMapIdx.erase(index);
    
            /// Delete the raw InputMap pointer
            delete pInputMap;
        }
        else
        {
            throw Zen::Utility::runtime_exception("Zen::ZInput::InputService::destroy() : Cannot locate InputMap index in m_inputMapIdx.");
        }
    }
    else
    {
        throw Zen::Utility::runtime_exception("Zen::ZInput::InputService::destroy() : _pInputMap is not a valid InputMap.");
    }
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
Scripting::I_ObjectReference*
InputService::getScriptObject()
{
    // TODO Make thread safe?
    if (m_pScriptObject == NULL)
    {
        m_pScriptObject = new ScriptObjectReference_type(
            Engine::Input::I_InputServiceManager::getSingleton().getDefaultScriptModule(), 
            Engine::Input::I_InputServiceManager::getSingleton().getDefaultScriptModule()->getScriptType(getScriptTypeName()), getSelfReference().lock());
    }

    return m_pScriptObject;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace ZInput
}   // namespace Zen
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
