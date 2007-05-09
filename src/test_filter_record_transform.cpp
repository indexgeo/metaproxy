/* $Id: test_filter_record_transform.cpp,v 1.6 2007-05-09 21:23:09 adam Exp $
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
#include "filter_record_transform.hpp"
//#include "util.hpp"
//#include "sru_util.hpp"
#include "router_chain.hpp"
#include "session.hpp"
#include "package.hpp"

//#include <iostream>
//#include <stdexcept>

#define BOOST_AUTO_TEST_MAIN
#include <boost/test/auto_unit_test.hpp>

#include <iostream>


using namespace boost::unit_test;

namespace mp = metaproxy_1;
//using namespace mp::util;



BOOST_AUTO_UNIT_TEST( test_filter_record_transform_1 )
{
    try 
    {
        mp::filter::RecordTransform f_rec_trans;
    }
    catch ( ... ) {
        BOOST_CHECK (false);
    }
}

BOOST_AUTO_UNIT_TEST( test_filter_record_transform_2 )
{
    try 
    {
        mp::RouterChain router;
        
        mp::filter::RecordTransform f_rec_trans;
        
        router.append(f_rec_trans);

        //check_sru_to_z3950_init(router);
        //check_sru_to_z3950_search(router, 
        //                           "@attrset Bib-1 @attr 1=4 the", 
        //                           "@attrset Bib-1 @attr 1=4 the");

    }
    catch ( ... ) {
        BOOST_CHECK (false);
    }
}


BOOST_AUTO_UNIT_TEST( test_filter_record_transform_3 )
{
    

    try 
    {
        mp::RouterChain router;
        

        std::string xmlconf = 
            "<?xml version='1.0'?>\n"
            "<filter xmlns='http://indexdata.com/metaproxy'\n"
            "        id='rec_trans_1' type='record_transform'>\n"
            "<retrievalinfo>"
            "<retrieval" 
            " syntax=\"usmarc\""
            " name=\"marcxml\">"
            "<backend syntax=\"usmarc\""
            " name=\"marcxml\""
            "/>"
            "</retrieval>"
            "</retrievalinfo>"
            "</filter>\n"
            ;
         
        //std::cout << xmlconf  << std::endl;

        // reading and parsing XML conf
        xmlDocPtr doc = xmlParseMemory(xmlconf.c_str(), xmlconf.size());
        BOOST_CHECK(doc);
        xmlNode *root_element = xmlDocGetRootElement(doc);

        // creating and configuring filter
        mp::filter::RecordTransform f_rec_trans;
        f_rec_trans.configure(root_element);
        
        // remeber to free XML DOM
        xmlFreeDoc(doc);
        
        // add only filter to router
        router.append(f_rec_trans);

        // start testing
        //check_sru_to_z3950_init(router);
        //check_sru_to_z3950_search(router, 
        //                           "@attrset Bib-1 @attr 1=4 the", 
        //                           "@attrset Bib-1 @attr 1=4 the");

    }

    catch (std::exception &e) {
        std::cout << e.what() << "\n";
        BOOST_CHECK (false);
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
