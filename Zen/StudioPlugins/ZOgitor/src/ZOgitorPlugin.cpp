//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
// World Builder
//
// Copyright (C) 2001 - 2008 Tony Richards
//
// Licensed under the Worlds by Sarge Publishing License - See your licensing
// agreement for terms and conditions.
//
// Do not redistribute this source code.
//
// Tony Richards trichards@gamesbysarge.com
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
#include <Zen/Core/Plugins/Utilities.hpp>

#include "ProjectServiceFactory.hpp"
#include "ExplorerNodeFactory.hpp"

IMPLEMENT_SINGLE_ZEN_PLUGIN(WorldBuilderServerModule, WorldBuilderServerPlugin)

BEGIN_ZEN_EXTENSION_MAP(WorldBuilderServerPlugin)
    ZEN_EXTENSION("Zen::Studio::Workbench::ProjectService", &WorldBuilder::ProjectServiceFactory::getSingleton())
    ZEN_EXTENSION("Zen::Studio::Workbench::ExplorerNode", &WorldBuilder::ExplorerNodeFactory::getSingleton())
END_ZEN_EXTENSION_MAP()
