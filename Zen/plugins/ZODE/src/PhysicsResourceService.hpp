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
#ifndef ZEN_ZODE_PHYSICS_RESOURCE_SERVICE_HPP_INCLUDED
#define ZEN_ZODE_PHYSICS_RESOURCE_SERVICE_HPP_INCLUDED

#include <Zen/Engine/Resource/I_ResourceService.hpp>

#include <Zen/Core/Memory/managed_ptr.hpp>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Zen {
    namespace Threading {
        class I_Mutex;
    }   // namespace Threading
namespace ZODE {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

class PhysicsResourceService
:   public Engine::Resource::I_ResourceService
{
    /// @name Types
    /// @{
public:
    typedef Zen::Memory::managed_ptr<Scripting::I_ScriptModule> pScriptModule_type;
    /// @}

    /// @name I_ResourceService implementation
    /// @{
public:
    virtual void addResourceLocation(const std::string& _path, const std::string& _type,
        const std::string& _group, bool _recursive = false);
    virtual void removeResourceLocation(const std::string& _path, const std::string& _group);

    virtual pResource_type loadResource(config_type& _config);
    
    virtual void getResourceNames(I_ResourceNameVisitor& _visitor, const std::string& _group, const std::string& _pattern) const;

    virtual void initialiseAllResourceGroups();
    virtual void loadResourceGroup(const std::string& _group);
    virtual void unloadResourceGroup(const std::string& _group);

    virtual const std::string& getScriptSingletonName() const;
    virtual void registerScriptModule(Zen::Scripting::script_module& _module);
    /// @}

    /// @name I_ScriptableType implementation
    /// @{
public:
    virtual Scripting::I_ObjectReference* getScriptObject();
    /// @}

    /// @name ResourceService implementation
    /// @{
private:
    void destroyResource(wpResource_type);
    /// @}

    /// @name 'Structors
    /// @{
public:
             PhysicsResourceService();
    virtual ~PhysicsResourceService();
    /// @}

    /// @name Member Variables
    /// @{
private:
    /// True if the group manager has been initialized
    volatile bool                   m_bInitialized;
    Threading::I_Mutex*             m_pGroupInitLock;

    Zen::Scripting::script_module*  m_pModule;
    ScriptObjectReference_type*     m_pScriptObject;
    /// @}

};  // class PhysicsResourceService

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace ZODE
}   // namespace Zen
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

#endif // ZEN_ZODE_PHYSICS_RESOURCE_SERVICE_HPP_INCLUDED
