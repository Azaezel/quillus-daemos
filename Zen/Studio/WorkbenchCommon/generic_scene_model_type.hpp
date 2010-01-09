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
#ifndef ZENSTUDIO_WORKBENCHCOMMON_GENERIC_SCENE_MODEL_TYPE_HPP_INCLUDED
#define ZENSTUDIO_WORKBENCHCOMMON_GENERIC_SCENE_MODEL_TYPE_HPP_INCLUDED

#include <Zen/Studio/WorkbenchCommon/I_SceneModelType.hpp>

#include <boost/bind.hpp>

#include <string>
#include <set>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Zen {
namespace Studio {
namespace Workbench {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
;

/// Generic I_SceneModelType template.
/// Use this template as a helper for creating scene model types.
/// TR: Why a template?  Dunno....
template<class BaseType_type = void>
class generic_scene_model_type
:   public Zen::Studio::Workbench::I_SceneModelType
{
    /// @name Types
    /// @{
public:
    typedef std::set<I_SceneContributor*>               SceneContributors_type;
    /// @}

    /// @name I_SceneModelType partial implementation
    /// @{
public:
    virtual void getContributors(I_SceneModel& _document, I_SceneContributorVisitor& _visitor);
    virtual void addContributor(I_SceneContributor& _contributor);
    virtual void removeContributor(I_SceneContributor& _contributor);
    /// @}

    /// @name 'Structors
    /// @{
public:
             generic_scene_model_type();
    virtual ~generic_scene_model_type();
    /// @}

    /// @name Member Variables
    /// @{
protected:
    SceneContributors_type    m_contributors;
    /// @}

};  // template generic_explorer_node_type

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
template<class BaseType_type>
generic_scene_model_type<BaseType_type>::generic_scene_model_type()
{
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
template<class BaseType_type>
generic_scene_model_type<BaseType_type>::~generic_scene_model_type()
{
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
template<class BaseType_type>
void
generic_scene_model_type<BaseType_type>::getContributors(I_SceneModel& _document, I_SceneContributorVisitor& _visitor)
{
    _visitor.begin();

    for(SceneContributors_type::iterator iter = m_contributors.begin(); iter != m_contributors.end(); iter++)
    {
        _visitor.visit(_document, **iter);
    }

    _visitor.end();
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
template<class BaseType_type>
void
generic_scene_model_type<BaseType_type>::addContributor(I_SceneContributor& _contributor)
{
    m_contributors.insert(&_contributor);
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
template<class BaseType_type>
void
generic_scene_model_type<BaseType_type>::removeContributor(I_SceneContributor& _contributor)
{
    m_contributors.erase(&_contributor);
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace Workbench
}   // namespace Studio
}   // namespace Zen
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

#endif // ZENSTUDIO_WORKBENCHCOMMON_GENERIC_SCENE_MODEL_TYPE_HPP_INCLUDED
