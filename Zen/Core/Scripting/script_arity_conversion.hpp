//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
// Only to be included by script_method_helper

#define Q BOOST_PP_ITERATION()
typename boost::remove_const<typename boost::remove_reference<BOOST_PP_CAT(parmType, Q)>::type>::type BOOST_PP_CAT(parm, Q);
    try
    {
        script_convert_argument<typename boost::remove_reference<BOOST_PP_CAT(parmType, Q)>::type> convert;
        BOOST_PP_CAT(parm, Q) = (typename boost::remove_const<typename boost::remove_reference<BOOST_PP_CAT(parmType, Q)>::type>::type)
            convert(_parms[Q]);
    }
    catch(boost::bad_any_cast& _e)
    {
        std::stringstream errorMessage;
        errorMessage << "Bad cast to " << typeid(typename boost::remove_reference<BOOST_PP_CAT(parmType, Q)>::type).name()
            << " for parameter " << Q << " (" << _e.what() << ")";
        throw Zen::Utility::runtime_exception(errorMessage.str());
    }
#undef Q
