/* $Id: test_filter_backend_test.cpp,v 1.11 2007-05-09 21:23:09 adam Exp $
   Copyright (c) 2005-2007, Index Data.

This file is part of Metaproxy.

Metaproxy is free software; you can redistribute it and/or modify it under
the terms of the GNU General Public License as published by the Free
Software Foundation; either version 2, or (at your option) any later
version.

Metaproxy is distributed in the hope that it will be useful, but WITHOUT ANY
WARRANTY; without even the implied warranty of MERCHANTABILITY or
FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
for more details.

You should have received a copy of the GNU General Public License
along with Metaproxy; see the file LICENSE.  If not, write to the
Free Software Foundation, 59 Temple Place - Suite 330, Boston, MA
02111-1307, USA.
 */

#include "config.hpp"
#include <iostream>
#include <stdexcept>

#include "util.hpp"
#include "filter_backend_test.hpp"
#include "filter_log.hpp"

#include "router_chain.hpp"
#include "session.hpp"
#include "package.hpp"

#include <yaz/zgdu.h>
#include <yaz/pquery.h>
#include <yaz/otherinfo.h>

#define BOOST_AUTO_TEST_MAIN
#include <boost/test/auto_unit_test.hpp>
using namespace boost::unit_test;

namespace mp = metaproxy_1;

BOOST_AUTO_UNIT_TEST( test_filter_backend_test_1 )
{
    try 
    {
        mp::filter::BackendTest btest;
    }
    catch ( ... ) {
        BOOST_CHECK (false);
    }
}

BOOST_AUTO_UNIT_TEST( test_filter_backend_test_2 )
{
    try 
    {
        mp::RouterChain router;
        
        mp::filter::BackendTest btest;
        router.append(btest);
        
        mp::Package pack;
        
        mp::odr odr;
        Z_APDU *apdu = zget_APDU(odr, Z_APDU_initRequest);
        
        BOOST_CHECK(apdu);
        
        pack.request() = apdu;
        
        // Put it in router
        pack.router(router).move(); 
        
        // Inspect that we got Z39.50 init Response OK.
        yazpp_1::GDU *gdu = &pack.response();
        
        BOOST_CHECK(!pack.session().is_closed()); 
        
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

BOOST_AUTO_UNIT_TEST( test_filter_backend_test_3 )
{
    try 
    {
        mp::RouterChain router;
        
        mp::filter::BackendTest btest;
        router.append(btest);
        
        mp::Package pack;
        
        // send search request as first request.. That should fail with
        // a close from the backend
        mp::odr odr;
        Z_APDU *apdu = zget_APDU(odr, Z_APDU_searchRequest);

        mp::util::pqf(odr, apdu, "computer");
        
        apdu->u.searchRequest->num_databaseNames = 1;
        apdu->u.searchRequest->databaseNames = (char**)
            odr_malloc(odr, sizeof(char *));
        apdu->u.searchRequest->databaseNames[0] = odr_strdup(odr, "Default");
        
        BOOST_CHECK(apdu);
        
        pack.request() = apdu;
        
        // Put it in router
        pack.router(router).move(); 
        
        // Inspect that we got Z39.50 close
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

BOOST_AUTO_UNIT_TEST( test_filter_backend_test_4 )
{
    try 
    {
        mp::RouterChain router;
        
        mp::filter::BackendTest btest;
        router.append(btest);
        
        mp::Package pack;
        
        // send present request as first request.. That should fail with
        // a close from the backend
        mp::odr odr;
        Z_APDU *apdu = zget_APDU(odr, Z_APDU_presentRequest);

        BOOST_CHECK(apdu);
        
        pack.request() = apdu;
        
        // Put it in router
        pack.router(router).move(); 
        
        // Inspect that we got Z39.50 close
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


/*
 * Local variables:
 * c-basic-offset: 4
 * indent-tabs-mode: nil
 * c-file-style: "stroustrup"
 * End:
 * vim: shiftwidth=4 tabstop=8 expandtab
 */
