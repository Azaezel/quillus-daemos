//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
// Zen Studio
//
// Copyright (C) 2001 - 2008 Tony Richards
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
#ifndef ZENSTUDIO_LANGUAGES_I_CLASS_DEFINITION_HPP_INCLUDED
#define ZENSTUDIO_LANGUAGES_I_CLASS_DEFINITION_HPP_INCLUDED

#include "Configuration.hpp"

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Zen {
namespace Studio {
namespace Languages {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

class WORKBENCH_DLL_LINK I_ClassDefinition
{
    /// @name Types
    /// @{
public:
    class I_RenamePayload;
    typedef Memory::managed_ptr<I_RenamePayload>    pRenamePayload_type;
    typedef Event::Event<pRenamePayload_type>       RenameEvent_type;
    /// @}

    /// @name I_ClassDefinition interface
    /// @{
public:
    /// Get the name of this class
    virtual const std::string& getName() = 0;

    /// Query the methods that exist in this class and pass them
    /// to the visitor.
    virtual void queryMethods(pMethodVisitor_type _pVisitor) = 0;

    /// Query the methods that exist in this class and return 
    /// them in a result set.
     virtual pMethodResultSet_type queryMethods() = 0;
 
     /// Query the variables that exist in this class and pass them
     /// to the visitor.
     virtual void queryVariables(pVariableVisitor_type _pVisitor) = 0;

     /// Query the variables that exist in this class and return
     /// them in a result set.
     virtual pVariableResultSet_type queryVariables() = 0;

    virtual void queryInterfaces(pInterfaceVisitor_type _pVisitor) = 0;
    virtual void querySuperClasses(pClassVisitor_type _pVisitor) = 0;

    virtual void queryDesignPatterns(pDesignPatternVisitor_type _pVisitor) = 0;
    

    virtual bool isGeneric() = 0;
    /// @}

    /// @name Events
    /// @{
public:
    RenameEvent_type    onRenameEvent;
    /// @}

    /// @name 'Structors
    /// @{
protected:
             I_ClassDefinition();
    virtual ~I_ClassDefinition();
    /// @}

};  // interface I_ClassDefinition

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace Languages
}   // namespace Studio
}   // namespace Zen
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

#endif // ZENSTUDIO_LANGUAGES_I_CLASS_DEFINITION_HPP_INCLUDED
