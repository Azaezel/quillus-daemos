//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
// Zen Studio
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

#include "ProjectWizardManager.hpp"
#include "ProjectWizardType.hpp"
#include "ProjectType.hpp"

#include <Zen/Core/Plugins/I_ExtensionRegistry.hpp>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Zen {
namespace Studio {
namespace Workbench {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
ProjectWizardManager::ProjectWizardManager()
{
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
ProjectWizardManager::~ProjectWizardManager()
{
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
ProjectWizardManager::getWizardsForType(const I_ProjectType& _type, I_ProjectWizardManager::I_WizardVisitor& _visitor)
{
    _visitor.begin();

    Plugins::I_ExtensionRegistry::pExtensionPoint_type pExtensionPoint = Plugins::I_ExtensionRegistry::getSingleton().getExtensionPoint(I_ProjectWizardService::getNamespace(), I_ProjectWizardService::getExtensionPointName());

    /// Get the Project Wizard extensions

    struct ExtensionVisitor : public Plugins::I_ExtensionPoint::I_ExtensionVisitor
    {
        virtual void begin()
        {
        }

        virtual void visit(pExtension_type _pExtension)
        {
            const Plugins::I_ConfigurationElement& config = _pExtension->getConfigurationElement();

            std::string name = config.getName();

            Plugins::I_ConfigurationElement const * const pProjectConfig = config.getChild("project-type");
            ProjectWizardType projectWizardType(pProjectConfig, _pExtension);

            if (projectWizardType.getClassName() == m_type.getClassName())
            {
                // Pass the wizard type on to the visitor
                m_visitor.visit(projectWizardType);
            }
        }

        virtual void end()
        {
        }

        ExtensionVisitor(const I_ProjectType& _type, I_ProjectWizardManager::I_WizardVisitor& _visitor)
            :   m_type(_type)
            ,   m_visitor(_visitor)
        {}

        const I_ProjectType&                        m_type; 
        I_ProjectWizardManager::I_WizardVisitor&    m_visitor;
    };

    ExtensionVisitor extensionVisitor(_type, _visitor);
    pExtensionPoint->getExtensions(extensionVisitor);

    _visitor.end();
}
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
I_ProjectWizardManager::pProjectWizardService_type
ProjectWizardManager::create(I_ProjectWizardType& _wizardType)
{
    const std::string& type(_wizardType.getType());

    Threading::CriticalSection guard(m_serviceCache.getLock());

    pProjectWizardService_type pService(m_serviceCache.getCachedService(type));

    if(pService.isValid())
    {
        return pService;
    }

    I_ProjectWizardFactory* pFactory = m_serviceCache.getFactory(type);

    if(pFactory == NULL)
    {
        // TODO: Error
        return pService;
    }

    return m_serviceCache.cacheService(type, pFactory->create(type));
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace Workbench
}   // namespace Studio
}   // namespace Zen
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
