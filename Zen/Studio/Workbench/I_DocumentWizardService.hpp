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
#ifndef ZENSTUDIO_WORKBENCH_I_DOCUMENT_WIZARD_SERVICE_HPP_INCLUDED
#define ZENSTUDIO_WORKBENCH_I_DOCUMENT_WIZARD_SERVICE_HPP_INCLUDED

#include "Configuration.hpp"

#include <Zen/Core/Memory/managed_ptr.hpp>
#include <Zen/Core/Memory/managed_weak_ptr.hpp>

#include <Zen/Core/Event/Event.hpp>

#include <string>
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Zen {
namespace Studio {
namespace Workbench {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
class I_DocumentWizardFactory;
class I_DocumentWizardService;

class WORKBENCH_DLL_LINK I_DocumentWizardService
{
	/// @name Types
	/// @{
public:
	typedef std::string										  index_type;
	typedef I_DocumentWizardFactory							  factory_type;

	typedef Memory::managed_ptr<I_DocumentWizardService>	  pService_type;
	typedef Memory::managed_weak_ptr<I_DocumentWizardService> wpService_type;
	typedef Event::Event<wpService_type>					  serviceEvent_type;
	/// @}

	/// @name I_DocumentWizardService.hpp interface
	/// @{
public:
	/// @}

	/// @name Static methods
	/// @{
public:
	static const std::string& getNamespace();
	static const std::string& getExtensionPointName();
	/// @}

	/// @name Events
	/// @{
public:
	serviceEvent_type onDestroyEvent;
	/// @}

	/// @name 'Structors
	/// @{
protected:
			I_DocumentWizardService();
	virtual ~I_DocumentWizardService();
	/// @}

};  // interface I_DocumentWizardService
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}	// namespace Workbench
}	// namespace Studio
namespace Memory {
	/// I_DocumentWizardService is managed by I_DocumentWizardManager
	template<>
	struct is_managed_by_factory<Studio::Workbench::I_DocumentWizardService> : public boost::true_type{};
}	// namespace Memory
}	// namespace Zen
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
#endif // ZENSTUDIO_WORKBENCH_I_DOCUMENT_WIZARD_SERVICE_HPP_INCLUDED