/* $Id: util.hpp,v 1.2 2005-10-30 17:13:36 adam Exp $
   Copyright (c) 2005, Index Data.

%LICENSE%
 */

#ifndef YP2_UTIL_HPP
#define YP2_UTIL_HPP

#include <yaz/z-core.h>
#include <string>

#include <boost/utility.hpp>

namespace yp2 {
    struct util  {
	static bool pqf(ODR odr, Z_APDU *apdu, const std::string &q);
    };

    class odr : public boost::noncopyable {
    public:
        odr(int type);
        odr();
        ~odr();
        operator ODR() const;
    private:
        ODR m_odr;
    };
        
}
#endif
/*
 * Local variables:
 * c-basic-offset: 4
 * indent-tabs-mode: nil
 * c-file-style: "stroustrup"
 * End:
 * vim: shiftwidth=4 tabstop=8 expandtab
 */
