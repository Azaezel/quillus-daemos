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
#ifndef ZENSTUDIO_WORKBENCH_I_PROJECT_TYPE_HPP_INCLUDED
#define ZENSTUDIO_WORKBENCH_I_PROJECT_TYPE_HPP_INCLUDED

#include <boost/noncopyable.hpp>

#include <string>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Zen {
namespace Studio {
namespace Workbench {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
class I_DocumentType;

/// @brief Project Type
/// This class represents the details of a project type
/// that can be manipulated by the Workbench.  Until
/// the extension is actually used, the plugin will not
/// be loaded.  This data comes from the plugin.xml
///
///    <project
///          class="Zen::Studio::WorldProjectService"
///          type="world"
///          name="World Project">
///      <document-type
///          name="World Zone"
///          file-extension="zone" />
///    </project>
class I_ProjectType
:   boost::noncopyable
{
    /// @name Types
    /// @{
public:
    /// Declared below
    class I_DocumentTypeVisitor;
    /// @}

    /// @name I_ProjectType interface
    /// @{
public:
    /// Get the name of this project type
    virtual const std::string& getName() const = 0;

    /// Get the name of the class of the I_ProjectService that implements
    /// this project type.
    virtual const std::string& getClassName() const = 0;

    /// Get the extension point type to pass to the I_ProjectServiceManager to 
    /// construct the appropriate I_ProjectService
    virtual const std::string& getType() const = 0;

    /// Get all of the document types this project can create
    virtual void getDocumentTypes(I_DocumentTypeVisitor& _visitor) const = 0;
    /// @}

    /// @name 'Structors
    /// @{
protected:
             I_ProjectType();
    virtual ~I_ProjectType();    
    /// @}

    /// @name Inner Classes
    /// @{
public:
    class I_DocumentTypeVisitor
    {
    public:
        virtual void begin() = 0;
        virtual void visit(I_DocumentType& _documentType) = 0;
        virtual void end() = 0;
    };  // class I_DocumentTypeVisitor

    /// @}

};  // interface I_ProjectType

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace Workbench
}   // namespace Studio
}   // namespace Zen
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

#endif // ZENSTUDIO_WORKBENCH_I_PROJECT_TYPE_HPP_INCLUDED
