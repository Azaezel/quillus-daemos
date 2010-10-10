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
#include "TerrainGroupNode.hpp"
#include "TerrainChunkNode.hpp"
#include "Project.hpp"
#include "TerrainGroupDocument.hpp"
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

#include <Zen/StudioPlugins/TerrainBuilderModel/I_TerrainGroupDomainObject.hpp>
#include <Zen/StudioPlugins/TerrainBuilderModel/I_TerrainGroupDataCollection.hpp>

#include <Zen/Studio/WorkbenchCommon/I_View.hpp>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace TerrainBuilder {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
TerrainGroupNode::DocumentEvent_type    TerrainGroupNode::onOpened;
TerrainGroupNode::DocumentEvent_type    TerrainGroupNode::onClosed;
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
TerrainGroupNode::TerrainGroupNode(Project& _project, const std::string& _name)
:   I_TerrainGroupNode(_name)
,   m_project(_project)
,   m_properties(*this)
{
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
TerrainGroupNode::~TerrainGroupNode()
{
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
TerrainGroupNode::setName(const std::string& _name)
{
    // Only do this if the name changes
    if (getName() != _name)
    {
        m_pTerrainGroupDO->getName() = _name;

        // Call the base class set name, which will fire the "modified" event.
        UserData::setName(_name);
    }
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
Zen::Studio::Workbench::I_ExplorerNodeType&
TerrainGroupNode::getType() const
{
    return I_TerrainGroupNode::getNodeType();
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
TerrainGroupNode::insert(pDatabaseConnection_type _pDBConn)
{
    m_properties.insert(_pDBConn);

    pTerrainGroupDataMap_type pTerrainGroupDM = 
        I_TerrainGroupDataMap::create(_pDBConn);

    m_pTerrainGroupDO = pTerrainGroupDM->createNew();

    m_pTerrainGroupDO->getExplorerNodeId() = getNode()->getNodeId();
    m_pTerrainGroupDO->getName() = getNode()->getDisplayName();
    m_pTerrainGroupDO->getPropertiesId() = m_properties.getPropertiesId();

    pTerrainGroupDM->update(m_pTerrainGroupDO);
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
TerrainGroupNode::load(pDatabaseConnection_type _pDBConn)
{
    pTerrainGroupDataMap_type pTerrainGroupDM = 
        I_TerrainGroupDataMap::create(_pDBConn);

    struct TerrainGroupVisitor
    :   public I_TerrainGroupDataCollection::I_CollectionVisitor
    {
        virtual void begin()
        {
        }

        virtual void visit(pTerrainGroupDomainObject_type _pDomainObject)
        {
            // Only use the first DO. There can be only one.
            if (m_firstTime)
            {
                m_parent.setTerrainGroupDO(_pDomainObject);
                m_firstTime = false;
            }
        }

        virtual void end()
        {
        }

        TerrainGroupVisitor(TerrainGroupNode& _parent)
        :   m_parent(_parent)
        ,   m_firstTime(true)
        {
        }

    private:
        TerrainGroupNode&   m_parent;
        bool                m_firstTime;
    };  // struct TerrainGroupVisitor

    TerrainGroupVisitor visitor(*this);

    pTerrainGroupDM->getByExplorerNodeId(getNode()->getNodeId())->getValue()
        ->getAll(visitor);

    m_properties.setPropertiesId(m_pTerrainGroupDO->getPropertiesId());
    m_properties.load(_pDBConn);

    getNode()->setDisplayName(m_pTerrainGroupDO->getName());
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
TerrainGroupNode::save(pDatabaseConnection_type _pDBConn)
{
    pTerrainGroupDataMap_type pTerrainGroupDM = 
        I_TerrainGroupDataMap::create(_pDBConn);

    m_pTerrainGroupDO->setPropertiesId(m_properties.getPropertiesId());

    pTerrainGroupDM->update(m_pTerrainGroupDO);
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
TerrainGroupNode::remove(pDatabaseConnection_type _pDBConn)
{
    pTerrainGroupDataMap_type pTerrainGroupDM =
        I_TerrainGroupDataMap::create(_pDBConn);

    pTerrainGroupDM->deleteByKey(m_pTerrainGroupDO->getTerrainGroupId());
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
TerrainGroupNode::setTerrainGroupDO(pTerrainGroupDomainObject_type _pTerrainGroupDO)
{
    m_pTerrainGroupDO = _pTerrainGroupDO;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
Project&
TerrainGroupNode::getProject()
{
    return m_project;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
TerrainGroupNode::registerProperties(Zen::Studio::Workbench::I_PropertiesPublisher& _properties)
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
                    propertyType,
                    &m_parent
                );

                m_parent.addChild(name, pProperty);
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

        PropertyVisitor(TerrainGroupNode& _parent)
        :   m_parent(_parent)
        {
        }

    private:
        TerrainGroupNode&   m_parent;
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
TerrainGroupNode::getPropertiesPublisher()
{
    return getProperties();
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
Properties&
TerrainGroupNode::getProperties()
{
    return m_properties;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
boost::uint64_t
TerrainGroupNode::getTerrainGroupId() const
{
    return m_pTerrainGroupDO->getTerrainGroupId();
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
TerrainGroupNode::destroyDocument(Zen::Memory::managed_weak_ptr<Zen::Studio::Workbench::I_Document> _wpDocument)
{
    TerrainGroupDocument* pDocument =
        dynamic_cast<TerrainGroupDocument*>(_wpDocument.get());

    if (pDocument != NULL)
    {
        delete pDocument;
    }
    else
    {
        // TODO Error?
    }
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
TerrainGroupNode::pDocument_type
TerrainGroupNode::getDocument()
{
    if (m_pDocument.isValid())
    {
        return m_pDocument;
    }

    // Create a document and return it

    // Get a reference to the workbench
    Zen::Studio::Workbench::I_WorkbenchService&
        workbench(m_project.getController().getWorkbenchService());

#if 0
    /// Create a Scene model user data
    Zen::Studio::Workbench::I_SceneModel::pUserData_type
        pUserData(new TerrainGroupDocument(m_project));

    /// Create a document and return it.
    Zen::Studio::Workbench::I_WorkbenchService::pSceneModel_type
        pSceneModel = workbench.createSceneModel(getNode(), &m_project, pUserData);

    m_pDocument = pSceneModel;
#endif  // 0

    /// Create the document.
    TerrainGroupDocument* pRawDocument = new TerrainGroupDocument(m_project);
    Zen::Studio::Workbench::I_Document::pDocument_type
        pTerrainGroupDocument(pRawDocument, &TerrainGroupNode::destroyDocument);

    pRawDocument->load(*this);

    m_pDocument = pTerrainGroupDocument;

    // TODO Return a future
    return m_pDocument;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
TerrainGroupNode::addChild(Zen::Studio::Workbench::I_ExplorerNode& _selectedNode)
{
    Zen::Studio::Workbench::I_ExplorerNode::pUserData_type pData(
        new TerrainChunkNode(m_project, "NewTerrainChunk")
    );
    
    m_project.getController().createChildNode(*getNode(), pData);
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
TerrainChunkNode*
TerrainGroupNode::getTerrainChunkByEntity(const Zen::Studio::Workbench::I_SceneEntity& _entity)
{
    // TODO Implement.
    return NULL;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace TerrainBuilder
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
