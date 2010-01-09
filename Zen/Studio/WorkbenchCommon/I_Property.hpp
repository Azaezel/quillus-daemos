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
#ifndef ZENSTUDIO_WORKBENCH_I_PROPERTY_HPP_INCLUDED
#define ZENSTUDIO_WORKBENCH_I_PROPERTY_HPP_INCLUDED

#include "Configuration.hpp"

#include <boost/noncopyable.hpp>

#include <string>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Zen {
namespace Studio {
namespace Workbench {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
class I_PropertiesPublisher;

/// Property interface.
class WORKBENCHCOMMON_DLL_LINK I_Property
:   public boost::noncopyable
{
    /// @name Types
    /// @{
public:
    enum PropertyType
    {
        CATEGORY_TYPE,
        TRUE_FALSE_PROPERTY,
        ENUM_TYPE,
        STRING_TYPE
    };
    /// @}

    /// @name I_Property interface
    /// @{
public:
    /// Get the name of this property.
    virtual const std::string& getName() const = 0;

    /// Get the fully qualified name of this property.
    /// This name is the fully qualified name using "\"
    /// as a separator.
    virtual const std::string& getFullName() const = 0;

    /// Get the value as a string.
    virtual const std::string& getValue() const = 0;

    /// Get the type of this property.
    virtual PropertyType getType() const = 0;

    /// Get the parent of this property.
    /// @return NULL if this property does not have a parent, otherwise
    ///     the parent is returned.
    virtual I_Property* getParent() = 0;

    /// Get the publisher of this property.
    virtual I_PropertiesPublisher& getPublisher() = 0;
    /// @}

    /// @name 'Structors
    /// @{
protected:
             I_Property();
    virtual ~I_Property();
    /// @}

};  // interface I_Property

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace Workbench
}   // namespace Studio
}   // namespace Zen
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

#endif // ZENSTUDIO_WORKBENCH_I_PROPERTY_HPP_INCLUDED
