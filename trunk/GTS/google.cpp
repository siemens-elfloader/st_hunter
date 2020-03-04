
#include <swilib.h>
#include "socket.h"
#include "google.h"
#include "FormBuilder/EForm.h"
#include "lang.h"
#include "main.h"
#include "LangPack.h"
#include "lua_support.h"
#include <string>


extern int ansi2utf8(const char *, char*);
extern LangPack *lgp;
extern EForm *edit;

GBSTMR reconnect_timer;
Google *gl;
char buffer[2*1024+12];
int parse(char *buf);
extern "C" int input_traf, output_traf;
// ================== сокет и парсинг ===================

void reconnetc(GBSTMR *d)
{
    gl->connect();
}


void Google::log(const char *msg, ...)
{
    /*
    char buf[128];
    va_list va;
    va_start(va, msg);

    vsprintf(buf, msg, va);
    va_end(va);

    edit->SetTextToItem(4, buf);
    */
}


void Google::dnr_not_ready()
{
    GBS_StartTimerProc(&reconnect_timer, 308, reconnetc);
}


void Google::end_socket_connect(int m)
{
    switch(m)
    {
        case REMOTE_CLOSED_SOCKET:
        case END_SOCKET_CONNECT:
            parse(buffer);

            input_traf  += this->getReceivedSize();
            output_traf += this->getSendedSize();
            break;

        case DNR_ERROR:
            //ShowMSG(1, (int)"DNR Error");
            edit->SetTextToItem(4, "ошибка");
            close();
            break;

        case SOCKET_ERROR:
            edit->SetTextToItem(4, "ќшибка при создании сокета");
            close();
            break;

    }
}


Google::Google()
{
}

Google::~Google()
{

}


int Google :: translate(char *_text, const string & thislang, const string & translatelang)
{
    if(!IsGPRSEnabled())
    {
        edit->SetTextToItem(4, lgp->lang[FirstEnableGPRS].c_str());
        return 0;
    }

    std::string text = _text;
    size_t it = text.find("\n");;

    while(it != std::string::npos)
    {
        text.replace(it, 1, "%0A");
        it = text.find("\n");
    }

    it = text.find(" ");
    while( it != std::string::npos )
    {
        text.replace(it, 1, "+");
        it = text.find(" ");
    }

    this->abort();
    uint32_t uft8_len = ansi2utf8(text.c_str(), buffer);   // в utf-8 кодировку
    buffer[uft8_len] = 0;

    this->clean_header();
    this->set_url("translate.google.com");
    //this->set_port(8080);

    this->http_add_header("GET /translate_a/t?client=t&text=%s&hl=ru&sl=%s&tl=%s&prev=inc&otf=2&pc=1 HTTP/1.0",
                          buffer, thislang.c_str(), translatelang.c_str());
    this->http_add_header("User-Agent: Opera/9.62");
    this->http_add_header("Host: %s", this->get_url());
    this->http_add_header("Referer: http://translate.google.com");
    this->http_add_header("Accept: */*");
    this->http_add_header("Connection: close\r\n");

    this->connect();

    this->write(this->get_header(), strlen(this->get_header()) );

    this->read(buffer, 2*1024);

    return 1;
}



/* ============================= ѕарсер =============================== */

char __sgn[] = {'.', ',', '?', '!', ':', 0};
char errbuf[128];
#define put_error(fmt, ...) sprintf(errbuf, fmt, ##__VA_ARGS__); tempbuf = errbuf;
#define IsSigned1(c) ( c == '.' || c== ',' || c== '!' || c == '?' || c== ':' )


/* unaligned ldrh */
static inline
short getShort(const char *data)
{
    /*short val1;
    asm(
        "mov     r0, %1 \n"
        "ldrb    r1, [r0, #1] \n"
        "lsl     r1, #8 \n"
        "ldrb    r0, [r0, #0] \n"
        "orr     %0, r0, r1 \n"
        :"=r"(val1)
        :"r"(data)
        :"r0", "r1", "r2"
    );
    return val1;
    */
    return data[1] << 8 | data[0];
}



void removeSpecialCharacters(char *string)
{
    char *s = string;
    int start = 0;

    do
    {
        if(*(s) == '\\' && *(s+1) == '"')
        {
            start ++;

            if(start%2 != 0)
            {
                //qWarning() << "O_o";
                if(*(s+2) == ' ')
                {
                    strcpy(s, s+1);
                    strcpy(s+1, s+2);
                }
                else strcpy(s, s+1);

            } else
            {
                //qWarning() << "o_O";
                strcpy(s, s+1);
                start = 0;
            }
        }

        if( IsSigned1(*(s+1)) && *(s) == ' ')
        {
            strcpy(s, s+1);
        }

    }
    while(*s++);
}



int parse(char *rc)
{
#ifdef __LUA_SUPPORT__
    char *_tempbuf = 0;
    if( !is_use_lua ) goto no_lua;
    _tempbuf = new char[strlen(rc)+256];

    do_lua(rc, _tempbuf);

    edit->SetUtf8TextToItem(4, _tempbuf);

    delete _tempbuf;

    return 0;

no_lua:
#endif

#ifdef DEBUG
    debug(" ======= ANSVER ======= ");
    debug("%s", rc);
    debug(" ======== END ========= ");
#endif

    int brakets = 0;
    std::string tempbuf;
    const char *begin;


    char *s = strstr(rc, "[[[");
    if(s<rc || s>rc+strlen(rc))
    {
        edit->SetUtf8TextToItem(4, "Bad ansver\n");
        return -1;
    }

    s += 2;
    brakets ++;

    do
    {
        if(*s == '[')
        {
            ++s;
            if(*s != '"')
            {
                put_error("Error: expected a '\"', but got a '%c'\n", *s);
                break;
            }
            begin = s+1;
            ++s;
            // пройдемс€ до конца
            while(*s)
            {
                // *(short*)s - вызывала пик

                if(*s == '"' &&
                        getShort(s-1) != *(short*)"\\\""
                        //memcmp(s, &val2, 2)
                  )
                    break;
                ++s;
            }

            if(!*s)
            {
                put_error("Error: not full ansver\n");
                break;
            }

            if( getShort(s-2) == *(short*)"\\n" )
            {
                tempbuf += std::string().assign(begin, s-begin-2) + "\n";
            }
            else
                tempbuf += std::string().assign(begin, s-begin);



            while(*s)
            {

                if(*s == ']' &&
                        getShort(s-1) != *(short*)"\\]"
                        //memcmp(s, "\\]", 2)
                  )
                    break;
                ++s;
            }

            if(!*s)
            {
                put_error("Error: not full ansver\n");
                break;
            }

            if(*s != ']')
            {
                put_error("Error: bad charset '%c'\n", *s);
                break;
            }

            ++s;

            if(*s == ']')
            {
                brakets--;
                if(!brakets)
                {
                    break;
                }
            }
            else if(*s != ',')
            {
                put_error("Error: parse error\n");
                break;
            }

        }
        else if( *s == ']')
        {
            brakets--;
            if(!brakets)
            {
                break;
            }
        }

    }
    while(*s++);


    /* не по феншую, но в пригцыпе огромных скоростей здесь ненадо */
    /*char repl[4] = "  \x0";
    for(size_t i=0; i < sizeof(__sgn)-1; ++i)
    {
        repl[0] = ' ';
        repl[1] = __sgn[i];
        repl[2] = 0;

        size_t it = tempbuf.find(repl);
        if(it != std::string::npos)
        {
            repl[0] = __sgn[i];
            repl[1] = 0;
            tempbuf.replace(it, 2, repl);
        }
    }*/

    /* ниху€, все должно быть по феншую */
    char *d = new char[tempbuf.length()+1];
    strcpy(d, tempbuf.c_str());
    removeSpecialCharacters(d);

    /* потом запилим */
    /*int len = 0;
    WSHDR *ws = AllocWS((len = strlen(d))+2);
    wsprintf(ws, d);
    CopyTextToClipboard(ws, 0, len);
    FreeWS(ws);*/

    edit->SetUtf8TextToItem(4, d);
    //edit->SetFocusTo(4);
    //edit->SetCursorTo(1);
    delete d;
    REDRAW();
    return 0;
}

