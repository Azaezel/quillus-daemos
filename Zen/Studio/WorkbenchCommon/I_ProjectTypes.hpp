//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
// Zen Studio
//
// Copyright (C) 2001 - 2009 Tony Richards
// Copyright (C) 2008 - 2009 Matthew Alan Gray
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
//  Matthew Alan Gray mgray@indiezen.org
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
#ifndef ZENSTUDIO_WORKBENCH_I_PROJECT_TYPES_HPP_INCLUDED
#define ZENSTUDIO_WORKBENCH_I_PROJECT_TYPES_HPP_INCLUDED

#include "Configuration.hpp"

#include <Zen/Core/Event/Event.hpp>

#include <boost/noncopyable.hpp>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Zen {
namespace Studio {
namespace Workbench {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
class WORKBENCHCOMMON_DLL_LINK I_ProjectType;

/// @brief Collection of Project Types
class WORKBENCHCOMMON_DLL_LINK I_ProjectTypes
:   boost::noncopyable
{
    /// @name Types
    /// @{
public:
    /// Project type visitor, defined below
    struct I_ProjectTypeVisitor;

    typedef Zen::Event::Event<I_ProjectType&>       ProjectTypeEvent_type;
    /// @}

    /// @name I_ProjectTypes interface
    /// @{
public:
    virtual void getProjectTypes(I_ProjectTypeVisitor& _visitor) = 0;
    /// @}

    /// @name 

    /// @name Events
    /// @{
public:
    ProjectTypeEvent_type   onNewProjectType;
    /// @}

    /// @name Inner Classes
    /// @{
public:
    struct I_ProjectTypeVisitor
    {
        virtual void begin() = 0;
        virtual void visit(const I_ProjectType& _projectType) = 0;
        virtual void end() = 0;
    };  // interface I_ProjectTypeVisitor
    /// @}

    /// @name Static factory method
    /// @{
public:
    static I_ProjectTypes* create();
    /// @}

    /// @name 'Structors
    /// @{
protected:
             I_ProjectTypes();
    virtual ~I_ProjectTypes();    
    /// @}

};  // interface I_ProjectTypes

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace Workbench
}   // namespace Studio
}   // namespace Zen
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

#endif // ZENSTUDIO_WORKBENCH_I_PROJECT_TYPES_HPP_INCLUDED
