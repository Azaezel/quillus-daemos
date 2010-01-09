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
#ifndef ZENSTUDIO_WORKBENCH_PROJECT_WIZARD_MANAGER_HPP_INCLUDED
#define ZENSTUDIO_WORKBENCH_PROJECT_WIZARD_MANAGER_HPP_INCLUDED

#include "../I_ProjectWizardManager.hpp"
#include "../I_ProjectWizardFactory.hpp"
#include "../I_ProjectWizardService.hpp"

#include <Zen/Core/Plugins/ServiceCache.hpp>
#include <Zen/Core/Plugins/I_ExtensionPoint.hpp>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Zen {
namespace Studio {
namespace Workbench {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

class ProjectWizardManager
:   public I_ProjectWizardManager
{
	/// @name Types
	/// @{
public:
    typedef Zen::Plugins::ServiceCache<I_ProjectWizardService>      ServiceCache_type;

    typedef Plugins::I_ExtensionPoint::pExtension_type              pExtension_type;
    /// @}

    /// @name ProjectWizardManager implementation
    /// @{
public:
    virtual void getWizardsForType(const I_ProjectType& _type, I_WizardVisitor& _visitor);
    virtual pProjectWizardService_type create(I_ProjectWizardType& _wizardType);
    /// @}

    /// @name 'Structors
    /// @{
public:
             ProjectWizardManager();
    virtual ~ProjectWizardManager();
    /// @}

    /// @name Member Variables
    /// @{
private:
    ServiceCache_type       m_serviceCache;
    /// @}

};  // interface ProjectWizardManager

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace Workbench
}   // namespace Studio
}   // namespace Zen
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

#endif  // ZENSTUDIO_WORKBENCH_PROJECT_WIZARD_MANAGER_HPP_INCLUDED
