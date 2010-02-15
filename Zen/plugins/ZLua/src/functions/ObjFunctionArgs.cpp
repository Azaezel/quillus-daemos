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

#include "../LuaType.hpp"
#include "../LuaMethod.hpp"

#include <boost/preprocessor/iteration/local.hpp>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Zen {
namespace ZLua {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

#define METHOD_MAX_SIZE 100

// Create the hundred C methods

#define METHOD_LOOP(n) \
static LuaMethod* sm_pMethod##n; \
static int cfunc##n(lua_State* L) \
{ \
    return (*sm_pMethod##n)(L); \
}

#define BOOST_PP_LOCAL_MACRO(n)     METHOD_LOOP(n)
#define BOOST_PP_LOCAL_LIMITS       (0, METHOD_MAX_SIZE - 1)
#include BOOST_PP_LOCAL_ITERATE()

// Create the hundred entries in the method table

#define METHOD_TABLE_LOOP(n) cfunc##n, &sm_pMethod##n,

#define BOOST_PP_LOCAL_MACRO(n)     METHOD_TABLE_LOOP(n)
#define BOOST_PP_LOCAL_LIMITS       (0, METHOD_MAX_SIZE - 1)

static unsigned int sm_methodItem = 0;
struct
{
    lua_CFunction     m_cFunction;
    LuaMethod**  m_ppMethod;
} static sm_methodTable[METHOD_MAX_SIZE] =
{
#include BOOST_PP_LOCAL_ITERATE()
};

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
LuaType::addMethod(const std::string& _name, const std::string& _docString, Scripting::I_ScriptType::object_function_args_type _function)
{
    unsigned item = sm_methodItem++;

    (*sm_methodTable[item].m_ppMethod) = new LuaMethod(this, _name, _docString, _function, sm_methodTable[item].m_cFunction);

    m_methods.push_back(*sm_methodTable[item].m_ppMethod);
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace ZLua
}   // namespace Zen
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
