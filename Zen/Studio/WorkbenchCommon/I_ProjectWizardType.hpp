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
#ifndef ZENSTUDIO_WORKBENCH_I_PROJECT_WIZARD_TYPE_HPP_INCLUDED
#define ZENSTUDIO_WORKBENCH_I_PROJECT_WIZARD_TYPE_HPP_INCLUDED

#include <boost/noncopyable.hpp>

#include <string>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Zen {
namespace Studio {
namespace Workbench {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
class I_DocumentType;

/// @brief Project Wizard Type
/// This class represents the details of a project wizard
/// type that can be used by the Workbench.  Until
/// the extension is actually used, the plugin will not
/// be loaded.  This data comes from the plugin.xml
///
///    <project-wizard
///          class="Zen::Studio::WorldProjectWizard"
///          type="world"
///          name="World Project">
///    </project-wizard>
class I_ProjectWizardType
:   boost::noncopyable
{
    /// @name Types
    /// @{
public:
    /// @}

    /// @name I_ProjectWizardType interface
    /// @{
public:
    /// Get the name of this project type
    virtual const std::string& getName() const = 0;

    /// Get the name of the class of the I_ProjectService that implements
    /// this project type.
    virtual const std::string& getClassName() const = 0;

    /// Get the extension point type to pass to the I_ProjectServiceManager to 
    /// construct the appropriate I_ProjectService
    virtual const std::string& getType() const = 0;
    /// @}

    /// @name 'Structors
    /// @{
protected:
             I_ProjectWizardType();
    virtual ~I_ProjectWizardType();    
    /// @}

    /// @name Inner Classes
    /// @{
public:
    /// @}

};  // interface I_ProjectWizardType

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace Workbench
}   // namespace Studio
}   // namespace Zen
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

#endif // ZENSTUDIO_WORKBENCH_I_PROJECT_WIZARD_TYPE_HPP_INCLUDED
