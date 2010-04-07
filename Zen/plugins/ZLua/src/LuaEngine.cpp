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
#include "Config.hpp"

#include <Zen/Core/Scripting/I_ScriptModule.hpp>
#include <Zen/Core/Scripting/I_ObjectReference.hpp>

#include <Zen/Core/Math/Matrix4.hpp>
#include <Zen/Core/Math/Point3.hpp>
#include <Zen/Core/Math/Vector3.hpp>
#include <Zen/Core/Math/Vector4.hpp>
#include <Zen/Core/Math/Radian.hpp>
#include <Zen/Core/Math/Degree.hpp>
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
static
std::string
getLuaErrorString(int _error)
{
    std::string errorMessage;

    switch(_error)
    {
    case LUA_ERRSYNTAX:
        errorMessage = "Lua syntax error";
        break;
    case LUA_ERRMEM:
        errorMessage = "Lua out of memory error";
        break;
    case LUA_ERRRUN:
        errorMessage = "Lua runtime error";
        break;
    case LUA_ERRERR:
        errorMessage = "Lua fatal error";
        break;
    case LUA_ERRFILE:
        errorMessage = "Lua file not found error";
        break;
    case LUA_YIELD:
        errorMessage = "Lua yield";
        break;
    }

    return errorMessage;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
bool
LuaEngine::executeScript(const std::string& _fileName)
{
    int luaTop = lua_gettop(m_pLua);

    // TODO This should really use a resource manager, but for now just do it this way
    int error;

    // Load a file.  The chunk is on top of the stack.
    if ((error = luaL_loadfile(m_pLua, _fileName.c_str())) != 0)
    {
        std::stringstream errString;
        errString << getLuaErrorString(error)
            << " while loading file " << _fileName << "." << std::endl;

        errString << lua_tostring(m_pLua, -1) << std::endl;

        // Pop the error message off the stack.
        lua_pop(m_pLua, 1);

        throw Utility::runtime_exception(errString.str());
    }

    int errorHandler = 0;

    lua_getglobal(m_pLua, "errorHandler");
    if (!lua_isnil(m_pLua, lua_gettop(m_pLua)))
    {
        // Set the error handler
        errorHandler = -2;
    }

    // Duplicate the chunk so we can keep a reference to it.
    // This also makes the chunk be on the top of the stack.
    lua_pushvalue(m_pLua, -2);

    // Call the chunk.
    if (error = lua_pcall(m_pLua, 0, 0, errorHandler) != 0)
    {
        std::stringstream errString;
        errString << getLuaErrorString(error) << " "
            << lua_tostring(m_pLua, -1) << std::endl;

        lua_getglobal(m_pLua, "errorHandler");

        if (!lua_isnil(m_pLua, lua_gettop(m_pLua)))
        {
            lua_pushstring(m_pLua, errString.str().c_str());
            lua_pcall(m_pLua, 1, 0, 0);
        }
        else
        {
            // TODO No errorHandler function exists.  Should this
            // throw an exception or should an error message be output?
            // For now just output an error message.
            std::cout << errString.str() << std::endl;
        }

        // Who knows what's leftover on the stack?  Just 
        // punt and set it to where we know it should be.
        lua_settop(m_pLua, luaTop + 2);
    }

    // Pop the errorHandler and the initial chunk.
    lua_pop(m_pLua, 2);

    // TODO Verify that luaTop hasn't changed.
    luaTop = lua_gettop(m_pLua);

    return true;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
static int new_Point3(lua_State *L, Math::Point3 _point3);

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
enum TYPES
{
    OBJECT_REFERENCE_TYPE,
    INT_TYPE,
    STRING_TYPE,
    REAL_TYPE,
    POINT3_TYPE,
    VOID_TYPE
};

class TypeHandler
{
public:
	typedef boost::function<std::string(boost::any&)>		ConvertToStringFunction_type;
	typedef boost::function<void(lua_State*, boost::any&)>	PushArgumentFunction_type;

	TypeHandler(TYPES _type,
			ConvertToStringFunction_type _convertToStringFunction,
			PushArgumentFunction_type _pushArgumentFunction)
	:	m_type(_type)
	,	m_convertToStringFunction(_convertToStringFunction)
	,	m_pushArgumentFunction(_pushArgumentFunction)
	{

	}

	TypeHandler(const TypeHandler& _right)
	{
		clone(_right);
	}

	TypeHandler& operator=(const TypeHandler& _right)
	{
		return clone(_right);
	}

	TypeHandler& clone(const TypeHandler& _right)
	{
		m_type = _right.m_type;
		m_convertToStringFunction = _right.m_convertToStringFunction;
		m_pushArgumentFunction = _right.m_pushArgumentFunction;
		return *this;
	}

	std::string convertToString(boost::any& _value)
	{
		return m_convertToStringFunction(_value);
	}

	void pushArgument(lua_State* _pLuaState, boost::any& _value)
	{
		return m_pushArgumentFunction(_pLuaState, _value);
	}

	TYPES getType()
	{
		return m_type;
	}


private:
	TYPES 							m_type;
	ConvertToStringFunction_type 	m_convertToStringFunction;
	PushArgumentFunction_type 		m_pushArgumentFunction;

};

typedef std::map<std::string, TypeHandler*> TypeMap_type;
static TypeMap_type sm_typeMap;

static bool sm_initialized = false;

static std::string convertIntToString(boost::any& _value)
{
	std::stringstream stream;
	stream << boost::any_cast<int>(_value);
	return stream.str();
}

static void pushInt(lua_State* L, boost::any& _value)
{
    int value = boost::any_cast<int>(_value);
    lua_pushinteger(L, value);
}

static std::string convertStringToString(boost::any& _value)
{
	std::string value = boost::any_cast<std::string>(_value);
	return value;
}

static void pushString(lua_State* L, boost::any& _value)
{
    std::string value = boost::any_cast<std::string>(_value);
    lua_pushstring(L, value.c_str());
}

static std::string convertRealToString(boost::any& _value)
{
	std::stringstream stream;
	stream << boost::any_cast<Math::Real>(_value);
	return stream.str();
}

static void pushReal(lua_State* L, boost::any& _value)
{
    Math::Real value = boost::any_cast<Math::Real>(_value);
    lua_pushnumber(L, value);
}

static std::string convertPoint3ToString(boost::any& _value)
{
	std::stringstream stream;
	Math::Point3 value = boost::any_cast<Math::Point3>(_value);
	stream << value.m_x << " " << value.m_y << " " << value.m_z;
	return stream.str();
}

static void pushPoint3(lua_State* L, boost::any& _value)
{
    new_Point3(L, boost::any_cast<Math::Point3>(_value));
}

static std::string convertVoidToString(boost::any& _value)
{
	return std::string("<void>");
}

static void pushVoid(lua_State* L, boost::any& _value)
{
	lua_pushnil(L);
}

static std::string convertObjectToString(boost::any& _value)
{
	throw Zen::Utility::runtime_exception("Error, cannot convert an object to a string.");
}

static void pushObject(lua_State* L, boost::any& _value)
{
    Scripting::I_ObjectReference* pObj = boost::any_cast<Scripting::I_ObjectReference*>(_value);
    lua_rawgeti(L, LUA_REGISTRYINDEX, (uintptr_t)pObj->getScriptUserData());
}

static void initTypeHuntMaps()
{
    if (sm_initialized)
    {
    	return ;
    }

    sm_initialized = true;
    sm_typeMap[typeid(Scripting::I_ObjectReference*).name()] = new TypeHandler(OBJECT_REFERENCE_TYPE, convertObjectToString, pushObject);
    sm_typeMap[typeid(int).name()] = new TypeHandler(INT_TYPE, convertIntToString, pushInt);
    sm_typeMap[typeid(std::string).name()] = new TypeHandler(STRING_TYPE, convertStringToString, pushString);
    sm_typeMap[typeid(Math::Real).name()] = new TypeHandler(REAL_TYPE, convertRealToString, pushReal);
    sm_typeMap[typeid(Math::Point3).name()] = new TypeHandler(POINT3_TYPE, convertPoint3ToString, pushPoint3);
    sm_typeMap[typeid(void).name()] = new TypeHandler(VOID_TYPE, convertVoidToString, pushVoid);
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
LuaEngine::executeMethod(boost::any& _object, boost::any& _method, std::vector<boost::any>& _parms)
{
	initTypeHuntMaps();

    lua_State* const L = getState();

    int object;

    TypeMap_type::iterator pType = sm_typeMap.find(_object.type().name());
    if (pType != sm_typeMap.end())
    {
        switch(pType->second->getType())
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

        pType->second->pushArgument(L, *iter);
        parms++;
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

    // Initialize up to 4x4 matrix
    for(int x = 1; x < 17; x++)
    {
        if (x < top)
        {
            pMatrix4->m_array[x - 1] =  (Zen::Math::Real)lua_tonumber(L, x);
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

static int setXYZRotation_Matrix4(lua_State *L)
{
    Math::Matrix4* pMatrix4 = checkMatrix4(L, 1);
    Math::Radian xRotation(checkReal(L, 2));
    Math::Radian yRotation(checkReal(L, 3));
    Math::Radian zRotation(checkReal(L, 4));

    pMatrix4->setXYZRotation(xRotation, yRotation, zRotation);
    return 0;
}

static int setPosition_Matrix4(lua_State *L)
{
    Math::Matrix4* pMatrix4 = checkMatrix4(L, 1);
    Math::Radian x(checkReal(L, 2));
    Math::Radian y(checkReal(L, 3));
    Math::Radian z(checkReal(L, 4));

    pMatrix4->setPosition(x,y,z);
    return 0;
}

static const struct luaL_reg MatrixLib [] = {
    {"setXRotation", setXRotation_Matrix4},
    {"setYRotation", setYRotation_Matrix4},
    {"setZRotation", setZRotation_Matrix4},
    {"setXYZRotation", setXYZRotation_Matrix4},
    {"setPosition", setPosition_Matrix4},
    {NULL, NULL}
};

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

static const char* sm_pRadian = "Radian";

static Math::Radian* checkRadian(lua_State *L, int idx)
{
    lua_pushliteral(L, "__zen_userdata");
    lua_gettable(L, idx);

    Math::Radian* pRadian = (Math::Radian*)luaL_checkudata(L, -1, sm_pRadian);
    luaL_argcheck(L, pRadian != NULL, idx, "Radian expected");
    lua_pop(L, 1);

    return pRadian;
}

static int new_Radian(lua_State* L)
{
    const int top = lua_gettop(L);

    lua_newtable(L);
    const int table = lua_gettop(L);

    Math::Radian* pRadian = new (lua_newuserdata(L, sizeof(Math::Radian))) Math::Radian();
    const int userdata = lua_gettop(L);

    // T["__zen_userdata"] = UD
    lua_pushvalue(L, userdata);
    lua_setfield(L, table, "__zen_userdata");

    // Get the meta table for this class type and set it for this object
    luaL_getmetatable(L, sm_pRadian);
    lua_setmetatable(L, table);

    // Get the meta table for this class type and set it for the user data
    luaL_getmetatable(L, sm_pRadian);
    lua_setmetatable(L, userdata);

    // Pop the userdata so only the table is left
    lua_pop(L, 1);

    if( 1 <= top )
    {
        *pRadian = (Zen::Math::Real)lua_tonumber(L, 1);
    }
    else
    {
        *pRadian = 0.0f;
    }

    return 1;
}

static const struct luaL_reg RadianLib [] = {
    {NULL, NULL}
};

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

static const char* sm_pDegree = "Degree";

static Math::Degree* checkDegree(lua_State *L, int idx)
{
    lua_pushliteral(L, "__zen_userdata");
    lua_gettable(L, idx);

    Math::Degree* pDegree = (Math::Degree*)luaL_checkudata(L, -1, sm_pDegree);
    luaL_argcheck(L, pDegree != NULL, idx, "Degree expected");
    lua_pop(L, 1);

    return pDegree;
}

static int new_Degree(lua_State* L)
{
    const int top = lua_gettop(L);

    lua_newtable(L);
    const int table = lua_gettop(L);

    Math::Degree* pDegree = new (lua_newuserdata(L, sizeof(Math::Degree))) Math::Degree();
    const int userdata = lua_gettop(L);

    // T["__zen_userdata"] = UD
    lua_pushvalue(L, userdata);
    lua_setfield(L, table, "__zen_userdata");

    // Get the meta table for this class type and set it for this object
    luaL_getmetatable(L, sm_pRadian);
    lua_setmetatable(L, table);

    // Get the meta table for this class type and set it for the user data
    luaL_getmetatable(L, sm_pRadian);
    lua_setmetatable(L, userdata);

    // Pop the userdata so only the table is left
    lua_pop(L, 1);

    if( 1 <= top )
    {
        *pDegree = (Zen::Math::Real)lua_tonumber(L, 1);
    }
    else
    {
        *pDegree = 0.0f;
    }

    return 1;
}

static const struct luaL_reg DegreeLib [] = {
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
            pVector3->m_array[x - 1] = (Zen::Math::Real)lua_tonumber(L, x);
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
            pVector4->m_array[x - 1] = (Zen::Math::Real)lua_tonumber(L, x);
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
            pQuaternion4->m_array[x - 1] = (Zen::Math::Real)lua_tonumber(L, x);
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
    {sm_pRadian,        new_Radian},
    {sm_pDegree,        new_Degree},
    {sm_pQuaternion4,   new_Quaternion4},
    {NULL, NULL}
};


//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
static const char* sm_pConfig = "Config";

static Config* checkConfig(lua_State *L, int idx)
{
    lua_pushliteral(L, "__zen_userdata");
    lua_gettable(L, idx);

    Config* pConfig = (Config*)luaL_checkudata(L, -1, sm_pConfig);
    luaL_argcheck(L, pConfig != NULL, idx, "Config expected");
    lua_pop(L, 1);

    return pConfig;
}

static int new_Config (lua_State *L)
{
	initTypeHuntMaps();

	// Save the top of the stack.
	// This is the first parameter to the Utility.Config(table);
    const int firstArgument = lua_gettop(L);

    lua_newtable(L);
    const int newTable = lua_gettop(L);

    // In-place creation of a Config
    Config* pConfig = new (lua_newuserdata(L, sizeof(Config))) Config;
    const int userdata = lua_gettop(L);

    // T["__zen_userdata"] = UD
    lua_pushvalue(L, userdata);
    lua_setfield(L, newTable, "__zen_userdata");

    // Get the meta table for this class type and set it for this object
    luaL_getmetatable(L, sm_pConfig);
    lua_setmetatable(L, newTable);

    // Get the meta table for this class type and set it for the user data
    luaL_getmetatable(L, sm_pConfig);
    lua_setmetatable(L, userdata);

    // Pop the userdata so only the table is left
    lua_pop(L, 1);

    // Iterate through the first argument, which should be a table.
    // Push nil as the first key
    lua_pushnil(L);
    int x = 0;

    // Loop through the items in the first argument
    // using the top of the stack as the key.  The first
    // iteration is nil, subsequent iterations are the key.
    // lua_next pushes the next key, next value onto the stack in
    // that order.
    while (lua_next(L, firstArgument) != 0)
    {
    	x++;

    	// Get the type of the key and verify that it's a string
    	if (lua_type(L, -2) != LUA_TSTRING)
    	{
    		// If it's not a string, display an error message.
    		std::cout << "Error: argument " << x << " to Utility.Config() must be a string and not "
    				<< lua_typename(L, -2) << " for key " << lua_tostring(L, -2);

    		// Break out of the loop.
    		break;
    	}

    	// Get the key and the value for this iteration
    	const std::string key = lua_tostring(L, -2);
    	const std::string value = lua_tostring(L, -1);

    	// Save the value in the map using the key.
    	pConfig->m_config[key] = value;

    	// Pop the value off the stack.  Leave the key on the stack so that
    	// the lua_next can use it.
    	lua_pop(L, 1);
    }

    return 1;
}

static const struct luaL_reg ConfigLib [] = {
	// TODO Add getter / setter methods?
    {NULL, NULL}
};

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
static const luaL_Reg UtilityLib[] = {
    {sm_pConfig,       	new_Config},
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

    // Register the Radian meta table
    luaL_newmetatable(L, sm_pRadian);

    // metatable.__index = metatable
    lua_pushstring(L, "__index");
    lua_pushvalue(L, -2);
    lua_settable(L, -3);

    luaL_openlib(L, NULL, RadianLib, 0);

    // Pop the metatable
    lua_pop(L, 1);

    //-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

    // Register the Degree meta table
    luaL_newmetatable(L, sm_pDegree);

    // metatable.__index = metatable
    lua_pushstring(L, "__index");
    lua_pushvalue(L, -2);
    lua_settable(L, -3);

    luaL_openlib(L, NULL, DegreeLib, 0);

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

    //-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

    luaL_register(L, "Utility", UtilityLib);

    // Register the Config meta table
    luaL_newmetatable(L, sm_pConfig);

    // metatable.__index = metatable
    lua_pushstring(L, "__index");
    lua_pushvalue(L, -2);
    lua_settable(L, -3);

    luaL_openlib(L, NULL, ConfigLib, 0);

    // Pop the metatable
    lua_pop(L, 1);
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace ZLua
}   // namespace Zen
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
