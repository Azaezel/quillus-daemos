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
#ifndef ZENSTUDIO_WORKBENCH_PROJECT_WIZARD_TYPE_HPP_INCLUDED
#define ZENSTUDIO_WORKBENCH_PROJECT_WIZARD_TYPE_HPP_INCLUDED

#include "../I_ProjectWizardType.hpp"

#include <Zen/Core/Plugins/I_ExtensionPoint.hpp>
#include <Zen/Core/Threading/I_Thread.hpp>
#include <Zen/Core/Threading/I_Mutex.hpp>

#include <list>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Zen {
namespace Studio {
namespace Workbench {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
class I_Workbench;
class DocumentType;

/// @brief Project Type
class ProjectWizardType
:   public I_ProjectWizardType
{
    /// @name Types
    /// @{
public:
    typedef Plugins::I_ExtensionPoint::pExtension_type      pExtension_type;
    typedef std::list<DocumentType*>                        DocumentTypes_type;
    /// @}

    /// @name I_ProjectWizardType implementation
    /// @{
public:
    virtual const std::string& getName() const;
    virtual const std::string& getClassName() const;
    virtual const std::string& getType() const;
    /// @}

    /// @name ProjectWizardType implementation
    /// @{
protected:
    /// @}

    /// @name 'Structors
    /// @{
public:
             ProjectWizardType(Plugins::I_ConfigurationElement const* const _pConfig, pExtension_type _pExtension);
    virtual ~ProjectWizardType();    
    /// @}

    /// @name Member Variables
    /// @{
private:
    /// Extension that defines this project type
    pExtension_type                 m_pExtension;

    /// Name of this project type
    std::string                     m_name;

    /// C++ Class that represents a I_ProjectService of this type
    std::string                     m_class;

    /// Shortcut string to pass to I_ProjectServiceManager to construct
    /// the appropriate I_ProjectService
    std::string                     m_type;
    /// @}

};  // class ProjectWizardType

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace Workbench
}   // namespace Studio
}   // namespace Zen
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

#endif // ZENSTUDIO_WORKBENCH_PROJECT_WIZARD_TYPE_HPP_INCLUDED
