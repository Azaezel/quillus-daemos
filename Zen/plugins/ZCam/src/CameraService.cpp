//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
// Zen Game Engine Framework
//
// Copyright (C) 2001 - 2007 Tony Richards
// Copyright (C)        2008 Walt Collins
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
//  Walt Collins (Arcanor) - wcollins@indiezen.com
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
#include "CameraService.hpp"
#include "OrbitCamera.hpp"

#include <boost/bind.hpp>

#include <iostream>
#include <stddef.h>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Zen {
namespace ZCam {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
CameraService::CameraService()
{
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
CameraService::~CameraService()
{
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
CameraService::pCamera_type
CameraService::createCameraController(const std::string& _type)
{
#if 0
    if (!m_pRenderingService.isValid())
    {
        std::cout << "Error: must setup required services prior to creating camera objects in CameraService::createOrbitCamera()." << std::endl;
        return pCamera;
    }
#endif
    if (_type == "orbit")
    {
        OrbitCamera* pRawCamera = new OrbitCamera(this->getSelfReference());
        pCamera_type pCamera = pCamera_type(pRawCamera, boost::bind(&CameraService::onDestroyCamera, this, _1));
        wpCamera_type wpCamera(pCamera);
        pRawCamera->setSelfReference(wpCamera);
        return pCamera;
    }

    std::cout << "Error: unknown camera controller type in CameraService::createCameraController()." << std::endl;
    pCamera_type pCamera;
    return pCamera;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
CameraService::onDestroyCamera(wpCamera_type _wpCamera)
{
    /// Fire the Camera's onDestroyEvent
    _wpCamera->onDestroyEvent(_wpCamera);
    
    /// delete the Camera pointer
    OrbitCamera* pCamera = dynamic_cast<OrbitCamera*>(_wpCamera.get());

    if (pCamera)
    {
        delete pCamera;
    }
    else
    {
        throw std::exception("Zen::ZCam::CameraService::onDestroyCamera() : _wpCamera is an invalid Camera.");
    }
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
CameraService::onFrame()
{
    // TODO - do we need this function?
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace ZCam
}   // namespace Zen
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
