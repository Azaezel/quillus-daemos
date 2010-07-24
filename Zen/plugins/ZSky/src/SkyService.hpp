//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
// IndieZen Game Engine Framework
//
// Copyright (C) 2001 - 2010 Tony Richards
// Copyright (C) 2008 - 2010 Matthew Alan Gray
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
#ifndef ZEN_ZSKY_SKY_SERVICE_HPP_INCLUDED
#define ZEN_ZSKY_SKY_SERVICE_HPP_INCLUDED

#include <Zen/Engine/World/I_SkyService.hpp>

#include <Zen/Core/Memory/managed_self_ref.hpp>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Zen {
namespace ZSky {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

class SkyService
:   public Engine::World::I_SkyService
,   public Memory::managed_self_ref<Zen::Engine::World::I_SkyService>
{
    /// @name Types
    /// @{
public:
    typedef Zen::Memory::managed_ptr<Zen::Scripting::I_ScriptModule>    pScriptModule_type;
    /// @}

    /// @name I_SkyService implementation
    /// @{
public:
    virtual void setPhysicsService(pPhysicsService_type _pService);
    virtual void setPhysicsResourceService(pPhysicsResourceService_type _pService);
    virtual void setRenderingService(pRenderingService_type _pService);
    virtual void setRenderingResourceService(pRenderingResourceService_type _pService);
    virtual pSky_type createSky(config_type& _config);
    virtual void registerScriptModule(Zen::Scripting::script_module& _module);
    /// @}

    /// @name I_ScriptableType implementation
    /// @{
public:
    virtual const std::string& getScriptTypeName();
    virtual Zen::Scripting::I_ObjectReference* getScriptObject();
    /// @}

    /// @name SkyService implementation
    /// @{
public:
    void onDestroySky(wpSky_type _wpSky);
    /// @}

    /// @name Event handlers
    /// @{
protected:
    /// @}

    /// @name 'Structors
    /// @{
public:
             SkyService();
    virtual ~SkyService();
    /// @}

    /// @name Member Variables
    /// @{
private:
    pPhysicsService_type            m_pPhysicsService;
    pPhysicsResourceService_type    m_pPhysicsResourceService;
    pRenderingService_type          m_pRenderingService;
    pRenderingResourceService_type  m_pRenderingResourceService;

    ScriptObjectReference_type*     m_pScriptObject;

    /// @}

};  // class SkyService

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace ZSky
}   // namespace Zen
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

#endif // ZEN_ZSKY_SKY_SERVICE_HPP_INCLUDED
