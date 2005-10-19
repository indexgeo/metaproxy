/* $Id: test_filter_z3950_client.cpp,v 1.2 2005-10-16 16:09:58 adam Exp $
   Copyright (c) 2005, Index Data.

%LICENSE%
 */

#include "config.hpp"
#include <iostream>
#include <stdexcept>

#include "filter_z3950_client.hpp"

#include "router.hpp"
#include "session.hpp"
#include "package.hpp"

#define BOOST_AUTO_TEST_MAIN
#include <boost/test/auto_unit_test.hpp>

#include <yaz/zgdu.h>
#include <yaz/otherinfo.h>
using namespace boost::unit_test;


BOOST_AUTO_TEST_CASE( test_filter_z3950_client_1 )
{
    try 
    {
        {
            yp2::filter::Z3950Client zc;
        }
    }
    catch ( ... ) {
        BOOST_CHECK (false);
    }
}

BOOST_AUTO_TEST_CASE( test_filter_z3950_client_2 )
{
    try 
    {
        yp2::RouterChain router;
        
        yp2::filter::Z3950Client zc;
        
        router.rule(zc);
        
        // Create package with Z39.50 init request in it
        yp2::Package pack;
        
        ODR odr = odr_createmem(ODR_ENCODE);
        Z_APDU *apdu = zget_APDU(odr, Z_APDU_initRequest);
        
        BOOST_CHECK(apdu);
        
        pack.request() = apdu;
        odr_destroy(odr);
        
        // Put it in router
        pack.router(router).move(); 
        
        // Inspect that we got Z39.50 init Response - a Z39.50 session
        // specify a virtual host
        yazpp_1::GDU *gdu = &pack.response();
        
        BOOST_CHECK(pack.session().is_closed()); 
        
        Z_GDU *z_gdu = gdu->get();
        BOOST_CHECK(z_gdu);
        if (z_gdu) {
            BOOST_CHECK_EQUAL(z_gdu->which, Z_GDU_Z3950);
            BOOST_CHECK_EQUAL(z_gdu->u.z3950->which, Z_APDU_initResponse);
        }
    }
    catch ( ... ) {
        BOOST_CHECK (false);
    }
}

BOOST_AUTO_TEST_CASE( test_filter_z3950_client_3 )
{
    try 
    {
        yp2::RouterChain router;
        
        yp2::filter::Z3950Client zc;

        router.rule(zc);
        
        // Create package with Z39.50 present request in it
        yp2::Package pack;
        
        ODR odr = odr_createmem(ODR_ENCODE);
        Z_APDU *apdu = zget_APDU(odr, Z_APDU_presentRequest);
        
        BOOST_CHECK(apdu);
        
        pack.request() = apdu;
        odr_destroy(odr);
        
        // Put it in router
        pack.router(router).move(); 
        
        // Inspect that we got Z39.50 close - a Z39.50 session must start
        // with an init !
        yazpp_1::GDU *gdu = &pack.response();
        
        BOOST_CHECK(pack.session().is_closed()); 
        
        Z_GDU *z_gdu = gdu->get();
        BOOST_CHECK(z_gdu);
        if (z_gdu) {
            BOOST_CHECK_EQUAL(z_gdu->which, Z_GDU_Z3950);
            BOOST_CHECK_EQUAL(z_gdu->u.z3950->which, Z_APDU_close);
        }
    }
    catch ( ... ) {
        BOOST_CHECK (false);
    }
}

BOOST_AUTO_TEST_CASE( test_filter_z3950_client_4 )
{
    try 
    {
        yp2::RouterChain router;
        
        yp2::filter::Z3950Client zc;
        
        router.rule(zc);
        
        // Create package with Z39.50 init request in it
        yp2::Package pack;
        
        ODR odr = odr_createmem(ODR_ENCODE);
        Z_APDU *apdu = zget_APDU(odr, Z_APDU_initRequest);
        
        const char *vhost = "localhost:9999";
        if (vhost)
            yaz_oi_set_string_oidval(&apdu->u.initRequest->otherInfo,
                                     odr, VAL_PROXY, 1, vhost);
        BOOST_CHECK(apdu);
        
        pack.request() = apdu;
        odr_destroy(odr);
        
        // Put it in router
        pack.router(router).move(); 
        
        if (pack.session().is_closed())
        {
            // OK, server was not up!
        }
        else
        {
            // Inspect that we got Z39.50 init response
            yazpp_1::GDU *gdu = &pack.response();
            Z_GDU *z_gdu = gdu->get();
            BOOST_CHECK(z_gdu);
            if (z_gdu) {
                BOOST_CHECK_EQUAL(z_gdu->which, Z_GDU_Z3950);
                BOOST_CHECK_EQUAL(z_gdu->u.z3950->which, Z_APDU_initResponse);
            }
        }
    }
    catch ( ... ) {
        BOOST_CHECK (false);
    }
}


/*
 * Local variables:
 * c-basic-offset: 4
 * indent-tabs-mode: nil
 * c-file-style: "stroustrup"
 * End:
 * vim: shiftwidth=4 tabstop=8 expandtab
 */