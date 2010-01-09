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
#ifndef ZENSTUDIO_WORKBENCHCOMMON_I_SCENE_CONTRIBUTOR_HPP_INCLUDED
#define ZENSTUDIO_WORKBENCHCOMMON_I_SCENE_CONTRIBUTOR_HPP_INCLUDED

#include "Configuration.hpp"

#include <Zen/Studio/Workbench/I_Contributor.hpp>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Zen {
    namespace Engine {
        namespace Rendering {
            class I_SceneNode;
        }   // namespace Rendering
    }   // namespace Engine
namespace Studio {
namespace Workbench {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
class I_SceneModel;
class I_SceneAction;

/// Scene Contributor.
/// This Contributor contributes actions to a Scene / Scene Node.
/// @see http://www.indiezen.org/wiki/irclogs/2009/05/22/#UTC2009-05-22T18:31:55
class WORKBENCH_DLL_LINK I_SceneContributor
:   public virtual I_Contributor
{
    /// @name Types
    /// @{
public:
    // Defined below.
    struct I_SceneActionVisitor;

    typedef I_SceneActionVisitor                                Visitor_type;
    /// @}

    /// @name I_SceneContributor interface
    /// @{
public:
    /// Get the scene actions associated with this model.
    /// @param _pSceneNode possibly NULL if the action is requested of the model and not of a specific scene node.
    virtual void getActions(I_SceneModel& _model, Engine::Rendering::I_SceneNode* _pSceneNode, I_SceneActionVisitor& _visitor) = 0;
    /// @}

    /// @name 'Structors
    /// @{
protected:
             I_SceneContributor();
    virtual ~I_SceneContributor();
    /// @}

    /// @name Inner Classes
    /// @{
public:
    struct I_SceneActionVisitor
    {
        virtual void begin() = 0;
        virtual void visit(I_SceneAction& _action) = 0;
        virtual void end() = 0;
    };  // struct I_SceneActionVisitor;
    /// @}

};  // interface I_SceneContributor

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace Workbench
}   // namespace Studio
}   // namespace Zen
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

#endif // ZENSTUDIO_WORKBENCHCOMMON_I_SCENE_CONTRIBUTOR_HPP_INCLUDED
