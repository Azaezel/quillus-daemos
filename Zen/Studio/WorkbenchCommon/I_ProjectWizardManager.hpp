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
#ifndef ZENSTUDIO_WORKBENCH_I_PROJECT_WIZARD_MANAGER_HPP_INCLUDED
#define ZENSTUDIO_WORKBENCH_I_PROJECT_WIZARD_MANAGER_HPP_INCLUDED

#include "Configuration.hpp"

#include <Zen/Core/Memory/managed_ptr.hpp>
#include <Zen/Core/Memory/managed_weak_ptr.hpp>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Zen {
namespace Studio {
namespace Workbench {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
class I_ProjectWizardService;
class I_ProjectType;
class I_ProjectWizardType;

class WORKBENCHCOMMON_DLL_LINK I_ProjectWizardManager
{
	/// @name Types
	/// @{
public:
    typedef Zen::Memory::managed_ptr<I_ProjectWizardService>        pProjectWizardService_type;
    typedef Zen::Memory::managed_weak_ptr<I_ProjectWizardService>   wpProjectWizardService_type;
    
    struct I_WizardVisitor;
    /// @}

    /// @name I_ProjectWizardManager interface
    /// @{
public:
    /// Get all of the wizards that can handle this project type.
    virtual void getWizardsForType(const I_ProjectType& _type, I_WizardVisitor& _visitor) = 0;

    /// Create a project wizard
    virtual pProjectWizardService_type create(I_ProjectWizardType& _wizardType) = 0;
    /// @}

    /// @name Static Methods
    /// @{
public:
    static I_ProjectWizardManager& getSingleton();
    /// @}

    /// @name 'Structors
    /// @{
protected:
             I_ProjectWizardManager();
    virtual ~I_ProjectWizardManager();
    /// @}

    /// @name Inner Classes
    /// @{
public:
    struct I_WizardVisitor
    {
        virtual void begin() = 0;
        virtual void visit(const I_ProjectWizardType& _wizardType) = 0;
        virtual void end() = 0;
    };  // interface I_WizardVisitor
    /// @}
};  // interface I_ProjectWizardManager

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace Workbench
}   // namespace Studio
}   // namespace Zen
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

#endif  // ZENSTUDIO_WORKBENCH_I_PROJECT_WIZARD_MANAGER_HPP_INCLUDED
