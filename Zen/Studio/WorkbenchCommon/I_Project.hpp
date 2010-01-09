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
#ifndef ZENSTUDIO_WORKBENCH_I_PROJECT_HPP_INCLUDED
#define ZENSTUDIO_WORKBENCH_I_PROJECT_HPP_INCLUDED

#include "Configuration.hpp"

#include <Zen/Core/Event/Event.hpp>

#include <Zen/Core/Memory/managed_ptr.hpp>
#include <Zen/Core/Memory/managed_weak_ptr.hpp>

#include <Zen/Studio/WorkbenchCommon/I_ExplorerNode.hpp>
#include <Zen/Studio/WorkbenchCommon/I_ProjectExplorerController.hpp>

#include <boost/noncopyable.hpp>
#include <boost/filesystem.hpp>

#include <string>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Zen {
namespace Studio {
namespace Workbench {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
class I_DocumentInfo;
class I_DocumentType;
class I_Document;
class I_Workbench;

/// Project interface.
/// @note TR - This probably should not be a UserData, but possibly it should have
/// a UserData.  Not sure yet.
class WORKBENCHCOMMON_DLL_LINK I_Project
:   public I_ExplorerNode::UserData
{
    /// @name Types
    /// @{
public:
    typedef Event::Event<I_Project*>                ProjectEvent_type;
    typedef Memory::managed_ptr<I_Document>         pDocument_type;
    typedef Memory::managed_weak_ptr<I_Workbench>   wpWorkbench_type;
    typedef I_ExplorerNode*                         pExplorerNode_type;

    // Defined below as an inner class
    struct I_DocumentTypeVisitor;
    struct I_DocumentInfoVisitor;
    struct I_ProjectActionVisitor;
    /// @}

    /// @name I_Project interface
    /// @{
public:
    /// Called by the framework when this project has been successfully created.
    virtual void onCreated() = 0;

    /// Get the unique project identifier.
    virtual boost::uint64_t getProjectId() const = 0;

    /// Set the unique project identifier.
    /// This is generally set by the persistence layer.
    virtual void setProjectId(boost::uint64_t _projectId) = 0;

    virtual Zen::Studio::Workbench::I_ProjectExplorerController& getController() = 0;

    /// Get the parent workbench service.
    virtual Zen::Studio::Workbench::I_WorkbenchService& getWorkbenchService() = 0;

    /// Get the database connection associated with this project.
    virtual pDatabaseConnection_type getDatabaseConnection() = 0;

    /// Get the control path for this project.
    virtual const boost::filesystem::path& getControlPath() = 0;
    /// @}

    /// @name Events
    /// @{
public:
    ProjectEvent_type   onClose;
    /// @}

    /// @name Inner Classes
    /// @{
public:
    struct I_DocumentTypeVisitor
    {
        virtual void begin() = 0;
        virtual void visit(const I_DocumentType& _documentType) = 0;
        virtual void end() = 0;
    };  // struct I_DocumentTypeVisitor

    struct I_DocumentInfoVisitor
    {
        virtual void begin() = 0;
        virtual void visit(const I_DocumentInfo& _documentType) = 0;
        virtual void end() = 0;
    };  // struct I_DocumentInfoVisitor

    /// @name 'Structors
    /// @{
protected:
             I_Project(const std::string& _name);
    virtual ~I_Project();
    /// @}

};  // interface I_Project

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace Workbench
}   // namespace Studio
namespace Memory {
	/// I_Project is managed by factory
	template<>
	struct is_managed_by_factory<Studio::Workbench::I_Project> : public boost::true_type{};
}	// namespace Memory
}   // namespace Zen
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

#endif // ZENSTUDIO_WORKBENCH_I_PROJECT_HPP_INCLUDED
