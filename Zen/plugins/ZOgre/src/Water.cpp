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
#ifdef USE_HYDRAX

#include "Water.hpp"
#include "Camera.hpp"
#include "RenderingCanvas.hpp"

// Hydrax includes
#include <Hydrax/Noise/Perlin/Perlin.h>
#include <Hydrax/Modules/ProjectedGrid/ProjectedGrid.h>

#include <Zen/Engine/Resource/I_Resource.hpp>
#include <Zen/Core/Utility/runtime_exception.hpp>

#include <boost/bind.hpp>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Zen {
namespace ZOgre {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
Water::Water(Zen::Engine::Rendering::I_Canvas& _canvas, const std::string& _configPath)
{
    // Get the RenderingCanvas object
    RenderingCanvas* pCanvas = dynamic_cast<RenderingCanvas*>(&_canvas);
    if( pCanvas == NULL )
    {
        throw Zen::Utility::runtime_exception("Zen::ZOgre::Water::Water() : _pView->getCanvas() did not return a valid RenderingCanvas.");
    }

    // Get the Ogre::SceneManager
    m_pSceneManager = pCanvas->getOgreSceneManager();
 
    // Get the Ogre::Viewport
    m_pViewport = pCanvas->getOgreViewport();

    // Get the Camera
    Camera* pCamera = dynamic_cast<Camera*>(&(pCanvas->getCurrentCamera()));
    if( pCamera == NULL )
    {
        throw Zen::Utility::runtime_exception("Zen::ZOgre::Water::Water() : pCanvas->getCurrentCamera() did not return a valid Camera.");
    }

    // Get the Ogre::Camera
    m_pCamera = &pCamera->getOgreCamera();

    // Create Hydrax object
    m_pHydrax = new Hydrax::Hydrax(m_pSceneManager, 
                                   m_pCamera, 
                                   m_pViewport);

    // Create our projected grid module  
    Hydrax::Module::Module* pModule = new Hydrax::Module::ProjectedGrid(// Hydrax parent pointer
                                                                        m_pHydrax,
                                                                        // Noise module
                                                                        new Hydrax::Noise::Perlin(/*Generic one*/),
                                                                        // Base plane
                                                                        Ogre::Plane(Ogre::Vector3(0,1,0), Ogre::Vector3(0,0,0)),
                                                                        // Normal mode
                                                                        Hydrax::MaterialManager::NM_VERTEX,
                                                                        // Projected grid options
                                                                        Hydrax::Module::ProjectedGrid::Options(264));

    // Set our module (m_pHydrax assumes ownership of pModule)
    m_pHydrax->setModule(pModule);

    // Load all parameters from config file
    // Remarks: The config file must be in Hydrax resource group.
    // All parameters can be set/updated directly by code(Like previous versions),
    // but due to the high number of customizable parameters, Hydrax 0.4 allows save/load config files.
    m_pHydrax->loadCfg(_configPath);

    // Create water
    m_pHydrax->create();

	// Add our hydrax depth technique to island material
	// (Because the terrain mesh is not an Ogre::Entity)
    m_pHydrax->getMaterialManager()->addDepthTechnique(
        static_cast<Ogre::MaterialPtr>(
            Ogre::MaterialManager::getSingleton().getByName("Terrain")
        )->createTechnique());
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
Water::~Water()
{
    // Remove water
    m_pHydrax->remove();

    // Destroy the m_pHydrax object.
    delete m_pHydrax;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
Water::update(Zen::Math::Real _timeDelta)
{
    // Update Hydrax (After any camera position/orientation/... change)
    m_pHydrax->update(_timeDelta);
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void 
Water::setPolygonMode(polygonMode_enum _mode)
{
    Ogre::PolygonMode mode;

    switch(_mode)
    {
    case Water::PMODE_SOLID:
        mode = Ogre::PM_SOLID;
        break;
    case Water::PMODE_WIREFRAME:
        mode = Ogre::PM_WIREFRAME;
        break;
    case Water::PMODE_POINT:
        mode = Ogre::PM_POINTS;
        break;
    default:
        mode = Ogre::PM_SOLID;
        break;
    };

    m_pHydrax->setPolygonMode(mode);
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
Water::polygonMode_enum 
Water::getPolygonMode() const
{
    Ogre::PolygonMode mode = m_pHydrax->getPolygonMode();

    switch(mode)
    {
    case Ogre::PM_SOLID:
        return Water::PMODE_SOLID;
        break;
    case Ogre::PM_WIREFRAME:
        return Water::PMODE_WIREFRAME;
        break;
    case Ogre::PM_POINTS:
        return Water::PMODE_POINT;
        break;
    default:
        return Water::PMODE_COUNT;
        break;
    };
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void 
Water::setShaderMode(shaderMode_enum _mode)
{
    Hydrax::MaterialManager::ShaderMode mode;

    switch(_mode)
    {
    case Water::SMODE_HLSL:
        mode = Hydrax::MaterialManager::SM_HLSL;
        break;
    case Water::SMODE_CG:
        mode = Hydrax::MaterialManager::SM_CG;
        break;
    case Water::SMODE_GLSL:
        mode = Hydrax::MaterialManager::SM_GLSL;
    };

    m_pHydrax->setShaderMode(mode);
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
Water::shaderMode_enum 
Water::getShaderMode() const
{
    Hydrax::MaterialManager::ShaderMode mode = m_pHydrax->getShaderMode();

    switch(mode)
    {
    case Hydrax::MaterialManager::SM_HLSL:
        return Water::SMODE_HLSL;
        break;
    case Hydrax::MaterialManager::SM_CG:
        return Water::SMODE_CG;
        break;
    case Hydrax::MaterialManager::SM_GLSL:
        return Water::SMODE_GLSL;
        break;
    default:
        return Water::SMODE_COUNT;
        break;
    };
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void 
Water::setPosition(const Math::Vector3& _position)
{
    Ogre::Vector3 position(_position.m_array);
    m_pHydrax->setPosition(position);
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
const Zen::Math::Vector3
Water::getPosition() const
{
    Ogre::Vector3 position(m_pHydrax->getPosition());
    return Zen::Math::Vector3(position.x,position.y,position.z);
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
Zen::Math::Real 
Water::getHeight(const Zen::Math::Vector3& _position) const
{
    Ogre::Vector3 position(_position.m_array);
    return m_pHydrax->getHeigth(position);
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void 
Water::rotate(const Math::Quaternion4& _q)
{
    Ogre::Quaternion q(_q.m_x,_q.m_y,_q.m_z,_q.m_w);
    m_pHydrax->rotate(q);
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void 
Water::setFullReflectionDistance(const Zen::Math::Real _distance)
{
    m_pHydrax->setFullReflectionDistance(_distance);
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
Zen::Math::Real 
Water::getFullReflectionDistance() const
{
    return m_pHydrax->getFullReflectionDistance();
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void 
Water::setGlobalTransparency(const Zen::Math::Real _transparency)
{
    m_pHydrax->setGlobalTransparency(_transparency);
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
Zen::Math::Real 
Water::getGlobalTransparency() const
{
    return m_pHydrax->getGlobalTransparency();
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void 
Water::setWaterColor(const Zen::Math::Vector3& _color)
{
    Ogre::Vector3 color(_color.m_array);
    m_pHydrax->setWaterColor(color);
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
const Zen::Math::Vector3
Water::getWaterColor() const
{
    Ogre::Vector3 color(m_pHydrax->getWaterColor());
    return Zen::Math::Vector3(color.x, color.y, color.z);
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void 
Water::setNormalDistortion(const Zen::Math::Real _distortion)
{
    m_pHydrax->setNormalDistortion(_distortion);
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
Zen::Math::Real 
Water::getNormalDistortion() const
{
    return m_pHydrax->getNormalDistortion();
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void 
Water::setSunPosition(const Zen::Math::Vector3& _position)
{
    Ogre::Vector3 position(_position.m_array);
    m_pHydrax->setSunPosition(position);
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
const Zen::Math::Vector3
Water::getSunPosition() const
{
    Ogre::Vector3 position(m_pHydrax->getSunPosition());
    return Zen::Math::Vector3(position.x, position.y, position.z);
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void 
Water::setSunStrength(const Zen::Math::Real _strength)
{
    m_pHydrax->setSunStrength(_strength);
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
Zen::Math::Real 
Water::getSunStrength() const
{
    return m_pHydrax->getSunStrength();
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void 
Water::setSunArea(const Zen::Math::Real _area)
{
    m_pHydrax->setSunArea(_area);
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
Zen::Math::Real 
Water::getSunArea() const
{
    return m_pHydrax->getSunArea();
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void 
Water::setSunColor(const Zen::Math::Vector3& _color)
{
    Ogre::Vector3 color(_color.m_array);
    m_pHydrax->setSunColor(color);
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
const Zen::Math::Vector3
Water::getSunColor() const
{
    Ogre::Vector3 color(m_pHydrax->getSunColor());
    return Zen::Math::Vector3(color.x, color.y, color.z);
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void 
Water::setFoamMaxDistance(const Zen::Math::Real _distance)
{
    m_pHydrax->setFoamMaxDistance(_distance);
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
Zen::Math::Real 
Water::getFoamMaxDistance() const
{
    return m_pHydrax->getFoamMaxDistance();
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void 
Water::setFoamScale(const Zen::Math::Real _scale)
{
    m_pHydrax->setFoamScale(_scale);
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
Zen::Math::Real 
Water::getFoamScale() const
{
    return m_pHydrax->getFoamScale();
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void 
Water::setFoamStart(const Zen::Math::Real _start)
{
    m_pHydrax->setFoamStart(_start);
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
Zen::Math::Real 
Water::getFoamStart() const
{
    return m_pHydrax->getFoamStart();
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void 
Water::setFoamTransparency(const Zen::Math::Real _transparency)
{
    m_pHydrax->setFoamTransparency(_transparency);
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
Zen::Math::Real 
Water::getFoamTransparency() const
{
    return m_pHydrax->getFoamTransparency();
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void 
Water::setDepthLimit(const Zen::Math::Real _depth)
{
    m_pHydrax->setDepthLimit(_depth);
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
Zen::Math::Real 
Water::getDepthLimit() const
{
    return m_pHydrax->getDepthLimit();
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void 
Water::setSmoothPower(const Zen::Math::Real _power)
{
    m_pHydrax->setSmoothPower(_power);
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
Zen::Math::Real 
Water::getSmoothPower() const
{
    return m_pHydrax->getSmoothPower();
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void 
Water::setCausticsScale(const Zen::Math::Real _scale)
{
    m_pHydrax->setCausticsScale(_scale);
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
Zen::Math::Real 
Water::getCausticsScale() const
{
    return m_pHydrax->getCausticsScale();
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void 
Water::setCausticsPower(const Zen::Math::Real _power)
{
    m_pHydrax->setCausticsPower(_power);
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
Zen::Math::Real 
Water::getCausticsPower() const
{
    return m_pHydrax->getCausticsPower();
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void 
Water::setCausticsEnd(const Zen::Math::Real _end)
{
    m_pHydrax->setCausticsEnd(_end);
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
Zen::Math::Real 
Water::getCausticsEnd() const
{
    return m_pHydrax->getCausticsEnd();
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void 
Water::setGodRaysExposure(const Zen::Math::Vector3& _exposure)
{
    Ogre::Vector3 exposure(_exposure.m_array);
    m_pHydrax->setGodRaysExposure(exposure);
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
const Zen::Math::Vector3
Water::getGodRaysExposure() const
{
    Ogre::Vector3 exposure(m_pHydrax->getGodRaysExposure());
    return Zen::Math::Vector3(exposure.x, exposure.y, exposure.z);
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void 
Water::setGodRaysIntensity(const Zen::Math::Real _intensity)
{
    m_pHydrax->setGodRaysIntensity(_intensity);
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
Zen::Math::Real 
Water::getGodRaysIntensity() const
{
    return m_pHydrax->getGodRaysIntensity();
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace ZOgre
}   // namespace Zen
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

#endif
