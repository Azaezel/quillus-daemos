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
#ifndef ZMICROPATHER_NODEITERATOR_HPP_INCLUDED
#define ZMICROPATHER_NODEITERATOR_HPP_INCLUDED

#include <Zen/Engine/Navigation/I_NodeIterator.hpp>
#include <Zen/Core/Memory/managed_self_ref.hpp>

#include <vector>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace ZMicroPather {

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
class NodeIterator
:   public Zen::Engine::Navigation::I_NodeIterator
{
    /// @name Types
    /// @{
public:
    typedef std::vector<void*>              Collection_type;
    /// @}

    /// @name NodeIterator func wrappers
    /// @{
public:
    /// Returns true if there there is a next node.
    virtual bool hasNext() const;
    /// Get the next node.
    virtual pNode_type next();
    /// @}

    /// @name 'Structors
protected:
    friend class Path;
             NodeIterator(std::vector<void*>& _path);
    virtual ~NodeIterator();
    /// @}

    /// @name Member Variables
    /// @{
private:
    Collection_type                     m_path;
    Collection_type::iterator           m_cursor;
    /// @}

};  // class Path

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace MicroPather
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

#endif // ZMICROPATHER_NODEITERATOR_HPP_INCLUDED
