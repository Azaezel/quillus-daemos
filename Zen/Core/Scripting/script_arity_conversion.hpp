//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
// Only to be included by script_method_helper

#define Q BOOST_PP_ITERATION()
typename boost::remove_reference<BOOST_PP_CAT(parmType, Q)>::type
    BOOST_PP_CAT(parm, Q) = (typename boost::remove_reference<BOOST_PP_CAT(parmType, Q)>::type)
        boost::any_cast<typename script_convert_argument<typename boost::remove_reference<BOOST_PP_CAT(parmType, Q)>::type>::type>(_parms[Q]);
#undef Q
