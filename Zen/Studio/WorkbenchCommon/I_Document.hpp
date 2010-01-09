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
#ifndef ZENSTUDIO_WORKBENCH_I_DOCUMENT_HPP_INCLUDED
#define ZENSTUDIO_WORKBENCH_I_DOCUMENT_HPP_INCLUDED

#include "Configuration.hpp"

#include <Zen/Core/Event/Event.hpp>

#include <Zen/Studio/WorkbenchCommon/I_Viewable.hpp>

#include <string>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Zen {
namespace Studio {
namespace Workbench {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
class I_Project;

class WORKBENCHCOMMON_DLL_LINK I_Document
:   public virtual I_Viewable
{
    /// @name Types
    /// @{
public:
    typedef Memory::managed_ptr<I_Document>                 pDocument_type;
    /// @}

    /// @name I_Document interface
    /// @{
public:
    /// Get the short name of the document.
    /// The short name is generally displayed on tabs and used for other short
    /// labelabels.
    virtual const std::string& getShortName() const = 0;

    /// Get the long name of the document.
    /// The long name is generally a unique fully qualfied identifier for the
    /// document.
    virtual const std::string& getLongName() const = 0;

    /// Get the parent project that contains this document.
    /// Note all documents are associated with projects.  NULL is returned
    /// if this document is not contained by a project.
    /// @return A raw pointer to the containing project, or NULL if the 
    ///     document is not contained by a project.
    virtual I_Project* getProject() const = 0;
    /// @}

    /// @name Events
    /// @{
public:
    /// @}

    /// @name 'Structors
    /// @{
protected:
             I_Document();
    virtual ~I_Document();
    /// @}

};  // interface I_Document

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace Workbench
}   // namespace Studio
namespace Memory 
{
    /// I_Document is managed by a factory
    template<>
    struct is_managed_by_factory<Zen::Studio::Workbench::I_Document> : public boost::true_type{};
}   // namespace Memory
}   // namespace Zen
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

#endif // ZENSTUDIO_WORKBENCH_I_DOCUMENT_HPP_INCLUDED
