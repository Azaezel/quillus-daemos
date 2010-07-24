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
#ifndef ZENSTUDIO_WORKBENCH_PROJECT_TYPES_HPP_INCLUDED
#define ZENSTUDIO_WORKBENCH_PROJECT_TYPES_HPP_INCLUDED

#include "../II_ProjectTypes.hpp"

#include <Zen/Core/Plugins/I_ExtensionPoint.hpp>
#include <Zen/Core/Threading/I_Thread.hpp>
#include <Zen/Core/Threading/I_Mutex.hpp>

#include <list>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Zen {
namespace Studio {
namespace Workbench {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
class ProjectType;

/// @brief Collection of Project Types
///
/// This class contains a collection of project types.
/// It also has the logic and the worker thread for getting
/// all of the project types from the currently installed plugins.
class ProjectTypes
:   public Internal::II_ProjectTypes
{
    /// @name Types
    /// @{
public:
    typedef Plugins::I_ExtensionPoint::pExtension_type      pExtension_type;
    /// @}

    /// @name I_ProjectTypes implementation
    /// @{
public:
    virtual void getProjectTypes(I_ProjectTypeVisitor& _visitor);
    virtual void init();
    /// @}

    /// @name ProjectTypes implementation
    /// @{
public:
    /// @}

    /// @name 'Structors
    /// @{
protected:
    friend class I_ProjectTypes;
             ProjectTypes();
    virtual ~ProjectTypes();    
    /// @}

    /// @name Member Variables
    /// @{
private:
    friend class ThreadedExtensionPointVisitor;

    //I_Workbench*        m_pParent;

    typedef std::list<ProjectType*>  collection_type;
    collection_type         m_projectTypes;

    Zen::Threading::I_Thread*   m_pThread;

    /// Guard for m_projectTypes
    Zen::Threading::I_Mutex*    m_pMutex;
    /// @}

};  // class Notebook

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace Workbench
}   // namespace Studio
}   // namespace Zen
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

#endif // ZENSTUDIO_WORKBENCH_PROJECT_TYPES_HPP_INCLUDED
