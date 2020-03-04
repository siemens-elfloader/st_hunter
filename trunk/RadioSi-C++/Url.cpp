
#include <swilib.h>
#include <ctype.h>
#include <netinet/in.h>
#include "Url.h"



Url::Url(const std::string & url)
{
    this->url = url;

    err = parseUrl(this->url.get(), this->host.get(), this->location.get(), &ip.get(), &port.get());

    //char s[256];
    //sprintf(s, "location: %s\nip: %d\nport: %d", location().c_str(), ip(), port());
    //ShowMSG(1, (int)s);
}


Url::Url(const Url & url, bool parse)
{
    this->url = url.url();
    this->host = url.host();
    this->location = url.location();
    this->proto = url.proto();
    this->err = url.err;

    if(parse)
        err = parseUrl(this->url.get(), this->host.get(), this->location.get(), &ip.get(), &port.get());
}


Url::~Url()
{

}


int Url::parseUrl(const std::string & url, std::string & host, std::string & location, unsigned int *ip, unsigned int *port)
{
    auto offset = [&](const std::string &s, const char *p) -> int { return p - s.c_str(); };
    auto s = url.c_str();

    if(url.compare(0, 7, "http://") == 0)
        s += 7;
    else
      return -1;

    auto host_begin = s;
    while(*s != ':' && *s != '/' && *s) ++s;
    if(*s != ':' && *s != '/' && *s)
        return -1;

    auto host_end = s;

    if( isdigit(*host_begin) ) {
        printf("Parseurl: ip detected\n");

        unsigned int _ip[4];
        sscanf(host_begin, "%d.%d.%d.%d", &_ip[0], &_ip[1], &_ip[2], &_ip[3]);
        *ip = htonl(IP_ADDR(_ip[0],_ip[1],_ip[2],_ip[3]));
        printf("ip: %d.%d.%d.%d\n", _ip[0],_ip[1],_ip[2],_ip[3]);

    } else {
        *ip = -1;
	printf("!host: %s\n", host_begin);
        host.assign(host_begin, host_end - host_begin);
    }


    if(*host_end == ':') {
	host_end++;

        auto request_location_start = url.find('/', offset(url, host_end));
        printf("getting port... %d\n", url.length() - request_location_start);

        if(request_location_start != std::string::npos) {
           location.assign(url, request_location_start, url.length() - request_location_start);
        }

        sscanf(((const char*)host_end), "%d", port);
    } else {
	if(*host_end)
	    location.assign(host_end, &*url.end() - host_end);
	else
	    location = "/";

        *port = 80;
    }

    printf("port: %d\n", *port);
    return 0;
}

