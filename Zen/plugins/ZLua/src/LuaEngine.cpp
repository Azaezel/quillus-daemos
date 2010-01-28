//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
// Zen Game Engine Framework
//
// Copyright (C) 2001 - 2008 Tony Richards
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

extern "C" {
#include <lauxlib.h>
#include <lualib.h>
#include <lua.h>
}

#include "LuaEngine.hpp"
#include "LuaModule.hpp"

#include <Zen/Core/Scripting/I_ScriptModule.hpp>
#include <Zen/Core/Scripting/I_ObjectReference.hpp>

#include <Zen/Core/Math/Matrix4.hpp>
#include <Zen/Core/Math/Point3.hpp>
#include <Zen/Core/Math/Vector3.hpp>
#include <Zen/Core/Math/Vector4.hpp>
#include <Zen/Core/Math/Quaternion4.hpp>

#include <Zen/Core/Utility/runtime_exception.hpp>

#include <boost/bind.hpp>
#include <boost/function.hpp>

#include <iostream>

#include <stdio.h>
#ifndef WIN32
#include <stdint.h>
#endif
#include <stdlib.h>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Zen {
namespace ZLua {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
LuaEngine::LuaEngine()
{
#if 0
    m_pHeap =
        pObjectHeap_type(new LuaHeap(this),
        boost::bind(&LuaEngine::onDestroyObjectHeap,this,_1));
#endif

    // http://www.lua.org/pil/24.1.html
    m_pLua = lua_open();

    registerModules();
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
LuaEngine::~LuaEngine()
{
    lua_close(m_pLua);
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
Scripting::I_ObjectHeap&
LuaEngine::heap()
{
    //return *m_pHeap;
    throw Utility::runtime_exception("LuaEngine::heap(): Error, not implemented");
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
bool
LuaEngine::executeScript(const std::string& _fileName)
{
    // TODO This should really use a resource manager, but for now just do it this way

    luaL_loadfile(m_pLua, _fileName.c_str());

    if (lua_pcall(m_pLua, 0, 0, 0) != 0)
    {
        std::string errString = lua_tostring(m_pLua, -1);
        std::cout << errString << std::endl;
    }

    return true;
}

static int new_Point3(lua_State *L, Math::Point3 _point3);

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
enum TYPES
{
    OBJECT_REFERENCE_TYPE,
    INT_TYPE,
    STRING_TYPE,
    REAL_TYPE,
    POINT3_TYPE
};

typedef std::map<std::string, TYPES> TypeMap_type;
static TypeMap_type sm_typeMap;

static bool sm_initialized = false;

static void initTypeHuntMaps()
{
    sm_initialized = true;
    sm_typeMap[typeid(Scripting::I_ObjectReference*).name()] = OBJECT_REFERENCE_TYPE;
    sm_typeMap[typeid(int).name()] = INT_TYPE;
    sm_typeMap[typeid(std::string).name()] = STRING_TYPE;
    sm_typeMap[typeid(Math::Real).name()] = REAL_TYPE;
    sm_typeMap[typeid(Math::Point3).name()] = POINT3_TYPE;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
LuaEngine::executeMethod(boost::any& _object, boost::any& _method, std::vector<boost::any>& _parms)
{
    if (!sm_initialized)
    {
        initTypeHuntMaps();
    }

    lua_State* const L = getState();

    int object;

    TypeMap_type::iterator pType = sm_typeMap.find(_object.type().name());
    if (pType != sm_typeMap.end())
    {
        switch(pType->second)
        {
        case OBJECT_REFERENCE_TYPE:
            object = (uintptr_t)(boost::any_cast<Scripting::I_ObjectReference*>(_object)->getScriptUserData());
            break;
        case INT_TYPE:
            object = boost::any_cast<int>(_object);
            break;
        default:
            // TODO Error?
            return;
        }
    }
    else
    {
        // TODO Error?
        return;
    }

    int method = boost::any_cast<int>(_method);

    lua_rawgeti(L, LUA_REGISTRYINDEX, method);
    lua_rawgeti(L, LUA_REGISTRYINDEX, object);

    int parms = 1;

    for(std::vector<boost::any>::iterator iter = _parms.begin(); iter != _parms.end(); iter++)
    {
        pType = sm_typeMap.find(iter->type().name());
        if (pType == sm_typeMap.end())
        {
            std::cout << "Error, undefined parameter type " << iter->type().name() << " in LuaEngine::executeMethod: " << iter->type().name() << std::endl;
            return;
        }

        switch(pType->second)
        {
        case OBJECT_REFERENCE_TYPE:
        {
            Scripting::I_ObjectReference* pObj = boost::any_cast<Scripting::I_ObjectReference*>(*iter);
            lua_rawgeti(L, LUA_REGISTRYINDEX, (uintptr_t)pObj->getScriptUserData());
            parms++;
            continue;
        }
        case STRING_TYPE:
        {
            std::string value = boost::any_cast<std::string>(*iter);
            lua_pushstring(L, value.c_str());
            parms++;
            continue;
        }
        case REAL_TYPE:
        {
            Math::Real value = boost::any_cast<Math::Real>(*iter);
            lua_pushnumber(L, value);
            parms++;
            continue;
        }
        case INT_TYPE:
        {
            int value = boost::any_cast<int>(*iter);
            lua_pushinteger(L, value);
            parms++;
            continue;
        }
        case POINT3_TYPE:
        {
            new_Point3(L, boost::any_cast<Math::Point3>(*iter));
            parms++;
            continue;
        }
        default:
            std::cout << "Error, undefined parameter type in LuaEngine::executeMethod: " << iter->type().name() << std::endl;
            // TODO Error
            return;
        }

    }

    if (lua_pcall(m_pLua, parms, 0, 0) != 0)
    {
        std::string errString = lua_tostring(m_pLua, -1);
        std::cout << errString << std::endl;
    }
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
LuaEngine::pScriptModule_type
LuaEngine::createScriptModule(const std::string& _moduleName, const std::string& _docString)
{
    return pScriptModule_type(new LuaModule(this, _moduleName, _docString),
        boost::bind(&LuaEngine::onDestroyScriptModule,this,_1));
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
LuaTypeMap&
LuaEngine::getTypeMap()
{
    return m_typeMap;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
#if 0
LuaEngine::pObjectReference_type
LuaEngine::getObject(PyObject* _pObject)
{
    return m_objectMap[_pObject];
}
#endif

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
#if 0
void
LuaEngine::setObject(PyObject* _pPyObject, pObjectReference_type _pCPPObject)
{
    // Isn't there a better way to do this?  Is there a way to use the PyObject to store the
    // raw C++ pointer?  For now, we just use this map.
    m_objectMap[_pPyObject] = _pCPPObject;
}
#endif

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
LuaEngine::onDestroyObjectHeap(wpObjectHeap_type _pObjectHeap)
{
    /// Fire the ObjectHeap's onDestroyEvent
    _pObjectHeap->onDestroyEvent(_pObjectHeap);

    /// Delete the ObjectHeap
    LuaHeap* pObjectHeap = dynamic_cast<LuaHeap*>(_pObjectHeap.get());

    if( pObjectHeap )
    {
        delete pObjectHeap;
    }
    else
    {
        throw Utility::runtime_exception("Zen::ZLua::LuaEngine::onDestroyObjectHeap() : _pObjectHeap is an invalid LuaHeap.");
    }
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
LuaEngine::onDestroyScriptModule(wpScriptModule_type _pScriptModule)
{
    /// Fire the ScriptModule's onDestroyEvent
    _pScriptModule->onDestroyEvent(_pScriptModule);

    /// Delete the ScriptModule
    LuaModule* pScriptModule = dynamic_cast<LuaModule*>(_pScriptModule.get());

    if( pScriptModule )
    {
        delete pScriptModule;
    }
    else
    {
        throw Utility::runtime_exception("Zen::ZLua::LuaEngine::onDestroyScriptModule() : _pScriptModule is an invalid LuaModule.");
    }
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

static Math::Real checkReal(lua_State *L, int idx)
{
    luaL_argcheck(L, lua_isnumber(L, idx), idx, "Real expected");
    return (Math::Real)lua_tonumber(L, idx);
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
static const char* sm_pMatrix4 = "Matrix4";

static Math::Matrix4* checkMatrix4(lua_State *L, int idx)
{
    lua_pushliteral(L, "__zen_userdata");
    lua_gettable(L, idx);

    Math::Matrix4* pMatrix4 = (Math::Matrix4*)luaL_checkudata(L, -1, sm_pMatrix4);
    luaL_argcheck(L, pMatrix4 != NULL, idx, "Matrix4 expected");
    lua_pop(L, 1);

    return pMatrix4;
}

static int new_Matrix4 (lua_State *L)
{
    const int top = lua_gettop(L);

    lua_newtable(L);
    const int table = lua_gettop(L);

    Math::Matrix4* pMatrix4 = new (lua_newuserdata(L, sizeof(Math::Matrix4))) Math::Matrix4(Math::Matrix4::INIT_IDENTITY);
    const int userdata = lua_gettop(L);

    // T["__zen_userdata"] = UD
    lua_pushvalue(L, userdata);
    lua_setfield(L, table, "__zen_userdata");

    // Get the meta table for this class type and set it for this object
    luaL_getmetatable(L, sm_pMatrix4);
    lua_setmetatable(L, table);

    // Get the meta table for this class type and set it for the user data
    luaL_getmetatable(L, sm_pMatrix4);
    lua_setmetatable(L, userdata);

    // Pop the userdata so only the table is left
    lua_pop(L, 1);

    for(int x = 1; x < 17; x++)
    {
        if (x < top)
        {
            pMatrix4->m_array[x - 1] = lua_tonumber(L, x);
        }
        else
        {
            pMatrix4->m_array[x - 1] = 0.0f;
        }
    }

    return 1;
}


static int setXRotation_Matrix4(lua_State *L)
{
    Math::Matrix4* pMatrix4 = checkMatrix4(L, 1);
    Math::Radian rotation(checkReal(L, 2));

    pMatrix4->setXRotation(rotation);
    return 0;
}

static int setYRotation_Matrix4(lua_State *L)
{
    Math::Matrix4* pMatrix4 = checkMatrix4(L, 1);
    Math::Radian rotation(checkReal(L, 2));

    pMatrix4->setYRotation(rotation);
    return 0;
}

static int setZRotation_Matrix4(lua_State *L)
{
    Math::Matrix4* pMatrix4 = checkMatrix4(L, 1);
    Math::Radian rotation(checkReal(L, 2));

    pMatrix4->setZRotation(rotation);
    return 0;
}

static const struct luaL_reg MatrixLib [] = {
    {"setXRotation", setXRotation_Matrix4},
    {"setYRotation", setYRotation_Matrix4},
    {"setZRotation", setZRotation_Matrix4},
    {NULL, NULL}
};

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

static const char* sm_pPoint3= "Point3";

static Math::Point3* checkPoint3(lua_State *L, int idx)
{
    lua_pushliteral(L, "__zen_userdata");
    lua_gettable(L, idx);

    Math::Point3* pPoint3 = (Math::Point3*)luaL_checkudata(L, -1, sm_pPoint3);
    luaL_argcheck(L, pPoint3 != NULL, idx, "Point3 expected");
    lua_pop(L, 1);

    return pPoint3;
}

static int new_Point3 (lua_State *L, Math::Point3 _point3)
{
    const int top = lua_gettop(L);

    lua_newtable(L);
    const int table = lua_gettop(L);

    Math::Point3* pPoint3 = new (lua_newuserdata(L, sizeof(Math::Point3))) Math::Point3(_point3);
    const int userdata = lua_gettop(L);

    // T["__zen_userdata"] = UD
    lua_pushvalue(L, userdata);
    lua_setfield(L, table, "__zen_userdata");

    // Get the meta table for this class type and set it for this object
    luaL_getmetatable(L, sm_pPoint3);
    lua_setmetatable(L, table);

    // Get the meta table for this class type and set it for the user data
    luaL_getmetatable(L, sm_pPoint3);
    lua_setmetatable(L, userdata);

    // Pop the userdata so only the table is left
    lua_pop(L, 1);

    return 1;
}

static int new_Point3NoArgs(lua_State *L)
{
    return new_Point3(L, Math::Point3());
}

static int setX_Point3(lua_State *L)
{
    Math::Point3* pPoint3 = checkPoint3(L, 1);
    Math::Real value = checkReal(L, 2);

    pPoint3->setX(value);
    return 0;
}

static int setY_Point3(lua_State *L)
{
    Math::Point3* pPoint3 = checkPoint3(L, 1);
    Math::Real value = checkReal(L, 2);

    pPoint3->setY(value);
    return 0;
}

static int setZ_Point3(lua_State *L)
{
    Math::Point3* pPoint3 = checkPoint3(L, 1);
    Math::Real value = checkReal(L, 2);

    pPoint3->setZ(value);
    return 0;
}


static int getX_Point3(lua_State *L)
{
    Math::Point3* pPoint3 = checkPoint3(L, 1);

    lua_pushnumber(L, pPoint3->getX());
    return 1;
}

static int getY_Point3(lua_State *L)
{
    Math::Point3* pPoint3 = checkPoint3(L, 1);

    lua_pushnumber(L, pPoint3->getY());
    return 1;
}

static int getZ_Point3(lua_State *L)
{
    Math::Point3* pPoint3 = checkPoint3(L, 1);

    lua_pushnumber(L, pPoint3->getZ());
    return 1;
}

static const struct luaL_reg Point3Lib [] = {
    {"setX", setX_Point3},
    {"setY", setY_Point3},
    {"setZ", setZ_Point3},
    {"getX", getX_Point3},
    {"getY", getY_Point3},
    {"getZ", getZ_Point3},
    {NULL, NULL}
};

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

static const char* sm_pVector3 = "Vector3";

static Math::Vector3* checkVector3(lua_State *L, int idx)
{
    lua_pushliteral(L, "__zen_userdata");
    lua_gettable(L, idx);

    Math::Vector3* pVector3 = (Math::Vector3*)luaL_checkudata(L, -1, sm_pVector3);
    luaL_argcheck(L, pVector3 != NULL, idx, "Vector3 expected");
    lua_pop(L, 1);

    return pVector3;
}

static int new_Vector3 (lua_State *L)
{
    const int top = lua_gettop(L);

    lua_newtable(L);
    const int table = lua_gettop(L);

    Math::Vector3* pVector3 = new (lua_newuserdata(L, sizeof(Math::Vector3))) Math::Vector3();
    const int userdata = lua_gettop(L);

    // T["__zen_userdata"] = UD
    lua_pushvalue(L, userdata);
    lua_setfield(L, table, "__zen_userdata");

    // Get the meta table for this class type and set it for this object
    luaL_getmetatable(L, sm_pVector3);
    lua_setmetatable(L, table);

    // Get the meta table for this class type and set it for the user data
    luaL_getmetatable(L, sm_pVector3);
    lua_setmetatable(L, userdata);

    // Pop the userdata so only the table is left
    lua_pop(L, 1);

    for(int x = 1; x < 4; x++)
    {
        if (x <= top)
        {
            pVector3->m_array[x - 1] = lua_tonumber(L, x + 1);
        }
        else
        {
            pVector3->m_array[x - 1] = 0.0f;
        }
    }

    return 1;
}

static int setX_Vector3(lua_State *L)
{
    Math::Vector3* pVector3 = checkVector3(L, 1);
    pVector3->m_x = checkReal(L, 2);
    return 0;
}

static int setY_Vector3(lua_State *L)
{
    Math::Vector3* pVector3 = checkVector3(L, 1);
    pVector3->m_y = checkReal(L, 2);
    return 0;
}

static int setZ_Vector3(lua_State *L)
{
    Math::Vector3* pVector3 = checkVector3(L, 1);
    pVector3->m_z = checkReal(L, 2);
    return 0;
}

static int getX_Vector3(lua_State *L)
{
    Math::Vector3* pVector3 = checkVector3(L, 1);

    lua_pushnumber(L, pVector3->m_x);
    return 1;
}

static int getY_Vector3(lua_State *L)
{
    Math::Vector3* pVector3 = checkVector3(L, 1);

    lua_pushnumber(L, pVector3->m_y);
    return 1;
}

static int getZ_Vector3(lua_State *L)
{
    Math::Vector3* pVector3 = checkVector3(L, 1);

    lua_pushnumber(L, pVector3->m_z);
    return 1;
}

static const struct luaL_reg Vector3Lib [] = {
    {"setX", setX_Vector3},
    {"setY", setY_Vector3},
    {"setZ", setZ_Vector3},
    {"getX", getX_Vector3},
    {"getY", getY_Vector3},
    {"getZ", getZ_Vector3},
    {NULL, NULL}
};

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

static const char* sm_pVector4= "Vector4";

static Math::Vector4* checkVector4(lua_State *L, int idx)
{
    lua_pushliteral(L, "__zen_userdata");
    lua_gettable(L, idx);

    Math::Vector4* pVector4 = (Math::Vector4*)luaL_checkudata(L, -1, sm_pVector4);
    luaL_argcheck(L, pVector4 != NULL, idx, "Vector4 expected");
    lua_pop(L, 1);

    return pVector4;
}

static int new_Vector4 (lua_State *L)
{
    const int top = lua_gettop(L);

    lua_newtable(L);
    const int table = lua_gettop(L);

    Math::Vector4* pVector4 = new (lua_newuserdata(L, sizeof(Math::Vector4))) Math::Vector4();
    const int userdata = lua_gettop(L);

    // T["__zen_userdata"] = UD
    lua_pushvalue(L, userdata);
    lua_setfield(L, table, "__zen_userdata");

    // Get the meta table for this class type and set it for this object
    luaL_getmetatable(L, sm_pVector4);
    lua_setmetatable(L, table);

    // Get the meta table for this class type and set it for the user data
    luaL_getmetatable(L, sm_pVector4);
    lua_setmetatable(L, userdata);

    // Pop the userdata so only the table is left
    lua_pop(L, 1);

    for(int x = 1; x < 5; x++)
    {
        if (x <= top)
        {
            pVector4->m_array[x - 1] = lua_tonumber(L, x);
        }
        else
        {
            pVector4->m_array[x - 1] = 0.0f;
        }
    }

    return 1;
}

static int setX_Vector4(lua_State *L)
{
    Math::Vector4* pVector4 = checkVector4(L, 1);
    pVector4->m_x = checkReal(L, 2);
    return 0;
}

static int setY_Vector4(lua_State *L)
{
    Math::Vector4* pVector4 = checkVector4(L, 1);
    pVector4->m_y = checkReal(L, 2);
    return 0;
}

static int setZ_Vector4(lua_State *L)
{
    Math::Vector4* pVector4 = checkVector4(L, 1);
    pVector4->m_z = checkReal(L, 2);
    return 0;
}

static int setW_Vector4(lua_State *L)
{
    Math::Vector4* pVector4 = checkVector4(L, 1);
    pVector4->m_w = checkReal(L, 2);
    return 0;
}

static int getX_Vector4(lua_State *L)
{
    Math::Vector4* pVector4 = checkVector4(L, 1);

    lua_pushnumber(L, pVector4->m_x);
    return 1;
}

static int getY_Vector4(lua_State *L)
{
    Math::Vector4* pVector4 = checkVector4(L, 1);

    lua_pushnumber(L, pVector4->m_y);
    return 1;
}

static int getZ_Vector4(lua_State *L)
{
    Math::Vector4* pVector4 = checkVector4(L, 1);

    lua_pushnumber(L, pVector4->m_z);
    return 1;
}

static int getW_Vector4(lua_State *L)
{
    Math::Vector4* pVector4 = checkVector4(L, 1);

    lua_pushnumber(L, pVector4->m_w);
    return 1;
}

static const struct luaL_reg Vector4Lib [] = {
    {"setX", setX_Vector4},
    {"setY", setY_Vector4},
    {"setZ", setZ_Vector4},
    {"setW", setW_Vector4},
    {"getX", getX_Vector4},
    {"getY", getY_Vector4},
    {"getZ", getZ_Vector4},
    {"getW", getW_Vector4},
    {NULL, NULL}
};


//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

static const char* sm_pQuaternion4 = "Quaternion4";

static Math::Quaternion4* checkQuaternion4(lua_State *L, int idx)
{
    lua_pushliteral(L, "__zen_userdata");
    lua_gettable(L, idx);

    Math::Quaternion4* pQuaternion4 = (Math::Quaternion4*)luaL_checkudata(L, -1, sm_pQuaternion4);
    luaL_argcheck(L, pQuaternion4 != NULL, idx, "Quaternion4 expected");
    lua_pop(L, 1);

    return pQuaternion4;
}

static int new_Quaternion4 (lua_State *L)
{
    const int top = lua_gettop(L);

    lua_newtable(L);
    const int table = lua_gettop(L);

    Math::Quaternion4* pQuaternion4 = new (lua_newuserdata(L, sizeof(Math::Quaternion4))) Math::Quaternion4();
    const int userdata = lua_gettop(L);

    // T["__zen_userdata"] = UD
    lua_pushvalue(L, userdata);
    lua_setfield(L, table, "__zen_userdata");

    // Get the meta table for this class type and set it for this object
    luaL_getmetatable(L, sm_pQuaternion4);
    lua_setmetatable(L, table);

    // Get the meta table for this class type and set it for the user data
    luaL_getmetatable(L, sm_pQuaternion4);
    lua_setmetatable(L, userdata);

    // Pop the userdata so only the table is left
    lua_pop(L, 1);

    for(int x = 1; x < 5; x++)
    {
        if (x <= top)
        {
            pQuaternion4->m_array[x - 1] = lua_tonumber(L, x);
        }
        else
        {
            pQuaternion4->m_array[x - 1] = 0.0f;
        }
    }

    return 1;
}

static int setX_Quaternion4(lua_State *L)
{
    Math::Quaternion4* pQuaternion4 = checkQuaternion4(L, 1);
    pQuaternion4->m_x = checkReal(L, 2);
    return 0;
}

static int setY_Quaternion4(lua_State *L)
{
    Math::Quaternion4* pQuaternion4 = checkQuaternion4(L, 1);
    pQuaternion4->m_y = checkReal(L, 2);
    return 0;
}

static int setZ_Quaternion4(lua_State *L)
{
    Math::Quaternion4* pQuaternion4 = checkQuaternion4(L, 1);
    pQuaternion4->m_z = checkReal(L, 2);
    return 0;
}

static int setW_Quaternion4(lua_State *L)
{
    Math::Quaternion4* pQuaternion4 = checkQuaternion4(L, 1);
    pQuaternion4->m_w = checkReal(L, 2);
    return 0;
}

static int getX_Quaternion4(lua_State *L)
{
    Math::Quaternion4* pQuaternion4 = checkQuaternion4(L, 1);

    lua_pushnumber(L, pQuaternion4->m_x);
    return 1;
}

static int getY_Quaternion4(lua_State *L)
{
    Math::Quaternion4* pQuaternion4 = checkQuaternion4(L, 1);

    lua_pushnumber(L, pQuaternion4->m_y);
    return 1;
}

static int getZ_Quaternion4(lua_State *L)
{
    Math::Quaternion4* pQuaternion4 = checkQuaternion4(L, 1);

    lua_pushnumber(L, pQuaternion4->m_z);
    return 1;
}

static int getW_Quaternion4(lua_State *L)
{
    Math::Quaternion4* pQuaternion4 = checkQuaternion4(L, 1);

    lua_pushnumber(L, pQuaternion4->m_w);
    return 1;
}

static const struct luaL_reg Quaternion4Lib [] = {
    {"setX", setX_Quaternion4},
    {"setY", setY_Quaternion4},
    {"setZ", setZ_Quaternion4},
    {"setW", setW_Quaternion4},
    {"getX", getX_Quaternion4},
    {"getY", getY_Quaternion4},
    {"getZ", getZ_Quaternion4},
    {"getW", getW_Quaternion4},
    {NULL, NULL}
};

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

static const luaL_Reg MathLib[] = {
    {sm_pMatrix4,       new_Matrix4},
    {sm_pVector3,       new_Vector3},
    {sm_pVector4,       new_Vector4},
    {sm_pPoint3,        new_Point3NoArgs},
    {sm_pQuaternion4,   new_Quaternion4},
    {NULL, NULL}
};

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
LuaEngine::registerModules()
{
    lua_State* const L = getState();

    luaL_openlibs(L);

    luaL_register(L, "Math", MathLib);

    // Register the Matrix meta table
    luaL_newmetatable(L, sm_pMatrix4);

    // metatable.__index = metatable
    lua_pushstring(L, "__index");
    lua_pushvalue(L, -2);
    lua_settable(L, -3);

    luaL_openlib(L, NULL, MatrixLib, 0);

    // Pop the metatable
    lua_pop(L, 1);

    //-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

    // Register the Point3 meta table
    luaL_newmetatable(L, sm_pPoint3);

    // metatable.__index = metatable
    lua_pushstring(L, "__index");
    lua_pushvalue(L, -2);
    lua_settable(L, -3);

    luaL_openlib(L, NULL, Point3Lib, 0);

    // Pop the metatable
    lua_pop(L, 1);

    //-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

    // Register the Vector3 meta table
    luaL_newmetatable(L, sm_pVector3);

    // metatable.__index = metatable
    lua_pushstring(L, "__index");
    lua_pushvalue(L, -2);
    lua_settable(L, -3);

    luaL_openlib(L, NULL, Vector3Lib, 0);

    // Pop the metatable
    lua_pop(L, 1);


    //-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

    // Register the Vector4 meta table
    luaL_newmetatable(L, sm_pVector4);

    // metatable.__index = metatable
    lua_pushstring(L, "__index");
    lua_pushvalue(L, -2);
    lua_settable(L, -3);

    luaL_openlib(L, NULL, Vector4Lib, 0);

    // Pop the metatable
    lua_pop(L, 1);

    //-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

    // Register the Quaternion4 meta table
    luaL_newmetatable(L, sm_pQuaternion4);

    // metatable.__index = metatable
    lua_pushstring(L, "__index");
    lua_pushvalue(L, -2);
    lua_settable(L, -3);

    luaL_openlib(L, NULL, Quaternion4Lib, 0);

    // Pop the metatable
    lua_pop(L, 1);
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace ZLua
}   // namespace Zen
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
