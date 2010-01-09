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
/// @author Tony Richards trichards@indiezen.com
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
#ifndef ZENSTUDIO_WORKBENCHCOMMON_I_SCENE_MODEL_TYPE_HPP_INCLUDED
#define ZENSTUDIO_WORKBENCHCOMMON_I_SCENE_MODEL_TYPE_HPP_INCLUDED

#include "Configuration.hpp"

#include <Zen/Core/Memory/managed_ptr.hpp>

#include <boost/noncopyable.hpp>

#include <string>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Zen {
namespace Studio {
namespace Workbench {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
class I_SceneModel;
class I_SceneContributor;

/// Scene Model Type.
/// Implement this to create a custom scene model type.
class WORKBENCHCOMMON_DLL_LINK I_SceneModelType
:   public boost::noncopyable
{
    /// @name Types
    /// @{
public:
    struct I_SceneContributorVisitor;
    /// @}

    /// @name I_SceneModelType interface
    /// @{
public:
    /// Get the contributors associated with this scene model.
    /// @todo Should this be const?
    virtual void getContributors(I_SceneModel& _model, I_SceneContributorVisitor& _visitor) = 0;

    /// Add a contributor to this scene model type.
    /// Be sure to call removeContributor() before _contributor goes
    /// out of scope because the implementation of this method may retain
    /// a reference to _contributor.
    virtual void addContributor(I_SceneContributor& _contributor) = 0;

    /// Remove a contributor from this scene model type.
    virtual void removeContributor(I_SceneContributor& _contributor) = 0;
    /// @}

    /// @name Inner Types
    /// @{
public:
    struct I_SceneContributorVisitor
    {
        virtual void begin() = 0;
        virtual void visit(I_SceneModel& _model, I_SceneContributor& _contributor) = 0;
        virtual void end() = 0;
    };  // struct I_SceneContributorVisitor
    /// }@

    /// @name 'Structors
    /// @{
protected:
             I_SceneModelType();
    virtual ~I_SceneModelType();
    /// @}

};  // interface I_SceneModelType

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace Workbench
}   // namespace Studio
namespace Memory {
	/// I_SceneModelType is managed by factory
	template<>
	struct is_managed_by_factory<Studio::Workbench::I_SceneModelType> : public boost::true_type{};
}	// namespace Memory
}   // namespace Zen
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

#endif // ZENSTUDIO_WORKBENCHCOMMON_I_SCENE_MODEL_TYPE_HPP_INCLUDED
