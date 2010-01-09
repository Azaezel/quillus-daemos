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
#include "LuaMethod.hpp"
#include "LuaType.hpp"
#include "LuaModule.hpp"
#include "LuaObject.hpp"
#include "LuaTypeMap.hpp"
#include "LuaEngine.hpp"

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
,   m_functionType(0)
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
,   m_functionType(1)
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
,   m_functionType(2)
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
,   m_functionType(3)
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
,   m_functionType(4)
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
,   m_functionType(5)
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
,   m_functionType(6)
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
,   m_functionType(7)
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
,   m_functionType(8)
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
,   m_functionType(9)
,   m_function9(_function)
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

    // Pretty much all of the functions are created in LuaType::activate()
    //lua_pushcfunction(L, m_pCFunction);
    //lua_setglobal(L, m_name.c_str());

#if 0
    m_pDef = new PyMethodDef;
    memset(m_pDef, 0, sizeof(PyMethodDef));
    m_pDef->ml_name = m_name.c_str();
    m_pDef->ml_meth = m_pCFunction;

    //m_pFunction = lua_CFunction_New(m_pDef, NULL);

    PyObject* pMethod = PyDescr_NewMethod(m_pType->getRawType(), m_pDef);

    PyDict_SetItemString(m_pType->getRawType()->tp_dict, m_pDef->ml_name, pMethod);
    Py_DECREF(pMethod);
#endif
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
#if 0
PyMethodDef*
LuaMethod::getDef()
{
    return m_pDef;
}
#endif

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
        case 0: // VoidFunctionNoArgs
            m_function0(*pObj);
            return 0;
        case 1: // VoidFunctionArgs
        case 2: // ObjFunctionArgs
        case 5: // StringFunctionArgs
        case 7: // BoolFunctionArgs
        case 9: // IntFunctionArgs
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

                if (m_functionType == 1)    // VoidFunctionArgs
                {
                    // Pop everything off the stack
                    lua_pop(L, lua_gettop(L));

                    m_function1(*pObj, parms);

                    // No return value
                    return 0;
                }
                else if (m_functionType == 5) // StringFunctionArgs
                {
                    // Pop everything off the stack
                    lua_pop(L, lua_gettop(L));

                    std::string returnValue = m_function5(*pObj, parms);

                    lua_pushstring(L, returnValue.c_str());

                    return 1;
                }
                else if (m_functionType == 7)   // BoolFunctionArgs
                {
                    // Pop everything off the stack
                    lua_pop(L, lua_gettop(L));

                    bool returnValue = m_function7(*pObj, parms);

                    lua_pushboolean(L, returnValue);

                    return 1;
                }
                else if (m_functionType == 9)   // IntFunctionArgs
                {
                    // Pop everything off the stack
                    lua_pop(L, lua_gettop(L));

                    int returnValue = m_function9(*pObj, parms);

                    lua_pushinteger(L, returnValue);

                    return 1;
                }
                else // ObjFunctionArgs
                {
                    // Pop everything off the stack
                    lua_pop(L, lua_gettop(L));

                    pObjectReference_type pReturn = m_function2(*pObj, parms);

                    lua_rawgeti(L, LUA_REGISTRYINDEX, (lua_Integer)pReturn->getScriptUserData());

                    // Return 1 item on the stack, which is the object we're returning
                    assert(lua_gettop(L) == 1);
                    return 1;
                }
            }
            break;
        case 3: // ObjFunctionNoArgs
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
        case 4: // StringFunctionNoArgs
            {
                // Pop everything off the stack
                lua_pop(L, lua_gettop(L));

                std::string returnValue = m_function4(*pObj);

                lua_pushstring(L, returnValue.c_str());

                return 1;
            }
            break;
        case 6: // BoolFunctionNoArgs
            {
                // Pop everything off the stack
                lua_pop(L, lua_gettop(L));

                bool returnValue = m_function6(*pObj);

                lua_pushboolean(L, returnValue);

                return 1;
            }
            break;
        case 8: // IntFunctionNoArgs
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
    catch(Utility::runtime_exception ex)
    {
        std::cout << "ERROR: Caught exception handling call to C++ from Lua. " << ex.what() << std::endl;
        throw ex;
    }
    catch(std::exception ex)
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
