
#ifndef __CXX_URL_H__
#define __CXX_URL_H__

#include <string>
#include "GSProperty"


class Url
{
public:

    Url() {}
    Url(const std::string & url);
    Url(const Url & url, bool parse = false);
    ~Url();

    inline int lastError() {
        return err;
    }

    int parseUrl(const std::string & url, std::string & host, std::string & location, unsigned int *ip, unsigned int *port);

public:
    GSProperty<std::string> url;
    GSProperty<std::string> host;
    GSProperty<std::string> location;
    GSProperty<std::string> proto;
    GSProperty<unsigned int> port;
    GSProperty<unsigned int> ip;


private:
    int err;

};





#endif
