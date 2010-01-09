//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
// Zen Game Engine Framework
//
// Copyright (C) 2001 - 2009 Tony Richards
// Copyright (C)		2009 Denis Hilliard
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
#ifndef ZEN_ENGINE_WIDGETS_I_STYLE_SERVICE_HPP_INCLUDED
#define ZEN_ENGINE_WIDGETS_I_WIDGET_SERVICE_HPP_INCLUDED

#include "Configuration.hpp"

#include <Zen/Core/Memory/managed_ptr.hpp>
#include <Zen/Core/Memory/managed_weak_ptr.hpp>

#include <string>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Zen {
namespace Engine {
namespace Widgets {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
class I_Widget;
class I_WidgetStyle;

class WIDGETS_DLL_LINK I_StyleService
{
	/// @name Types
	/// @{
public:
	typedef std::string                                 index_type;
				
	/// @}
				
	/// @name I_StyleService interface
	/// @{
public:
	/// Style the widget
	virtual void styleWidget(I_Widget & _widget, const std::string & _path, const std::string & _type) = 0;
	/// Add widget decorator passing ownership to service
	virtual void addStyle(I_WidgetStyle * _decorator, const std::string & _type) = 0;
	/// Get widget decorator by type
	virtual I_WidgetStyle & getStyle(const std::string & _type) = 0;
	/// Destroy widget decorator by type
	virtual void destroyStyle(const std::string & _type) = 0;
				
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

	/// @}    
	/// @name 'Structors
	/// @{
protected:
		I_StyleService();
	virtual ~I_StyleService();
	/// @}
				
};	// interface I_StyleService
			
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace Widgets
}   // namespace Engine
namespace Memory {
/// I_StyleService is managed by I_StyleServiceFactory
	template<>
	struct is_managed_by_factory<Engine::Widgets::I_StyleService> : public boost::true_type{};
}   // namespace Memory
}   // namespace Zen
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

#endif // ZEN_ENGINE_WIDGETS_I_STYLE_SERVICE_HPP_INCLUDED


