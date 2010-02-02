//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
// Zen Game Engine Framework
//
// Copyright (C) 2001 - 2009 Tony Richards
// Copyright (C)        2009 Jason Smith
// Copyright (C) 2008 - 2009 Matthew Alan Gray
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
//  Tony Richards trichards@indiezen.org
//  Jason Smith jsmith@airsteampunk.com
//  Matthew Alan Gray mgray@indiezen.org
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
#ifndef ZMICROPATHER_PATH_HPP_INCLUDED
#define ZMICROPATHER_PATH_HPP_INCLUDED

#include <Zen/Engine/Navigation/I_Path.hpp>

#include <vector>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace ZMicroPather {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
class Path
:   public Zen::Engine::Navigation::I_Path
{
    /// @name Types
    /// @{
public:
    typedef Zen::Memory::managed_weak_ptr<Zen::Engine::Navigation::I_NodeIterator>  wpNodeIterator_type;
    /// @}

    /// @name I_Path implementation
    /// @{
public:
    virtual Zen::Engine::Navigation::I_Path::pNodeIterator_type front();
    virtual Zen::Math::Real getCost();
    /// @}

    /// @name Path implementation
    /// @{
public:
    /// Get a pointer to the underlying vector so MicroPather can use it.
    std::vector<void*>* getVectorReference();

    /// Get a pointer to the underlying cost so MicroPather can use it.
    /// @note the return type is intentionally a float and not Zen::Math::Real.
    float* getTotalCostReference();

    static void destroy(wpNodeIterator_type _pType);
    /// @}

    /// @name 'Structors
    /// @{
protected:
    friend class Graph;
             Path();
    virtual ~Path();
    /// @}

    /// @name Member Variables
    /// @{
private:
    std::vector<void*>  m_path;
    float               m_cost;
    /// @}

};  // class Path

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace MicroPather
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

#endif // ZMICROPATHER_PATH_HPP_INCLUDED
