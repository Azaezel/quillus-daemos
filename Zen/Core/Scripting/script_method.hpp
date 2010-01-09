//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
// Zen Core Framework
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
#ifndef ZEN_SCRIPTING_SCRIPT_METHOD_HPP_INCLUDED
#define ZEN_SCRIPTING_SCRIPT_METHOD_HPP_INCLUDED

#include <Zen/Core/Scripting.hpp>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Zen {
namespace Scripting {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
;

template<typename ScriptableClass_type, class Method_type, typename Return_type, class DispatchHelper_type>
class script_method
:   public I_ScriptMethod
{
    /// @name I_ScriptMethod implementation
    /// @{
public:
    virtual boost::any dispatch(pObjectReference_type _pObjectReference, std::vector<boost::any>& _arguments);
    /// @}

    /// @name 'Structors
    /// @{
public:
    script_method(Method_type _pFunction, DispatchHelper_type& _signature);
    /// @}

    /// @name Member Variables
    /// @{
public:
    Method_type             m_pFunction;
    DispatchHelper_type&    m_dispatchHelper;
    /// @}

};  // class script_method

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
template<class ScriptableClass_type, class Method_type, typename Return_type, class DispatchHelper_type>
inline
script_method<ScriptableClass_type, Method_type, Return_type, DispatchHelper_type>::script_method(Method_type _pFunction, DispatchHelper_type& _signature)
:   m_pFunction(_pFunction)
,   m_dispatchHelper(_signature)
{
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
template<typename ScriptableClass_type, class Method_type, typename Return_type, class DispatchHelper_type>
boost::any
script_method<ScriptableClass_type, Method_type, Return_type, DispatchHelper_type>::dispatch(I_ObjectReference* _pObject, std::vector<boost::any>& _parms)
{
    return m_dispatchHelper.dispatch(m_pFunction, _pObject, _parms);
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace Scripting
}   // namespace Zen
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

#endif // ZEN_SCRIPTING_SCRIPT_METHOD_HPP_INCLUDED
