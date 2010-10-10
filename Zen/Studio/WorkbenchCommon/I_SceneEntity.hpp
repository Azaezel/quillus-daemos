//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
// Zen Studio
//
// Copyright (C) 2001 - 2010 Tony Richards
// Copyright (C) 2008 - 2010 Matthew Alan Gray
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
#ifndef ZENSTUDIO_WORKBENCH_I_SCENE_ENTITY_HPP_INCLUDED
#define ZENSTUDIO_WORKBENCH_I_SCENE_ENTITY_HPP_INCLUDED

#include "Configuration.hpp"

#include <Zen/Core/Memory/managed_ptr.hpp>

#include <Zen/Enterprise/Database/I_Persistable.hpp>

#include <boost/noncopyable.hpp>

#include <string>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Zen {
namespace Studio {
namespace Workbench {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
class I_SceneController;

class WORKBENCHCOMMON_DLL_LINK I_SceneEntity
:   public boost::noncopyable
{
    /// @name Types
    /// @{
public:
    class WORKBENCHCOMMON_DLL_LINK UserData
    :   public Database::I_Persistable
    {
        /// The framework will call setEntity() with user data that is
        /// associated with an I_SceneEntity.
        virtual void setEntity(I_SceneEntity* _pEntity) { m_pEntity = _pEntity; }

        virtual I_SceneEntity* getEntity() const { return m_pEntity; }

        /// Return true if this user data can be modified.
        virtual bool getMutable() const = 0;

        /// Set the name of this entity.
        virtual void setName(const std::string& _name) { m_name = _name; }

        /// Get the name of this entity.
        virtual const std::string& getName() const { return m_name; }

    public:
                 UserData() {}
        virtual ~UserData() {}

    private:
        I_SceneEntity*              m_pEntity;
        std::string                 m_name;
    };  // class UserData

    typedef Memory::managed_ptr<UserData>           pUserData_type;
    /// @}

    /// @name I_SceneEntity interface
    /// @{
public:
    virtual pUserData_type getUserData() = 0;
    /// @}

    /// @name 'Structors
    /// @{
protected:
             I_SceneEntity();
    virtual ~I_SceneEntity();
    /// @}

};  // interface I_SceneEntity

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace Workbench
}   // namespace Studio
}   // namespace Zen
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

#endif // ZENSTUDIO_WORKBENCH_I_SCENE_ENTITY_HPP_INCLUDED
