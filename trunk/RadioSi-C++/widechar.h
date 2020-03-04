
#ifndef __CXX_WIDE_CHAR_H__
#define __CXX_WIDE_CHAR_H__

#include <string>
#include <swilib.h>


class widechar
{
public:

    widechar(int size = 128);
    widechar(const widechar &);
    widechar(const widechar *);

    ~widechar();

    widechar & fromUtf8(const char *, int len = -1);

    widechar & operator =(const char *);
    widechar & operator =(const std::string &);
    widechar & operator =(const widechar &);
    widechar & operator =(const widechar *);
    int operator ==(const widechar &);
    int operator ==(const widechar *);
    operator WSHDR *() const {
        return ws;
    }

    int length() const;
    widechar & realloc(int size);
    inline int size() const {
        return _size;
    }

private:
    WSHDR *ws;
    int _size;
};



#endif
