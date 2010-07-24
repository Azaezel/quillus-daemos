//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
// Zen Engine Game Tutorial
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
// This project is part of the Zen Engine Tutorials
// 
// For more details, click on the link below for the IndieZen.org documentation:
//
// http://www.indiezen.org/wiki/wiki/zoss/Engine/Tutorials
//
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
#ifndef GAMECLIENT_WIDGET_MANAGER_HPP_INCLUDED
#define GAMECLIENT_WIDGET_MANAGER_HPP_INCLUDED

#include <Zen/Engine/Input/I_InputService.hpp>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Game {
namespace Client {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
class GameClient;

class WidgetManager
{
    /// @name Types
    /// @{
public:
    typedef Zen::Memory::managed_ptr<Zen::Engine::Input::I_MouseMoveEvent>  pMouseMoveEvent_type;
    typedef Zen::Memory::managed_ptr<Zen::Engine::Input::I_MouseClickEvent> pMouseClickEvent_type;
    /// @}

    /// @name Event Handlers
    /// @{
public:
    void handleMouseMoveEvent(pMouseMoveEvent_type _pEvent);
    void handleMouseClickEvent(pMouseClickEvent_type _pEvent);
    /// @}

    /// @name 'Structors
    /// @{
public:
    explicit WidgetManager(GameClient& _gameClient, Zen::Engine::Input::I_InputService& _inputService);
    virtual ~WidgetManager();
    /// @}

    /// @name Member Variables
    /// @{
private:
    GameClient&     m_gameClient;
    /// @}


};  // class WidgetManager

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace Client
}   // namespace Game
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

#endif // GAMECLIENT_WIDGET_MANAGER_HPP_INCLUDED
