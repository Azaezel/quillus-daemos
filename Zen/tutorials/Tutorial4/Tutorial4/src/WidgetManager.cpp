//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
// Zen Engine Game Tutorial
//
// Copyright (C) 2001 - 2010 Tony Richards
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
#include "WidgetManager.hpp"
#include "GameClient.hpp"

#include <Zen/Core/Math/Radian.hpp>

#include <Zen/Engine/Input/I_MouseMoveEvent.hpp>
#include <Zen/Engine/Input/I_MouseClickEvent.hpp>

#include <Zen/Engine/Rendering/I_RenderingCanvas.hpp>
#include <Zen/Engine/Rendering/I_Camera.hpp>

#include <Zen/Engine/Widgets/I_WidgetService.hpp>

#include <boost/bind.hpp>

#include <iostream>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Game {
namespace Client {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

WidgetManager::WidgetManager(GameClient& _gameClient, Zen::Engine::Input::I_InputService& _inputService)
:   m_gameClient(_gameClient)
,   m_rightMouseDown(false)
{
    _inputService.onMouseMoveEvent.connect(boost::bind(&WidgetManager::handleMouseMoveEvent, this, _1));
    _inputService.onMouseClickEvent.connect(boost::bind(&WidgetManager::handleMouseClickEvent, this, _1));
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
WidgetManager::~WidgetManager()
{
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
WidgetManager::handleMouseMoveEvent(pMouseMoveEvent_type _pEvent)
{
	static int cursorVisible = 0;
    static bool widgetMouseVisible = false;

    Zen::Engine::Widgets::I_WidgetService& widgetService(
        m_gameClient.base().getWidgetService()
    );

    // TR This code is not working on Linux for some reason... dunno why.
#if 0
	if ((unsigned int)_pEvent->getX() == m_gameClient.base().getRenderingCanvas().getWidth() ||
		(unsigned int)_pEvent->getY() == m_gameClient.base().getRenderingCanvas().getHeight() ||
		_pEvent->getX() == 0 ||
		_pEvent->getY() == 0)
    {
        // Show the OS Cursor; might need to do it more than once
		while (cursorVisible < 0)
		{
            // Hide the operating system's mouse cursor.
            widgetService.hideMouseCursor();
		}

        // TODO Tell the widget service that the mouse is gone
        // m_pWidgetService->mouseGone(true);

        if (widgetMouseVisible)
        {
            // TODO Hide the widget mouse and disable the widget service
            //m_pWidgetService->showCursor(false);
            //m_pWidgetService->setEnabled(false);

            widgetMouseVisible = false;
        }
    }
    else
    {
        // Hide the OS cursor; 
		while (cursorVisible > -1)
		{
            // TR - Why does this deadlock on Linux
            //widgetService.hideMouseCursor();
		}

        // Tell the widet service to show the mouse
        if (!widgetMouseVisible)
        {
            // Hide the widget mouse and disable the widget service
            //m_pWidgetService->showCursor(true);
            //m_pWidgetService->setEnabled(true);

            widgetMouseVisible = true;
        }
    }
#endif

    //m_pWidgetService->setMousePosition(_event.getX(), _event.getY());

    if (_pEvent->getZDelta() != 0)
    {
        //m_pWidgetSErvice->setMouseWheelDelta(_event.getZDelta());
    }

    // If the shift key is down, the mouse moves the camera, otherwise the mouse moves
    // the direction that the player is facting.
    Zen::Engine::Rendering::I_Camera& camera = m_gameClient.base().getRenderingCanvas().getCurrentCamera();

    if (false)  // (is shift down?)
    {
        if (m_rightMouseDown)
        {
            // TODO Implement (move player)
        }
    }
    else
    {
        // Move the camera - this logic only works for the chase camera

        if (m_rightMouseDown)
        {

            float yaw = _pEvent->getXDelta();
            yaw /= -100.0;
            Zen::Math::Radian radian(yaw);
            camera.yaw(radian, true, false);

            float pitch = _pEvent->getYDelta();
            pitch /= -100.0;
            Zen::Math::Radian radian2(pitch);
            camera.pitch(radian2, true, true);

        }
    }

    // Use the mouse scroll for zoom
    float zoom = _pEvent->getZDelta();
    if (zoom != 0)
    {
        zoom /= -100;

        Zen::Math::Vector3 position = camera.getPosition();
        Zen::Math::Real length = position.magnitude() + zoom;
        position.normalize();
        position *= length;
        camera.setPosition(position.m_x, position.m_y, position.m_z);
    }
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
WidgetManager::handleMouseClickEvent(pMouseClickEvent_type _pEvent)
{
    // Right mouse button toggles the cursor
    if (_pEvent->getButton() == Zen::Engine::Input::BUTTON_ID_RIGHT)
    {
        if (_pEvent->wasClicked())
        {
            m_rightMouseDown = true;
        }
        else
        {
            m_rightMouseDown = false;
        }
    }
    else if (_pEvent->getButton() == Zen::Engine::Input::BUTTON_ID_LEFT)
    {
        // Move the player to the projected point.
    }
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace Client
}   // namespace Game
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
