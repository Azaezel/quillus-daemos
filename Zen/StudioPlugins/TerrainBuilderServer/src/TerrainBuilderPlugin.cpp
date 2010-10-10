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
#include <Zen/Core/Plugins/Utilities.hpp>

#include "ProjectServiceFactory.hpp"
#include "ExplorerNodeFactory.hpp"

IMPLEMENT_SINGLE_ZEN_PLUGIN(TerrainBuilderServerModule, TerrainBuilderServerPlugin)

BEGIN_ZEN_EXTENSION_MAP(TerrainBuilderServerPlugin)
    ZEN_EXTENSION("Zen::Studio::Workbench::ProjectService", &TerrainBuilder::ProjectServiceFactory::getSingleton())
    ZEN_EXTENSION("Zen::Studio::Workbench::ExplorerNode", &TerrainBuilder::ExplorerNodeFactory::getSingleton())
END_ZEN_EXTENSION_MAP()
