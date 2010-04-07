//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
// Zen Game Engine Framework
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
#ifndef ZEN_ZODE_HEIGHTFIELD_RESOURCE_HPP
#define ZEN_ZODE_HEIGHTFIELD_RESOURCE_HPP

#include <Zen/Core/Memory/managed_self_ref.hpp>
#include <Zen/Engine/Resource/I_CollisionResource.hpp>

#include <ode/ode.h>

#include <vector>
#include <map>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Zen {
namespace ZODE {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
class PhysicsActor;

class HeightfieldResource
:   virtual public Zen::Engine::Resource::I_CollisionResource
,   public Zen::Memory::managed_self_ref<Zen::Engine::Resource::I_Resource>
{
    /// @name Types
    /// @{
public:
    typedef std::map<std::string, std::string>  config_type;
    /// @}

    /// @name I_Resource implementation
    /// @{
public:
    /// @}

    /// @name Getter / Setter
    /// @{
public:
    /// @}

    /// @name HeigthfieldResource implementation
    /// @{
public:
    void initialize(PhysicsActor* _pActor);
    /// @}

    /// @name 'Structors
    /// @{
public:
             HeightfieldResource(config_type& _config);
    virtual ~HeightfieldResource();
    /// @}

    /// @name Member Variables
    /// @{
private:
    std::string                     m_fileName;
    Math::Real                      m_width;
    Math::Real                      m_depth;
    int                             m_widthSamples;
    int                             m_depthSamples;
    Math::Real                      m_scale;
    Math::Real                      m_thickness;
    bool                            m_wrap;

    std::vector<boost::int16_t>     m_buffer;


    dHeightfieldDataID              m_heightfieldDataId;
    dGeomID                         m_heightfieldGeometryId;
    /// @}

};  // class HeightfieldResource

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace ZODE
}   // namespace Zen
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

#endif // ZEN_ZODE_HEIGHTFIELD_RESOURCE_HPP
