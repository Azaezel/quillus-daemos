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
#if !defined(BOOST_PP_IS_ITERATING)

#ifndef ZEN_SCRIPTING_SCRIPT_METHOD_HELPER_HPP_INCLUDED
#define ZEN_SCRIPTING_SCRIPT_METHOD_HELPER_HPP_INCLUDED

#include <Zen/Core/Scripting/script_convert_argument.hpp>

#include <boost/preprocessor/repetition/enum_trailing_params.hpp>
#include <boost/preprocessor/repetition/enum_trailing_binary_params.hpp>
#include <boost/preprocessor/repetition/enum_params.hpp>
#include <boost/preprocessor/repetition/repeat.hpp>
#include <boost/preprocessor/facilities/expand.hpp>
#include <boost/preprocessor/arithmetic/add.hpp>
#include <boost/preprocessor/control/if.hpp>

#include <boost/preprocessor/iteration/local.hpp>
#include <boost/preprocessor/iteration/iterate.hpp>

#include <boost/preprocessor/iterate.hpp>
#include <boost/preprocessor/cat.hpp>

#include <boost/any.hpp>


//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Zen {
namespace Scripting {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~



//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace detail {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

template<typename ScriptableClass_type, typename pScriptObject_type>
struct getRawObject
{
    inline
    ScriptableClass_type*
    operator()(typename ScriptableClass_type::ScriptObjectReference_type* _pObject)
    {
        return _pObject->getRawObject();
    }
};

template<typename ScriptableClass_type>
struct getRawObject<ScriptableClass_type, ScriptableClass_type*>
{
    inline
    ScriptableClass_type*
    operator()(typename ScriptableClass_type::ScriptObjectReference_type* _pObject)
    {
        return _pObject->getObject();
    }
};

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
} // namespace detail
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

template<typename Method_type, typename Return_type, class ScriptableClass_type>
class script_dispatch_helper
{
public:
    typedef typename ScriptableClass_type::ScriptObjectReference_type   ScriptObjectReference_type;
    typedef typename ScriptObjectReference_type::object_ptr_type        object_ptr_type;

    typedef Return_type                                        MethodReturn_type;

    ScriptableClass_type*
    getRawObject(typename ScriptableClass_type::ScriptObjectReference_type* _pObject)
    {
        detail::getRawObject<ScriptableClass_type, object_ptr_type> getRawObject;

        return getRawObject(_pObject);
    }

    virtual
    boost::any dispatch(Method_type _function, I_ObjectReference* _pObject, std::vector<boost::any> _parms) = 0;
};

#define BOOST_PP_ITERATION_PARAMS_1 \
    (3, (0, ZEN_SCRIPTING_MAX_SCRIPT_PARMS, <Zen/Core/Scripting/script_dispatch_helper.hpp>))

#include BOOST_PP_ITERATE()

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace Scripting
}   // namespace Zen
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

#endif // ZEN_SCRIPTING_SCRIPT_METHOD_HELPER_HPP_INCLUDED


#else // BOOST_PP_IS_ITERATING

#define N BOOST_PP_ITERATION()
#define M BOOST_PP_SUB(N, 1)

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
template<typename Method_type, typename Return_type, class ScriptableClass_type BOOST_PP_ENUM_TRAILING_PARAMS_Z(1, N, class parmType)>
class BOOST_PP_CAT(derived_dispatch_helper, N)
:   public script_dispatch_helper<Method_type, Return_type, ScriptableClass_type>
{
public:

    virtual
    boost::any dispatch(Method_type _function, I_ObjectReference* _pObject, std::vector<boost::any> _parms)
    {
        typename ScriptableClass_type::ScriptObjectReference_type* pObject = dynamic_cast<typename ScriptableClass_type::ScriptObjectReference_type*>(_pObject);

#define BOOST_PP_ITERATION_PARAMS_2 \
    (3, (0, M, <Zen/Core/Scripting/script_arity_conversion.hpp>))

// Only expand if M != 0
#if BOOST_PP_IF(N, 1, 0)
#include BOOST_PP_ITERATE()
#endif

        return // need this? (script_convert_argument<typename Return_type>::type)
            (getRawObject(pObject)->*_function)(BOOST_PP_ENUM_PARAMS_Z(1, N, parm));
    }
};

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

// Specialization for Return_type = void
template<typename Method_type, class ScriptableClass_type BOOST_PP_ENUM_TRAILING_PARAMS_Z(1, N, class parmType)>
class BOOST_PP_CAT(derived_dispatch_helper, N)<Method_type, void, ScriptableClass_type BOOST_PP_ENUM_TRAILING_PARAMS_Z(1, N, parmType)>
:   public script_dispatch_helper<Method_type, void, ScriptableClass_type>
{
public:

    virtual
    boost::any dispatch(Method_type _function, I_ObjectReference* _pObject, std::vector<boost::any> _parms)
    {
        typename ScriptableClass_type::ScriptObjectReference_type* pObject = dynamic_cast<typename ScriptableClass_type::ScriptObjectReference_type*>(_pObject);

#define BOOST_PP_ITERATION_PARAMS_2 \
    (3, (0, M, <Zen/Core/Scripting/script_arity_conversion.hpp>))

// Only expand if M != 0
#if BOOST_PP_IF(N, 1, 0)
#include BOOST_PP_ITERATE()
#endif

        boost::any returnValue;
        (getRawObject(pObject)->*_function)(BOOST_PP_ENUM_PARAMS_Z(1, N, parm));
        return returnValue;
    }
};

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

template<typename Method_type, typename Return_type, class ScriptableClass_type BOOST_PP_ENUM_TRAILING_PARAMS_Z(1, N, class parmType)>
script_dispatch_helper<Method_type, Return_type, ScriptableClass_type>&
get_dispatch_helper(Method_type _function, Return_type(ScriptableClass_type::*_method)(BOOST_PP_ENUM_PARAMS_Z(1, N, parmType)))
{
    static BOOST_PP_CAT(derived_dispatch_helper, N) <Method_type, Return_type, ScriptableClass_type BOOST_PP_ENUM_TRAILING_PARAMS_Z(1, N, parmType)> tmp;
    return tmp;
}

#undef N
#undef M

#endif // BOOST_PP_IS_ITERATING
