//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
// Zen Studio
//
// Copyright (C) 2001 - 2008 Tony Richards
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
#ifndef ZENSTUDIO_WORKBENCH_I_DOCUMENT_WIZARD_MANAGER_HPP_INCLUDED
#define ZENSTUDIO_WORKBENCH_I_DOCUMENT_WIZARD_MANAGER_HPP_INCLUDED

#include "Configuration.hpp"

#include <Zen/Core/Memory/managed_ptr.hpp>
#include <Zen/Core/Memory/managed_weak_ptr.hpp>
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Zen {
namespace Studio {
namespace Workbench {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
class I_DocumentWizardService;

class WORKBENCH_DLL_LINK I_DocumentWizardManager
{
	/// @name Types
	/// @{
public:
	typedef Zen::Memory::managed_ptr<I_DocumentWizardService> pDocumentWizardService_type;
	typedef Zen::Memory::managed_weak_ptr<I_DocumentWizardService> wpDocumentWizardService_type;
	/// @}
	
	/// @name I_DocumentWizardManager.hpp interface
	/// @{
public:
	// Create a document wizard
	virtual pDocumentWizardService_type create(const std::string& _type) = 0; 
	/// @}

	/// @name Static methods
	/// @{
public:
	static I_DocumentWizardManager& getSingleton();
	/// @}

	/// @name 'Structors
	/// @{
protected:
			 I_DocumentWizardManager();
	virtual ~I_DocumentWizardManager();
	/// @}
};  // interface I_DocumentWizardManager
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace Workbench
}   // namespace Studio
}   // namespace Zen
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
#endif // ZENSTUDIO_WORKBENCH_I_DOCUMENT_WIZARD_MANAGER_HPP_INCLUDED