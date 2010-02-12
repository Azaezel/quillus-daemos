//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
// Zen Game Engine Framework
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
#include "../ZOgreModule.hpp"

#include "ZOgrePlugin.hpp"

#include "Ogre.hpp"

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Zen {
namespace ZOgre {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

class ZOgreModule
:   public Plugins::I_Module
{
    /// @name Types
    /// @{
public:
    typedef boost::shared_ptr<ZOgrePlugin>     plugin_ptr_type;
    /// @}

    /// @name I_Module implementation
    /// @{
public:
    virtual plugin_name_collection_type& getPluginNames();
    virtual Plugins::I_Module::plugin_ptr_type getPlugin(const plugin_name_type& _pluginName);
    virtual void destroyPlugin(Plugins::I_Module::plugin_ptr_type _plugin);
    /// @}

    /// @name 'Structors
    /// @{
public:
             ZOgreModule();
    virtual ~ZOgreModule();
    /// @}

    /// @name Implementation
    /// @{
public:
    static ZOgreModule& instance();
    /// @}

    /// @name Member Variables
    /// @{
private:
    plugin_name_collection_type                 m_names;
    plugin_ptr_type                             m_pPlugin;
    /// @}

};  // class ZOgreModule

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
ZOgreModule::ZOgreModule()
{
    // TODO: Register the plugins
    m_pPlugin = plugin_ptr_type(new ZOgrePlugin);
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
ZOgreModule::~ZOgreModule()
{
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
static ZOgreModule* pModule = NULL;
static Ogre::Root* pRoot = NULL;

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
ZOgreModule& 
ZOgreModule::instance()
{
    // TODO: Guard
    if (pModule == NULL)
    {
        pModule = new ZOgreModule();
    }

    if (pRoot == NULL)
    {
#ifdef _DEBUG
	    pRoot = new Ogre::Root("plugins-debug.cfg", "ogre.cfg", "Ogre.log");
#else
	    pRoot = new Ogre::Root("plugins-release.cfg", "ogre.cfg", "Ogre.log");
#endif
    }

    return *pModule;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
Zen::Plugins::I_Module::plugin_name_collection_type& 
ZOgreModule::getPluginNames()
{
    return m_names;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
Plugins::I_Module::plugin_ptr_type
ZOgreModule::getPlugin(const plugin_name_type& _PluginName)
{
    // TODO: Return the appropriate plugin; 
    // For now since only one type is supported then return that.
    return m_pPlugin;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void 
ZOgreModule::destroyPlugin(Plugins::I_Module::plugin_ptr_type _pPlugin)
{
    // TODO delete?
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace ZOgre
}   // namespace Zen
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
extern "C"
ZOGRE_DLL_LINK
Zen::Plugins::I_Module&
getModule()
{
    return Zen::ZOgre::ZOgreModule::instance();
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
