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
#ifndef WORKBENCH_I_CONTRIBUTOR_SERVICE_FACTORY_HPP_INCLUDED
#define WORKBENCH_I_CONTRIBUTOR_SERVICE_FACTORY_HPP_INCLUDED

#include "Configuration.hpp"

#include <Zen/Core/Memory/managed_ptr.hpp>

#include <Zen/Core/Plugins/I_ClassFactory.hpp>

#include <string>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Zen {
namespace Studio {
namespace Workbench {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
class I_Workbench;
class I_ContributorService;

class WORKBENCH_DLL_LINK I_ContributorServiceFactory
:   public Plugins::I_ClassFactory
{
    /// @name Types
    /// @{
public:
    typedef Memory::managed_ptr<I_ContributorService>    pContributorService_type;
    /// @}

    /// @name I_ContributorServiceFactory interface
    /// @{
public:
    /// Create a contributor service.
    /// @param _type The type of contributor service.  This is generally the extension
    ///     point name.
    /// @param _workbench The main workbench that is creating the contributor service.
    virtual pContributorService_type create(const std::string& _type, I_Workbench& _workbench) = 0;
    /// @}

    /// @name 'Structors
    /// @{
protected:
             I_ContributorServiceFactory();
    virtual ~I_ContributorServiceFactory();
    /// @}

};  // interface I_ContributorServiceFactory

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace Workbench
}   // namespace Studio
}   // namespace Zen
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

#endif // WORKBENCH_I_CONTRIBUTOR_SERVICE_FACTORY_HPP_INCLUDED
