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
#include "LuaMethod.hpp"
#include "LuaType.hpp"
#include "LuaModule.hpp"
#include "LuaObject.hpp"
#include "LuaTypeMap.hpp"
#include "LuaEngine.hpp"
#include "Config.hpp"

#include <Zen/Core/Math/Matrix4.hpp>
#include <Zen/Core/Math/Quaternion4.hpp>
#include <Zen/Core/Math/Vector4.hpp>
#include <Zen/Core/Math/Vector3.hpp>
#include <Zen/Core/Math/Point3.hpp>

#include <Zen/Core/Scripting/I_ScriptObject.hpp>
#include <Zen/Core/Scripting/I_ObjectReference.hpp>

#include <Zen/Core/Utility/runtime_exception.hpp>

#include <iostream>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Zen {
namespace ZLua {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
LuaMethod::LuaMethod(LuaType* _pType, const std::string& _name, const std::string& _docString, Scripting::I_ScriptType::void_function_no_args_type _function, lua_CFunction _pCFunction)
:   m_pType(_pType)
,   m_name(_name)
,   m_docString(_docString)
,   m_pCFunction(_pCFunction)
,   m_functionType(VOID_FUNCTION_NO_ARGS)
,   m_function0(_function)
{
    init();
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
LuaMethod::LuaMethod(LuaType* _pType, const std::string& _name, const std::string& _docString, Scripting::I_ScriptType::void_function_args_type _function, lua_CFunction _pCFunction)
:   m_pType(_pType)
,   m_name(_name)
,   m_docString(_docString)
,   m_pCFunction(_pCFunction)
,   m_functionType(VOID_FUNCTION_ARGS)
,   m_function1(_function)
{
    init();
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
LuaMethod::LuaMethod(LuaType* _pType, const std::string& _name, const std::string& _docString, Scripting::I_ScriptType::object_function_args_type _function, lua_CFunction _pCFunction)
:   m_pType(_pType)
,   m_name(_name)
,   m_docString(_docString)
,   m_pCFunction(_pCFunction)
,   m_functionType(OBJ_FUNCTION_ARGS)
,   m_function2(_function)
{
    init();
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
LuaMethod::LuaMethod(LuaType* _pType, const std::string& _name, const std::string& _docString, Scripting::I_ScriptType::object_function_no_args_type _function, lua_CFunction _pCFunction)
:   m_pType(_pType)
,   m_name(_name)
,   m_docString(_docString)
,   m_pCFunction(_pCFunction)
,   m_functionType(OBJ_FUNCTION_NO_ARGS)
,   m_function3(_function)
{
    init();
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
LuaMethod::LuaMethod(LuaType* _pType, const std::string& _name, const std::string& _docString, Scripting::I_ScriptType::string_function_no_args_type _function, lua_CFunction _pCFunction)
:   m_pType(_pType)
,   m_name(_name)
,   m_docString(_docString)
,   m_pCFunction(_pCFunction)
,   m_functionType(STRING_FUNCTION_NO_ARGS)
,   m_function4(_function)
{
    init();
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
LuaMethod::LuaMethod(LuaType* _pType, const std::string& _name, const std::string& _docString, Scripting::I_ScriptType::string_function_args_type _function, lua_CFunction _pCFunction)
:   m_pType(_pType)
,   m_name(_name)
,   m_docString(_docString)
,   m_pCFunction(_pCFunction)
,   m_functionType(STRING_FUNCTION_ARGS)
,   m_function5(_function)
{
    init();
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
LuaMethod::LuaMethod(LuaType* _pType, const std::string& _name, const std::string& _docString, Scripting::I_ScriptType::bool_function_no_args_type _function, lua_CFunction _pCFunction)
:   m_pType(_pType)
,   m_name(_name)
,   m_docString(_docString)
,   m_pCFunction(_pCFunction)
,   m_functionType(BOOL_FUNCTION_NO_ARGS)
,   m_function6(_function)
{
    init();
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
LuaMethod::LuaMethod(LuaType* _pType, const std::string& _name, const std::string& _docString, Scripting::I_ScriptType::bool_function_args_type _function, lua_CFunction _pCFunction)
:   m_pType(_pType)
,   m_name(_name)
,   m_docString(_docString)
,   m_pCFunction(_pCFunction)
,   m_functionType(BOOL_FUNCTION_ARGS)
,   m_function7(_function)
{
    init();
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
LuaMethod::LuaMethod(LuaType* _pType, const std::string& _name, const std::string& _docString, Scripting::I_ScriptType::int_function_no_args_type _function, lua_CFunction _pCFunction)
:   m_pType(_pType)
,   m_name(_name)
,   m_docString(_docString)
,   m_pCFunction(_pCFunction)
,   m_functionType(INT_FUNCTION_NO_ARGS)
,   m_function8(_function)
{
    init();
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
LuaMethod::LuaMethod(LuaType* _pType, const std::string& _name, const std::string& _docString, Scripting::I_ScriptType::int_function_args_type _function, lua_CFunction _pCFunction)
:   m_pType(_pType)
,   m_name(_name)
,   m_docString(_docString)
,   m_pCFunction(_pCFunction)
,   m_functionType(INT_FUNCTION_ARGS)
,   m_function9(_function)
{
    init();
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
LuaMethod::LuaMethod(LuaType* _pType, const std::string& _name, const std::string& _docString, Scripting::I_ScriptMethod* _function, lua_CFunction _pCFunction)
:   m_pType(_pType)
,   m_name(_name)
,   m_docString(_docString)
,   m_pCFunction(_pCFunction)
,   m_functionType(GENERIC_FUNCTION_ARGS)
,   m_function10(_function)
{
    init();
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
LuaMethod::~LuaMethod()
{
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
LuaMethod::init()
{
    lua_State* const L = m_pType->getModule().getEngine().getState();

    // All of the functions are created in LuaType::activate()
    //lua_pushcfunction(L, m_pCFunction);
    //lua_setglobal(L, m_name.c_str());
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
int
LuaMethod::operator()(lua_State* L)
{
    try
    {

        // Get the user data from the first argument as it's the I_ObjectReference*
        lua_pushliteral(L, "__zen_userdata");
        lua_gettable(L, 1);
        pObjectReference_type* pObj = (pObjectReference_type*)lua_touserdata(L, -1);
        lua_pop(L, 1);

        switch(m_functionType)
        {
        case VOID_FUNCTION_NO_ARGS:
            m_function0(*pObj);
            return 0;
        case VOID_FUNCTION_ARGS:
        case OBJ_FUNCTION_ARGS:
        case STRING_FUNCTION_ARGS:
        case BOOL_FUNCTION_ARGS:
        case INT_FUNCTION_ARGS:
        case GENERIC_FUNCTION_ARGS:
            {
                int top = lua_gettop(L);

                std::vector<boost::any> parms;
                for(int x = 2; x <= lua_gettop(L); x++)
                {
                    switch(lua_type(L, x))
                    {
                    case LUA_TBOOLEAN:
                        {
                            bool value = lua_toboolean(L, x);
                            parms.push_back(value);
                        }
                        break;
                    case LUA_TNUMBER:
                        {
                            Math::Real value = (Math::Real)lua_tonumber(L, x);
                            parms.push_back(value);
                        }
                        break;
                    case LUA_TSTRING:
                        {
                            std::string value = lua_tostring(L, x);
                            parms.push_back(value);
                        }
                        break;
                    case LUA_TUSERDATA:
                        {
                            // Should we get here anymore?
                            pObjectReference_type* pValue = (pObjectReference_type*)lua_touserdata(L, x);
                            parms.push_back(*pValue);
                        }
                        break;
                    case LUA_TTABLE:        // A script object
                        {
                            const int tmpTop = lua_gettop(L);

                            //lua_topointer
                            lua_pushstring(L, "__zen_userdata");
                            lua_gettable(L, x);
                            int newType = lua_type(L, -1);

                            // Check to see if this is a user data
                            if (newType == LUA_TUSERDATA)
                            {
                                // Check what type of user data
                                lua_getmetatable(L, x);
                                lua_getfield(L, LUA_REGISTRYINDEX, "Matrix4");
                                if (lua_rawequal(L, -1, -2))
                                {
                                    Math::Matrix4* pMatrix4 = (Math::Matrix4*)lua_touserdata(L, -3);

                                    // Note that we're pushing a pointer, not a copy of the object
                                    parms.push_back(pMatrix4);

                                    // Pop the table and the two meta tables
                                    lua_pop(L, 3);
                                    assert(tmpTop == lua_gettop(L));
                                    continue;
                                }

                                lua_pop(L, 1);
                                lua_getfield(L, LUA_REGISTRYINDEX, "Quaternion4");
                                if (lua_rawequal(L, -1, -2))
                                {
                                    Math::Quaternion4* pQuaternion4 = (Math::Quaternion4*)lua_touserdata(L, -3);

                                    // Note that we're pushing a pointer, not a copy of the object
                                    parms.push_back(pQuaternion4);

                                    // Pop the table and the two meta tables
                                    lua_pop(L, 3);
                                    assert(tmpTop == lua_gettop(L));
                                    continue;
                                }

                                lua_pop(L, 1);
                                lua_getfield(L, LUA_REGISTRYINDEX, "Vector3");
                                if (lua_rawequal(L, -1, -2))
                                {
                                    Math::Vector3* pVector3 = (Math::Vector3*)lua_touserdata(L, -3);

                                    // Note that we're pushing a pointer, not a copy of the object
                                    parms.push_back(pVector3);

                                    // Pop the table and the two meta tables
                                    lua_pop(L, 3);
                                    assert(tmpTop == lua_gettop(L));
                                    continue;
                                }

                                lua_pop(L, 1);
                                lua_getfield(L, LUA_REGISTRYINDEX, "Vector4");
                                if (lua_rawequal(L, -1, -2))
                                {
                                    Math::Vector4* pVector4 = (Math::Vector4*)lua_touserdata(L, -3);

                                    // Note that we're pushing a pointer, not a copy of the object
                                    parms.push_back(pVector4);

                                    // Pop the table and the two meta tables
                                    lua_pop(L, 3);
                                    assert(tmpTop == lua_gettop(L));
                                    continue;
                                }

                                lua_pop(L, 1);
                                lua_getfield(L, LUA_REGISTRYINDEX, "Point3");
                                if (lua_rawequal(L, -1, -2))
                                {
                                    Math::Point3* pPoint3 = (Math::Point3*)lua_touserdata(L, -3);

                                    // Note that we're pushing a pointer, not a copy of the object
                                    parms.push_back(pPoint3);

                                    // Pop the table and the two meta tables
                                    lua_pop(L, 3);
                                    assert(tmpTop == lua_gettop(L));
                                    continue;
                                }

                                // Check to see if the argument is a Config type.
                                lua_pop(L, 1);
                                lua_getfield(L, LUA_REGISTRYINDEX, "Config");
                                if (lua_rawequal(L, -1, -2))
                                {
                                	// Get the user data, which is a Config*
                                    Config* pConfig = (Config*)lua_touserdata(L, -3);

                                    // Push the value onto the list of arguments being passed
                                    // to the function being called by Lua.
                                    // Note that we're pushing a <b>pointer</b>, of the
                                    // std::map<std::string, std::string> and not the value.
                                    parms.push_back(&pConfig->m_config);

                                    // Pop the table and the two meta tables
                                    lua_pop(L, 3);
                                    assert(tmpTop == lua_gettop(L));
                                    continue;
                                }

                                pObjectReference_type* pValue = (pObjectReference_type*)lua_touserdata(L, -3);
                                parms.push_back(*pValue);
                                lua_pop(L, 2);
                            }
                            else
                            {
                                // Push a non C++ script object
                                lua_pushvalue(L, x);

                                int ref = luaL_ref(L, LUA_REGISTRYINDEX);
                                parms.push_back(ref);
                            }

                            lua_pop(L, 1);

                            assert(tmpTop == lua_gettop(L));
                        }
                        break;
                    case LUA_TFUNCTION:     // As script function (not C++)
                        {
                            lua_pushvalue(L, x);

                            int ref = luaL_ref(L, LUA_REGISTRYINDEX);
                            parms.push_back(ref);
                        }
                        break;
                    }
                }

                if (m_functionType == VOID_FUNCTION_ARGS)
                {
                    // Pop everything off the stack
                    lua_pop(L, lua_gettop(L));

                    m_function1(*pObj, parms);

                    // No return value
                    return 0;
                }
                else if (m_functionType == STRING_FUNCTION_ARGS)
                {
                    // Pop everything off the stack
                    lua_pop(L, lua_gettop(L));

                    std::string returnValue = m_function5(*pObj, parms);

                    lua_pushstring(L, returnValue.c_str());

                    return 1;
                }
                else if (m_functionType == BOOL_FUNCTION_ARGS)
                {
                    // Pop everything off the stack
                    lua_pop(L, lua_gettop(L));

                    bool returnValue = m_function7(*pObj, parms);

                    lua_pushboolean(L, returnValue);

                    return 1;
                }
                else if (m_functionType == INT_FUNCTION_ARGS)
                {
                    // Pop everything off the stack
                    lua_pop(L, lua_gettop(L));

                    int returnValue = m_function9(*pObj, parms);

                    lua_pushinteger(L, returnValue);

                    return 1;
                }
                else if (m_functionType == OBJ_FUNCTION_ARGS)
                {
                    // Pop everything off the stack
                    lua_pop(L, lua_gettop(L));

                    pObjectReference_type pReturn = m_function2(*pObj, parms);

                    lua_rawgeti(L, LUA_REGISTRYINDEX, (lua_Integer)pReturn->getScriptUserData());

                    // Return 1 item on the stack, which is the object we're returning
                    assert(lua_gettop(L) == 1);
                    return 1;
                }
                else if (m_functionType == GENERIC_FUNCTION_ARGS)
                {
                    // Pop everything off the stack
                    lua_pop(L, lua_gettop(L));

                    boost::any anyReturn = m_function10->dispatch(*pObj, parms);

                    // TODO this is inefficient... do a map of functors instead.
                    if (anyReturn.type() == typeid(void))
                    {
                        // no return, don't bother doing anything
                        return 0;
                    }
                    else if (anyReturn.type() == typeid(Zen::Scripting::I_ObjectReference*))
                    {
                        // object
                        pObjectReference_type pReturn = boost::any_cast<pObjectReference_type>(anyReturn);

                        lua_rawgeti(L, LUA_REGISTRYINDEX, (lua_Integer)pReturn->getScriptUserData());

                        assert(lua_gettop(L) == 1);
                        return 1;
                    }
                    else if (anyReturn.type() == typeid(std::string))
                    {
                        std::string returnValue = boost::any_cast<std::string>(anyReturn);

                        lua_pushstring(L, returnValue.c_str());

                        return 1;
                    }
                    else if(anyReturn.type() == typeid(bool))
                    {
                        bool returnValue = boost::any_cast<bool>(anyReturn);

                        lua_pushboolean(L, returnValue);

                        return 1;
                    }
                    else if (anyReturn.type() == typeid(int))
                    {
                        int returnValue = boost::any_cast<int>(anyReturn);

                        lua_pushinteger(L, returnValue);

                        return 1;
                    }
                    else if (anyReturn.type() == typeid(Zen::Math::Real))
                    {
                        Zen::Math::Real returnValue = boost::any_cast<Zen::Math::Real>(anyReturn);

                        lua_pushnumber(L, returnValue);

                        return 1;
                    }
                    else
                    {
                        // TODO Make this error message a little more detailed
                        throw Zen::Utility::runtime_exception("Script method returned unknown type.");
                    }

                    // TODO Throw an exception since the type wasn't 
                    assert(lua_gettop(L) == 0);
                    return 0;
                }
            }
            break;
        case OBJ_FUNCTION_NO_ARGS:
            {
                // Pop everything off the stack
                lua_pop(L, lua_gettop(L));

                pObjectReference_type pReturn = m_function3(*pObj);

                lua_rawgeti(L, LUA_REGISTRYINDEX, (lua_Integer)pReturn->getScriptUserData());

                // Return 1 item on the stack, which is the object we're returning
                assert(lua_gettop(L) == 1);
                return 1;

            }
            break;
        case STRING_FUNCTION_NO_ARGS:
            {
                // Pop everything off the stack
                lua_pop(L, lua_gettop(L));

                std::string returnValue = m_function4(*pObj);

                lua_pushstring(L, returnValue.c_str());

                return 1;
            }
            break;
        case BOOL_FUNCTION_NO_ARGS:
            {
                // Pop everything off the stack
                lua_pop(L, lua_gettop(L));

                bool returnValue = m_function6(*pObj);

                lua_pushboolean(L, returnValue);

                return 1;
            }
            break;
        case INT_FUNCTION_NO_ARGS:
            {
                // Pop everything off the stack
                lua_pop(L, lua_gettop(L));

                int returnValue = m_function8(*pObj);

                lua_pushinteger(L, returnValue);

                return 1;
            }
            break;
        }

    }
    catch(Utility::runtime_exception& ex)
    {
        std::cout << "ERROR: Caught exception handling call to C++ from Lua. " << ex.what() << std::endl;
        throw ex;
    }
    catch(std::exception& ex)
    {
        std::cout << "ERROR: Caught exception handling call to C++ from Lua. " << ex.what() << std::endl;
        throw ex;
    }
    catch(...)
    {
        std::cout << "ERROR: Caught unknown exception handling call to C++ from Lua." << std::endl;
        throw;
    }

    // TODO Return the number of items on the stack.
    return 0;

}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace ZLua
}   // namespace Zen
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
