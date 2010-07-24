//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
// Zen Game Engine Framework
//
// Copyright (C) 2001 - 2009 Tony Richards
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
//  Tony Richards trichards@indiezen.com
//  Matthew Alan Gray mgray@indiezen.org
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
#ifndef ZEN_ENGINE_RENDERING_I_BOUNDING_VOLUME_TREE_HPP_INCLUDED
#define ZEN_ENGINE_RENDERING_I_BOUNDING_VOLUME_TREE_HPP_INCLUDED

#include "Configuration.hpp"

#include <Zen/Core/Memory/managed_ptr.hpp>
#include <Zen/Core/Memory/managed_weak_ptr.hpp>
#include <Zen/Core/Event/Event.hpp>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Zen {
namespace Engine {
namespace Rendering {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

class I_BoundingVolume;

class RENDERING_DLL_LINK I_BoundingVolumeTree
{
    /// @name Forward declarations
    /// @{
public:
    class I_Node;
    /// @}

    /// @name Types
    /// @{
public:
    typedef Zen::Memory::managed_ptr<I_BoundingVolumeTree>      pBoundingVolumeTree_type;
    typedef Zen::Memory::managed_weak_ptr<I_BoundingVolumeTree> wpBoundingVolumeTree_type;
    typedef Zen::Event::Event<wpBoundingVolumeTree_type>        boundingVolumeTreeEvent_type;

    typedef Zen::Memory::managed_ptr<I_Node>                    pNode_type;
    typedef Zen::Memory::managed_ptr<I_BoundingVolume>          pBoundingVolume_type;
    /// @}

    /// @name Zen::Engine::Rendering::I_BoundingVolume interface
    /// @{
public:
    virtual const pNode_type getRoot() const = 0;
    virtual void addBoundingVolume(const pBoundingVolume_type& _pBoundingVolume) = 0;
    virtual void removeBoundingVolume(const pBoundingVolume_type& _pBoundingVolume) = 0;
    /// @}

    /// @name Events
    /// @{
public:
    boundingVolumeTreeEvent_type    onDestroyEvent;
    /// @}

    /// @name Handlers
    /// @{
public:
    class RENDERING_DLL_LINK I_Node
    {
        /// @name Types
        /// @{
    public:
        typedef Zen::Memory::managed_ptr<I_Node>            pNode_type;
        typedef Zen::Memory::managed_weak_ptr<I_Node>       wpNode_type;
        typedef Zen::Event::Event<wpNode_type>              nodeEvent_type;

        typedef Zen::Memory::managed_ptr<I_BoundingVolume>  pBoundingVolume_type;
        /// @}

        /// @name Zen::Engine::Rendering::I_BoundingVolume::I_Node interface
        /// @{
    public:
        virtual unsigned int getNumChildren() const = 0;
        virtual const pBoundingVolume_type getBoundingVolume(unsigned int _index) const = 0;
        virtual const pBoundingVolume_type getNodeBoundingVolume() const = 0;
        virtual const pNode_type getLeftChild() const = 0;
        virtual const pNode_type getRightChild() const = 0;
        virtual bool isLeaf() const = 0;
        /// @}

        /// @name Events
        /// @{
    public:
        nodeEvent_type  onDestroyEvent;
        /// @}

        /// @name 'Structors
        /// @{
    protected:
                 I_Node();
        virtual ~I_Node();
        /// @}

    };  // interface Node

    class RENDERING_DLL_LINK I_BoundingVolumeSplitter
    {
        /// @name Types
        /// @{
    public:
        typedef Zen::Memory::managed_ptr<I_BoundingVolumeSplitter>      pBoundingVolumeSplitter_type;
        typedef Zen::Memory::managed_weak_ptr<I_BoundingVolumeSplitter> wpBoundingVolumeSplitter_type;
        typedef Zen::Event::Event<wpBoundingVolumeSplitter_type>        boundingVolumeSplitterEvent_type;

        typedef Zen::Memory::managed_ptr<I_Node>                        pNode_type;
        typedef Zen::Memory::managed_ptr<I_BoundingVolume>              pBoundingVolume_type;
        /// @}

        /// @name Zen::Engine::Rendering::I_BoundingVolumeTree::I_BoundingVolumeSplitter interface
        /// @{
    public:
        virtual bool splitLeft(const pNode_type& _pNode) const = 0;
        virtual bool lessThan(const pNode_type& _lhs, const pNode_type& _rhs) const = 0;
        virtual const pBoundingVolume_type getNodeBoundingVolume() const = 0;
        virtual bool operator()(const pNode_type& _pNode) const = 0;
        virtual bool operator()(const pNode_type& _lhs, const pNode_type& _rhs) const = 0;
        /// @}

        /// @name Events
        /// @{
    public:
        boundingVolumeSplitterEvent_type    onDestroyEvent;
        /// @}

        /// @name 'Structors
        /// @{
                 I_BoundingVolumeSplitter();
        virtual ~I_BoundingVolumeSplitter();
        /// @}

    };  // interface I_BoundingVolumeSplitter
    /// @}

    /// @name 'Structors
    /// @{
protected:
             I_BoundingVolumeTree();
    virtual ~I_BoundingVolumeTree();
    /// @}

};  // interface I_BoundingVolumeTree

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace Rendering
}   // namespace Engine
namespace Memory {
    /// I_BoundingVolumeTree is managed by a factory method
    template<>
    struct is_managed_by_factory<Zen::Engine::Rendering::I_BoundingVolumeTree> : public boost::true_type{};
    /// I_BoundingVolumeTree::I_Node is managed by a factory method
    template<>
    struct is_managed_by_factory<Zen::Engine::Rendering::I_BoundingVolumeTree::I_Node> : public boost::true_type{};
    /// I_BoundingVolumeTree::I_BoundingVolumeSplitter is managed by a factory method
    template<>
    struct is_managed_by_factory<Zen::Engine::Rendering::I_BoundingVolumeTree::I_BoundingVolumeSplitter> : public boost::true_type{};
}   // namespace Memory
}   // namespace Zen
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

#endif // ZEN_ENGINE_RENDERING_I_BOUNDING_VOLUME_TREE_HPP_INCLUDED
