//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
// Terrain Builder
//
// Copyright (C) 2001 - 2010 Tony Richards
// Copyright (C) 2008 - 2010 Matthew Alan Gray
//
// Licensed under the Games by Sarge Publishing License - See your licensing
// agreement for terms and conditions.
//
// Do not redistribute this source code.
//
// Tony Richards trichards@gamesbysarge.com
// Matthew Alan Gray mgray@indiezen.org
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
#include "TerrainChunkNode.hpp"
#include "Project.hpp"
#include "TerrainChunkDocument.hpp"
#include "Properties.hpp"
#include "Property.hpp"

#include <Zen/Core/Utility/runtime_exception.hpp>

#include <Zen/Core/Memory/managed_weak_ptr.hpp>

#include <Zen/Core/Plugins/I_ExtensionRegistry.hpp>
#include <Zen/Core/Plugins/I_ExtensionPoint.hpp>
#include <Zen/Core/Plugins/I_Extension.hpp>
#include <Zen/Core/Plugins/I_ConfigurationElement.hpp>

#include <Zen/Studio/WorkbenchCommon/I_ExplorerNodeAction.hpp>
#include <Zen/Studio/WorkbenchCommon/I_ProjectExplorerController.hpp>
#include <Zen/Studio/WorkbenchCommon/I_ProjectExplorerModel.hpp>
#include <Zen/Studio/WorkbenchCommon/I_WorkbenchService.hpp>
#include <Zen/Studio/WorkbenchCommon/I_ExplorerNodeType.hpp>
#include <Zen/Studio/WorkbenchCommon/I_ExplorerNode.hpp>
#include <Zen/Studio/WorkbenchCommon/generic_explorer_node_type.hpp>

#include <Zen/StudioPlugins/TerrainBuilderModel/I_TerrainChunkDomainObject.hpp>
#include <Zen/StudioPlugins/TerrainBuilderModel/I_TerrainChunkDataCollection.hpp>

#include <Zen/Studio/WorkbenchCommon/I_View.hpp>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace TerrainBuilder {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
TerrainChunkNode::DocumentEvent_type    TerrainChunkNode::onOpened;
TerrainChunkNode::DocumentEvent_type    TerrainChunkNode::onClosed;
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
TerrainChunkNode::TerrainChunkNode(Project& _project, const std::string& _name)
:   I_TerrainChunkNode(_name)
,   m_project(_project)
,   m_properties(*this)
{
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
TerrainChunkNode::~TerrainChunkNode()
{
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
TerrainChunkNode::setName(const std::string& _name)
{
    // Only do this if the name changes
    if (getName() != _name)
    {
        m_pTerrainChunkDO->getName() = _name;

        // Call the base class set name, which will fire the "modified" event.
        UserData::setName(_name);
    }
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
Zen::Studio::Workbench::I_ExplorerNodeType&
TerrainChunkNode::getType() const
{
    return I_TerrainChunkNode::getNodeType();
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
TerrainChunkNode::insert(pDatabaseConnection_type _pDBConn)
{
    m_properties.insert(_pDBConn);

    pTerrainChunkDataMap_type pTerrainChunkDM = 
        I_TerrainChunkDataMap::create(_pDBConn);

    m_pTerrainChunkDO = pTerrainChunkDM->createNew();

    m_pTerrainChunkDO->getExplorerNodeId() = getNode()->getNodeId();
    m_pTerrainChunkDO->getName() = getNode()->getDisplayName();
    m_pTerrainChunkDO->getPropertiesId() = m_properties.getPropertiesId();

    pTerrainChunkDM->update(m_pTerrainChunkDO);
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
TerrainChunkNode::load(pDatabaseConnection_type _pDBConn)
{
    pTerrainChunkDataMap_type pTerrainChunkDM = 
        I_TerrainChunkDataMap::create(_pDBConn);

    struct TerrainChunkVisitor
    :   public I_TerrainChunkDataCollection::I_CollectionVisitor
    {
        virtual void begin()
        {
        }

        virtual void visit(pTerrainChunkDomainObject_type _pDomainObject)
        {
            // Only use the first DO. There can be only one.
            if (m_firstTime)
            {
                m_parent.setTerrainChunkDO(_pDomainObject);
                m_firstTime = false;
            }
        }

        virtual void end()
        {
        }

        TerrainChunkVisitor(TerrainChunkNode& _parent)
        :   m_parent(_parent)
        ,   m_firstTime(true)
        {
        }

    private:
        TerrainChunkNode&   m_parent;
        bool                m_firstTime;
    };  // struct TerrainChunkVisitor

    TerrainChunkVisitor visitor(*this);

    pTerrainChunkDM->getByExplorerNodeId(getNode()->getNodeId())->getValue()
        ->getAll(visitor);

    m_properties.setPropertiesId(m_pTerrainChunkDO->getPropertiesId());
    m_properties.load(_pDBConn);

    getNode()->setDisplayName(m_pTerrainChunkDO->getName());
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
TerrainChunkNode::save(pDatabaseConnection_type _pDBConn)
{
    pTerrainChunkDataMap_type pTerrainChunkDM = 
        I_TerrainChunkDataMap::create(_pDBConn);

    m_pTerrainChunkDO->setPropertiesId(m_properties.getPropertiesId());

    pTerrainChunkDM->update(m_pTerrainChunkDO);
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
TerrainChunkNode::remove(pDatabaseConnection_type _pDBConn)
{
    pTerrainChunkDataMap_type pTerrainChunkDM =
        I_TerrainChunkDataMap::create(_pDBConn);

    pTerrainChunkDM->deleteByKey(m_pTerrainChunkDO->getTerrainChunkId());
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
TerrainChunkNode::setTerrainChunkDO(pTerrainChunkDomainObject_type _pTerrainChunkDO)
{
    m_pTerrainChunkDO = _pTerrainChunkDO;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
Project&
TerrainChunkNode::getProject()
{
    return m_project;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
TerrainChunkNode::registerProperties(Zen::Studio::Workbench::I_PropertiesPublisher& _properties)
{
    class PropertyVisitor
    :   public Zen::Plugins::I_ConfigurationElement::I_ConfigurationElementVisitor
    {
    public:
        class CategoryPropertyVisitor
        :   public Zen::Plugins::I_ConfigurationElement::I_ConfigurationElementVisitor
        {
        public:
            virtual void begin()
            {
            }

            virtual void visit(const Zen::Plugins::I_ConfigurationElement& _element)
            {
                std::string name = _element.getAttribute("name");
                std::string typeString = _element.getAttribute("type");

                /// TODO Do this with a const map using boost::assign::map_list_of
                Property::PropertyType propertyType;
                if (typeString == "CATEGORY_TYPE")
                {
                    propertyType = Property::CATEGORY_TYPE;
                }
                else if (typeString == "TRUE_FALSE_PROPERTY")
                {
                    propertyType = Property::TRUE_FALSE_PROPERTY;
                }
                else if (typeString == "ENUM_TYPE")
                {
                    propertyType = Property::ENUM_TYPE;
                }
                else
                {
                    propertyType = Property::STRING_TYPE;
                }

                Property* pProperty = new Property(
                    m_parent.getProperties(),
                    name,
                    "",
                    propertyType
                );

                m_parent.getProperties().registerProperty(pProperty);

                if (propertyType == Property::CATEGORY_TYPE)
                {
                    CategoryPropertyVisitor visitor(*pProperty);
                    _element.getChildren("property", visitor);
                }
            }

            virtual void end()
            {
            }

            CategoryPropertyVisitor(Property& _parent)
            :   m_parent(_parent)
            {
            }

        private:
            Property&   m_parent;

        };  // class CategoryPropertyVisitor

        virtual void begin()
        {
        }

        virtual void visit(const Zen::Plugins::I_ConfigurationElement& _element)
        {
            std::string name = _element.getAttribute("name");
            std::string typeString = _element.getAttribute("type");

            /// TODO Do this with a const map using boost::assign::map_list_of
            Property::PropertyType propertyType;
            if (typeString == "CATEGORY_TYPE")
            {
                propertyType = Property::CATEGORY_TYPE;
            }
            else if (typeString == "TRUE_FALSE_PROPERTY")
            {
                propertyType = Property::TRUE_FALSE_PROPERTY;
            }
            else if (typeString == "ENUM_TYPE")
            {
                propertyType = Property::ENUM_TYPE;
            }
            else
            {
                propertyType = Property::STRING_TYPE;
            }

            Property* pProperty = new Property(
                m_parent.getProperties(),
                name,
                "",
                propertyType
            );

            m_parent.getProperties().registerProperty(pProperty);

            if (propertyType == Property::CATEGORY_TYPE)
            {
                CategoryPropertyVisitor visitor(*pProperty);
                _element.getChildren("property", visitor);
            }
        }

        virtual void end()
        {
        }

        PropertyVisitor(TerrainChunkNode& _parent)
        :   m_parent(_parent)
        {
        }

    private:
        TerrainChunkNode&   m_parent;
    };  // PropertyVisitor

    Zen::Plugins::I_ExtensionRegistry::pExtensionPoint_type pExtensionPoint =
        Zen::Plugins::I_ExtensionRegistry::getSingleton()
            .getExtensionPoint("Zen::Studio::Workbench", "ExplorerNode");

    if (pExtensionPoint.get() != NULL)
    {
        Zen::Plugins::I_Extension::extension_ptr_type pExtension =
            pExtensionPoint->getExtension(getType().getName());

        if (pExtension != NULL)
        {
            const Zen::Plugins::I_ConfigurationElement& config(
                pExtension->getConfigurationElement()
            );

            PropertyVisitor propertyVisitor(*this);
            config.getChild("properties")->getChildren("property", propertyVisitor);
        }
    }
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
Zen::Studio::Workbench::I_PropertiesPublisher&
TerrainChunkNode::getPropertiesPublisher()
{
    return getProperties();
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
Properties&
TerrainChunkNode::getProperties()
{
    return m_properties;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
boost::uint64_t
TerrainChunkNode::getTerrainChunkId() const
{
    return m_pTerrainChunkDO->getTerrainChunkId();
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
TerrainChunkNode::destroyDocument(Zen::Memory::managed_weak_ptr<Zen::Studio::Workbench::I_Document> _wpDocument)
{
    TerrainChunkDocument* pDocument
        = dynamic_cast<TerrainChunkDocument*>(_wpDocument.get());

    if (pDocument != NULL)
    {
        onClosed(pDocument);
        delete pDocument;
    }
    else
    {
        // TODO Error?
    }
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
TerrainChunkNode::pDocument_type
TerrainChunkNode::getDocument()
{
    if (m_pDocument.isValid())
    {
        return m_pDocument;
    }

    // Create a document and return it

    // Get a reference to the workbench
    Zen::Studio::Workbench::I_WorkbenchService&
        workbench(m_project.getController().getWorkbenchService());

    // Create the document.
    TerrainChunkDocument* pRawDocument =
        new TerrainChunkDocument(m_project);
    pDocument_type pTerrainChunkDocument(pRawDocument, &destroyDocument);

    // TODO Create a future and pass it to this method. Then have this method
    // fire the future event when the document is fully loaded.
    pRawDocument->load(*this);

    m_pDocument = pTerrainChunkDocument;

    onOpened(pRawDocument);

    // TODO Return a future
    return m_pDocument;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace TerrainBuilder
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
