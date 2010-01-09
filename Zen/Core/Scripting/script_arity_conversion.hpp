//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
// Only to be included by script_method_helper

#define Q BOOST_PP_ITERATION()

BOOST_PP_CAT(parmType, Q)
    BOOST_PP_CAT(parm, Q) = 
    (BOOST_PP_CAT(parmType, Q))
    boost::any_cast<script_convert_argument<BOOST_PP_CAT(parmType, Q)>::type>
    (_parms[Q]);

#undef Q
