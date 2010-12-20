//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
// Zen Game Engine Framework
//
// Copyright (C) 2001 - 2010 Tony Richards
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

#include "Mesh.hpp"

#include <Zen/Engine/Resource/I_ResourceManager.hpp>

#include <OgreSubMesh.h>

#include <boost/cstdint.hpp>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Zen {
namespace ZOgre {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
Mesh::Mesh(Ogre::MeshPtr _pMesh)
:   m_pMesh(_pMesh)
{
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
Mesh::~Mesh()
{
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
static void
getVertexData(Ogre::VertexData* _pVertexData, std::vector<Zen::Math::Point3>& _vertices)
{
    const Ogre::VertexElement* pVertexPosition = _pVertexData->vertexDeclaration->findElementBySemantic(Ogre::VES_POSITION);
    Ogre::HardwareVertexBufferSharedPtr pVertexBuffer = _pVertexData->vertexBufferBinding->getBuffer(pVertexPosition->getSource());
    const size_t vertexSize = pVertexBuffer->getVertexSize();
    const size_t vertexCount = _pVertexData->vertexCount;

    float* pFloat;
    unsigned char* pVertex = static_cast<unsigned char*>(pVertexBuffer->lock(Ogre::HardwareBuffer::HBL_READ_ONLY));
    for(size_t i = 0; i < vertexCount; i++)
    {
        // Adjust pFloat to point to the beginning of this vertex
        pVertexPosition->baseVertexPointerToElement(pVertex, &pFloat);

        // Copy the vertex to a Point3
        Zen::Math::Point3 vertex;
        memcpy(vertex.m_array, pFloat, sizeof(vertex.m_array));
        _vertices.push_back(vertex);

        // Skip to the next vertex in the buffer
        pVertex += vertexSize;
    }


    pVertexBuffer->unlock();
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
static void
getIndexData(Ogre::IndexData* _pIndexData, std::vector<boost::array<int, 3> >& _indices)
{
    Ogre::HardwareIndexBufferSharedPtr pIndexBuffer = _pIndexData->indexBuffer;

    const bool use32BitIndices = (pIndexBuffer->getType() == Ogre::HardwareIndexBuffer::IT_32BIT);
    const size_t triangleCount = _pIndexData->indexCount / 3;
    if (use32BitIndices)
    {
        boost::uint32_t* pInt = static_cast<boost::uint32_t*>(pIndexBuffer->lock(Ogre::HardwareBuffer::HBL_READ_ONLY));

        for(size_t i = 0; i < triangleCount; i++)
        {
            boost::array<int, 3> converted = { (int)pInt[0], (int)pInt[1], (int)pInt[2] };
            pInt += 3;
            _indices.push_back(converted);
        }

        pIndexBuffer->unlock();
    }
    else
    {
        // Use 16 bit indices
        boost::uint16_t* pInt = static_cast<boost::uint16_t*>(pIndexBuffer->lock(Ogre::HardwareBuffer::HBL_READ_ONLY));

        for(size_t i = 0; i < triangleCount; i++)
        {
            boost::array<int, 3> converted = { (int)pInt[0], (int)pInt[1], (int)pInt[2] };
            pInt += 3;
            _indices.push_back(converted);
        }

        pIndexBuffer->unlock();
    }
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
Mesh::getMesh(std::vector<Zen::Math::Point3>& _vertices, std::vector<boost::array<int, 3> >& _indices)
{
    if (m_pMesh->sharedVertexData != NULL)
    {
        getVertexData(m_pMesh->sharedVertexData, _vertices);
    }

    Ogre::Mesh::SubMeshIterator iter = m_pMesh->getSubMeshIterator();
    while(iter.hasMoreElements())
    {
        Ogre::SubMesh* pSubMesh = iter.getNext();

        if (pSubMesh->useSharedVertices)
        {
            getIndexData(pSubMesh->indexData, _indices);
            getVertexData(pSubMesh->vertexData, _vertices);
        }
        else
        {
            getIndexData(pSubMesh->indexData, _indices);
        }
    }
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
Scripting::I_ObjectReference*
Mesh::getScriptObject()
{
    // TODO Make thread safe?
    if (m_pScriptObject == NULL)
    {
        m_pScriptObject = new ScriptObjectReference_type(
            Engine::Resource::I_ResourceManager::getSingleton().getDefaultResourceScriptModule(),
            Engine::Resource::I_ResourceManager::getSingleton().getDefaultResourceScriptModule()->getScriptType(getScriptTypeName()),
            getSelfReference().lock()
        );
    }
    return m_pScriptObject;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace ZOgre
}   // namespace Zen
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
