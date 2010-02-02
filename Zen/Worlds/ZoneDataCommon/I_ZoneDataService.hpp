//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
// Zen Studio Workbench Framework
//
// Copyright (C) 2001 - 2009 Tony Richards
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
#ifndef ZEN_WORLDS_ZONE_DATA_I_ZONE_DATA_SERVICE_HPP_INCLUDED
#define ZEN_WORLDS_ZONE_DATA_I_ZONE_DATA_SERVICE_HPP_INCLUDED

#include "Configuration.hpp"

#include <Zen/Core/Memory/managed_ptr.hpp>
#include <Zen/Core/Event/Event.hpp>
#include <Zen/Core/Event/future_return_value.hpp>

#include <Zen/Enterprise/AppServer/I_ApplicationService.hpp>
#include <Zen/Enterprise/Networking/I_Endpoint.hpp>
#include <Zen/Enterprise/Database/I_DatabaseConnection.hpp>

#include <Zen/Studio/WorkbenchCommon/I_SpreadSheetDocument.hpp>
#include <Zen/Studio/WorkbenchCommon/I_SpreadSheetCell.hpp>

#include <Zen/Studio/WorkbenchCommon/I_SceneModel.hpp>
#include <Zen/Studio/WorkbenchCommon/I_ExplorerNode.hpp>

#include <boost/filesystem.hpp>

#include <string>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Zen {
namespace Worlds {
namespace ZoneData {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
;

/// Workbench Service interface.
/// See this discussion for more details about this design.
/// http://www.indiezen.org/wiki/irclogs/2009/03/19/#UTC2009-03-19T14:27:25
/// @see I_ApplicationServer::getApplication() to get an instance of an 
///     implementation of this interface.
class WORKBENCHCOMMON_DLL_LINK I_ZoneDataService
:   public Enterprise::AppServer::I_ApplicationService
{
    /// @name Types
    /// @{
public:
    typedef Memory::managed_ptr<Networking::I_Endpoint>                 pEndpoint_type;
	typedef Memory::managed_ptr<I_ZoneDataService>			            pService_type;
    /// @}

    /// @name I_ZoneDataService interface
    /// @{
public:
    /// Connect to the zone server for the specified ZoneId.
    virtual pFutureZoneDataSession_type connect(pSession_type _pSession, pZoneId_type _pZoneId) = 0;
    /// @}

    /// @name 'Structors
    /// @{
protected:
             I_ZoneDataService();
    virtual ~I_ZoneDataService();
    /// @}

};  // interface I_ZoneDataService

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace ZoneData
}   // namespace Worlds
namespace Memory
{
    /// I_ZoneDataService is managed by a factory
    template<>
    struct is_managed_by_factory<Worlds::ZoneData::I_ZoneDataService> : public boost::true_type{};
}   // namespace Memory
}   // namespace Zen
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

#endif // ZEN_WORLDS_ZONE_DATA_I_ZONE_DATA_SERVICE_HPP_INCLUDED
