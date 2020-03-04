
#include <swilib.h>
#include "widechar.h"




widechar::widechar(int size)
{
    if(size > 0) {
        ws = AllocWS(size);
        _size = size;
    } else
        _size = 0;
}


widechar::widechar(const widechar & wc)
{
    if(wc.size() > 0) {
        ws = AllocWS(wc.size());
        wstrcpy(ws, wc.ws);
    }
}


widechar::widechar(const widechar * wc)
{
    if(wc->size() > 0) {
        ws = AllocWS(wc->length());
        wstrcpy(ws, wc->ws);
    }
}


widechar::~widechar()
{
    if(ws)
        FreeWS(ws);
}


widechar & widechar::realloc(int size)
{
    if(ws) {
        FreeWS(ws);
    }

    ws = AllocWS(size);
    return *this;
}


int widechar::length() const
{
    return ws? wstrlen(ws) : 0;
}


widechar & widechar::fromUtf8(const char *s, int len)
{
    if(len < 0)
        len = strlen(s);

    if(size() < len)
        realloc(len);

    utf8_2ws(ws, s, len);
    return *this;
}


widechar & widechar::operator =(const char *s)
{
    int len = strlen(s);

    if(len > length())
        realloc(len);

    ws_2str(ws, (char *)s, len);
    return *this;
}


widechar & widechar::operator =(const std::string &s)
{
    return operator =(s.c_str());
}


widechar & widechar::operator =(const widechar &wc)
{
    operator =(&wc);
    return *this;
}


widechar & widechar::operator =(const widechar *wc)
{
    int len = wc->length();

    if(len > length()) {
        FreeWS(ws);
        ws = AllocWS(len);
    }

    wstrncpy(ws, wc->ws, len);
    return *this;
}


int widechar::operator ==(const widechar &wc)
{
    return operator ==(&wc);
}


int widechar::operator ==(const widechar *wc)
{
    return wstrcmp(ws, wc->ws);
}












