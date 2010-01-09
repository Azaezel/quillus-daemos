//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
// Zen Studio
//
// Copyright (C) 2001 - 2009 Tony Richards
// Copyright (C)        2008 Steen Rasmussen
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
//  Steen Rasmussen steenrasmussen@gmail.com
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
#ifndef ZENSTUDIO_WORKBENCH_DOCUMENT_WIZARD_MANAGER_HPP_INCLUDED
#define ZENSTUDIO_WORKBENCH_DOCUMENT_WIZARD_MANAGER_HPP_INCLUDED

#include "../I_DocumentWizardFactory.hpp"

#include "../I_DocumentWizardManager.hpp"

#include "../I_DocumentWizardService.hpp"

#include <Zen/Core/Plugins/ServiceCache.hpp>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Zen {
namespace Studio {
namespace Workbench {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
class DocumentWizardManager
:	public I_DocumentWizardManager
{
	/// @name Types
	/// @{
public:
	typedef Zen::Plugins::ServiceCache<I_DocumentWizardService> ServiceCache_type;
	/// @}

	/// @name DocumentWizardManager implementation
	/// @{
public:
	virtual pDocumentWizardService_type create(const std::string& _type);	
	/// @}

	/// @name 'Structors
	/// @{
public:
			DocumentWizardManager();
	virtual ~DocumentWizardManager();
	/// @}

	/// @name Member variables
	/// @{
private:
	ServiceCache_type m_serviceCache;
	/// @}

}; // interface DocumentWizardManager

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
} // Workbench
} // Studio
} // Zen
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

#endif // ZENSTUDIO_WORKBENCH_DOCUMENT_WIZARD_MANAGER_HPP_INCLUDED