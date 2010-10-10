//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
// Zen Spaces
//
// Copyright (C) 2001 - 2009 Tony Richards
//
// Licensed under the Games by Sarge Publishing License - See your licensing
// agreement for terms and conditions.
//
// Do not redistribute this source code.
//
// Tony Richards trichards@gamesbysarge.com
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
// This is generated by the Zen Spaces Code Generator.  Do not modify!
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
#ifndef TERRAINBUILDER_I_TERRAINCHUNK_DOMAIN_OBJECT_HPP_INCLUDED
#define TERRAINBUILDER_I_TERRAINCHUNK_DOMAIN_OBJECT_HPP_INCLUDED

#include "Configuration.hpp"

#include <Zen/Core/Memory/managed_ptr.hpp>

#include <Zen/Enterprise/DataModel/I_DataElement.hpp>

#include <boost/noncopyable.hpp>
#include <boost/any.hpp>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace TerrainBuilder {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~


class TERRAINBUILDERMODEL_DLL_LINK I_TerrainChunkDomainObject
:	public boost::noncopyable
{
    /// @name Types
    /// @{
public:
    typedef Zen::Memory::managed_ptr<Zen::Enterprise::DataModel::I_DataElement> pElement_type;
    typedef Zen::Enterprise::DataModel::I_DataElement&                          ElementReference_type;
    /// @}

    /// @name Getter / Setter methods
    /// @{
public:
    /// Get the TerrainChunkId element.
    /// Use this return value to check to see if the column is NULL in the database,
    /// or you can use it to modify the value instead of calling setTerrainChunkId().
    virtual ElementReference_type getTerrainChunkId() const = 0;
    
    /// Set the TerrainChunkId element value.
    /// This method does not directly update the database.  
    /// You must use the I_TerrainChunkDataMap::update() method for the modifications
    /// to be persisted.
    virtual void setTerrainChunkId(const boost::any& _value) = 0;

    /// Get the ExplorerNodeId element.
    /// Use this return value to check to see if the column is NULL in the database,
    /// or you can use it to modify the value instead of calling setExplorerNodeId().
    virtual ElementReference_type getExplorerNodeId() const = 0;
    
    /// Set the ExplorerNodeId element value.
    /// This method does not directly update the database.  
    /// You must use the I_TerrainChunkDataMap::update() method for the modifications
    /// to be persisted.
    virtual void setExplorerNodeId(const boost::any& _value) = 0;

    /// Get the Name element.
    /// Use this return value to check to see if the column is NULL in the database,
    /// or you can use it to modify the value instead of calling setName().
    virtual ElementReference_type getName() const = 0;
    
    /// Set the Name element value.
    /// This method does not directly update the database.  
    /// You must use the I_TerrainChunkDataMap::update() method for the modifications
    /// to be persisted.
    virtual void setName(const boost::any& _value) = 0;

    /// Get the PropertiesId element.
    /// Use this return value to check to see if the column is NULL in the database,
    /// or you can use it to modify the value instead of calling setPropertiesId().
    virtual ElementReference_type getPropertiesId() const = 0;
    
    /// Set the PropertiesId element value.
    /// This method does not directly update the database.  
    /// You must use the I_TerrainChunkDataMap::update() method for the modifications
    /// to be persisted.
    virtual void setPropertiesId(const boost::any& _value) = 0;
    
    virtual Zen::Enterprise::DataModel::I_DataElement& getElement(const std::string& _fieldName) = 0;
    /// @}

    /// @name 'Structors
    /// @{
protected:
             I_TerrainChunkDomainObject();
    virtual ~I_TerrainChunkDomainObject();
    /// @}

};  // interface I_TerrainChunkDomainObject

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace TerrainBuilder
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

namespace Zen {
namespace Memory {
    /// I_TerrainChunkDomainObject is a managed by factory
    template<>
    struct is_managed_by_factory<TerrainBuilder::I_TerrainChunkDomainObject> : public boost::true_type{};
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace Memory
}   // namespace Zen
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

#endif // TERRAINBUILDER_I_TERRAINCHUNK_DOMAIN_OBJECT_HPP_INCLUDED
