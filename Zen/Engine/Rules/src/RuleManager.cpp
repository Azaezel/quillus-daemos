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
#include "RuleManager.hpp"
#include "../I_RuleService.hpp"
#include "../I_StateMachine.hpp"

#include <Zen/Core/Plugins/I_ExtensionRegistry.hpp>
#include <Zen/Core/Plugins/I_ExtensionPoint.hpp>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Zen {
namespace Engine {
namespace Rules {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
RuleManager::RuleManager()
{
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
RuleManager::~RuleManager()
{
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
RuleManager::pRuleService_type
RuleManager::createRuleService(const std::string& _type)
{
    Plugins::I_ExtensionRegistry::pExtensionPoint_type pExtensionPoint = Plugins::I_ExtensionRegistry::getSingleton().getExtensionPoint(I_RuleService::getNamespace(), I_RuleService::getExtensionPointName());

    typedef Plugins::I_ExtensionPoint::pExtension_type      pExtension_type;

    struct ExtensionVisitor : public Plugins::I_ExtensionPoint::I_ExtensionVisitor
    {
        virtual void begin() {}

        virtual void visit(pExtension_type _pExtension)
        {
            const Plugins::I_ConfigurationElement& config = _pExtension->getConfigurationElement();

            std::string name = config.getName();

            Plugins::I_ConfigurationElement const * const pProjectConfig = config.getChild("project");
        }


        virtual void end() {}

        ExtensionVisitor(const std::string& _type) : m_type(_type)
        {
        }

        const std::string& m_type;
        pRuleService_type   m_pRuleService;
    };

    ExtensionVisitor extensionVisitor(_type);

    pExtensionPoint->getExtensions(extensionVisitor);

    return extensionVisitor.m_pRuleService;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
RuleManager::pStateMachine_type
RuleManager::createStateMachine()
{
    return pStateMachine_type();
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace Rules
}   // namespace Engine
}   // namespace Zen
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
