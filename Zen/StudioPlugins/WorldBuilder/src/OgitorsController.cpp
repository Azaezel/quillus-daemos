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
#include "OgitorsController.hpp"

#include <boost/bind.hpp>

#include <sstream>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace WorldBuilder {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
OgitorsController::OgitorsController(ZenOgitorsSystem& _system, Zen::Studio::Workbench::I_SceneView& _sceneView)
:   m_system(_system)
,   m_sceneView(_sceneView)
,   m_root(Ogitors::OgitorsRoot::getSingleton())
{
    // Connect the events
    m_pKeyDownConnection = m_sceneView.onKeyDown.connect(boost::bind(&OgitorsController::handleKeyDown, this, _1));
    m_pKeyUpConnection = m_sceneView.onKeyUp.connect(boost::bind(&OgitorsController::handleKeyUp, this, _1));

}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
OgitorsController::~OgitorsController()
{
    // Disconnect the events
    m_pKeyDownConnection->disconnect();
    m_pKeyUpConnection->disconnect();
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
OgitorsController::handleKeyDown(KeyEventPayload_type _event)
{
    if (_event == 'L')
    {
        // HACK
        m_root.LoadScene("C:/dev/ogitor-build/RunPath/Projects/SampleScene2/SampleScene2.ogscene");
    }
    else if (_event == 'G')
    {
        // Toggle grid.
        Ogitors::NameObjectPairList list = m_root.GetObjectsByType(Ogitors::ETYPE_VIEWPORT);

        Ogitors::NameObjectPairList::iterator it = list.begin();
        while(it != list.end())
        {
            Ogitors::CViewportEditor* pViewPort = static_cast<Ogitors::CViewportEditor*>(it->second);
            
            pViewPort->ShowGrid(!pViewPort->IsGridVisible());

            it++;
        }
    }
    else
    {
        m_root.OnKeyDown(_event);
    }
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
OgitorsController::handleKeyUp(KeyEventPayload_type _event)
{
    m_root.OnKeyUp(_event);
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace WorldBuilder
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
