

#include <swilib.h>
#include <sstream>
#include <spl/process.h>
#include <spl/coreevent.h>
#include <spl/csm.h>
#include <spl/gui.h>
#include <spl/lcd_primitives.h>
#include "main.h"


int csm_id, gui_id;
int scr_w, scr_h;
char info[128] = "";
RECT canvas;
LCDLAYER *mmi_layer;
MainPlayer *player = nullptr;


typedef struct
{
    CoreEvent head;
    void (*f)(/*void *, void *, void **/);
    void *d[3];
}BosEvent __attribute__((aligned(4)));


void BosThread(void (*f)())
{
    BosEvent e;

    e.head.id = 0;
    e.head.dispatcher = [=](void *e) {
        BosEvent *ev = (BosEvent*)e;

        ev->f(/*ev->d[0], ev->d[1], ev->d[2]*/);
    };

    /*va_list va;
    va_start(va, f);

    e.d[0] = va_arg(va, void *);
    e.d[1] = va_arg(va, void *);
    e.d[2] = va_arg(va, void *);*/

    e.f = (void (*)(/*void*, void*, void**/))f;

    sendEvent(getpid(), &e, sizeof e);
}



MainPlayer::MainPlayer() :
    SourceStream("http://195.95.206.17/HitFM_32"),
    aac_context(nullptr),
    mbuffer(nullptr),
    pcm_stream(nullptr),
    screen_ws(AllocWS(256))
{
    info.status = "Stoped";

    info.status = "Connecting...";
    REDRAW();
    fetchDirectConnection();
}


MainPlayer::~MainPlayer()
{
    SetVibration(0);
    FreeWS(screen_ws);
}

/* функция запроса данных */
int MainPlayer::readBuffer(void *data, uint32_t size)
{
    return pcm_stream->read(data, size);
}


void MainPlayer::connectStatus(int code)
{
    if(code) {
        ShowMSG(1, (int)"Error initializing");
        info.status = "Failed initialize";
    }
    else {

        info.status = "Connecting...";
        REDRAW();
        aac_context = new AACContext(ibStream());
        aac_context->initialize();

        setAudioParameters(ULONG_MAX, aac_context->samplerate(), aac_context->channels(), 16);

        std::ostringstream ss;

        ss << aac_context->samplerate();
        info.samplerate = ss.str();

        ss = std::ostringstream();
        ss << aac_context->channels();
        info.channels = ss.str();

        mbuffer = new MultivariateBuffer(2, (640 * 2) * 600);

        info.status = "Caching...";
        REDRAW();

        pcm_stream = new OutStream(aac_context, mbuffer);
        pcm_stream->waitForFlushed();

        info.status = "Playing";
        REDRAW();

        play();
        //setVolume(5);

        BosThread( [=]() {
                player->destroyConnection();
        } );

    }
}


int get_header_param(const std::string & head, const std::string & p, std::string & data)
{
    const char *name = strstr(head.c_str(), p.c_str());

    if(name) {
        name += p.length();
        char *end = strchr(name, '\r');

        if(end)
        {
            while(isspace(*name) && *name) name++;
            if(!*name) return -1;

            data.assign(name, end-name);
            return 0;
        }
    }

    return -1;
}


void MainPlayer::headerFetched()
{
    std::string s;
    get_header_param(SourceStream::header, "icy-name:", s);
    info.station.fromUtf8(s.c_str());

    get_header_param(SourceStream::header, "icy-description:", s);
    info.description.fromUtf8(s.c_str());

    get_header_param(SourceStream::header, "icy-br:", info.bitrate);

    REDRAW();
}


void MainPlayer::icyMetaUpdated()
{
    //ShowMSG(1, (int)icy_chunk().c_str());
    info.title.fromUtf8(icy_chunk.c_str(), 128);
    REDRAW();
}


void onRedrawGUI(int id)
{
    auto _ = [&](const std::string & s) -> const char * { return s.c_str(); };

    uint32_t red = 0xFF0000FF,
             black = 0xFF000000;
    lcd_draw_fillrect(mmi_layer, 0, 0, canvas.x2, canvas.y2, (char*)&red);

    if(player) {
        wsprintf(player->screen_ws, "Station: %w\nDescr.: %w\nTitle: %w\nChannels: %s\nBitrate: %s\nSamplerate: %s\nStatus: %s\nVolume: %d\n%s",
                (WSHDR*)player->info.station, (WSHDR*)player->info.description, (WSHDR*)player->info.title,
                _(player->info.channels), _(player->info.bitrate), _(player->info.samplerate),
                _(player->info.status), player? player->volume() : 0, info);

        lcd_draw_text(mmi_layer, player->screen_ws, 1, 10, scr_w, scr_h, FONT_SMALL, 0, (char*)&black, 0);
    }

    displayLayer(mmi_layer);
}


void onCreateGUI(int id)
{
    mmi_layer = *RamMMILCDLayer();

    player = new MainPlayer();
}


void onCloseGUI(int id)
{
    delete player;
}


void onFocusGUI(int id)
{
}


void onUnfocusGUI(int id)
{
}


void onKeyGUI(int id, GUI_MSG *msg)
{
    if ((msg->gbsmsg->msg == KEY_DOWN || msg->gbsmsg->msg == LONG_PRESS))
    {
        switch(msg->gbsmsg->submess)
        {
        case RIGHT_SOFT:
            closeGUI(id);
            return;

        case VOL_UP_BUTTON:
            if(player)
                player->addVolume(1);
            break;

        case VOL_DOWN_BUTTON:
            if(player)
                player->subVolume(1);
            break;
        }
    }

    REDRAW();
}



void onCreateCSM(int id, CSM_RAM *ram)
{
    canvas.x = 0;
    canvas.y = 0;
    scr_w = canvas.x2 = ScreenW();
    scr_h = canvas.y2 = ScreenH();

    gui_id = createGUI(&canvas, onRedrawGUI,
                       onCreateGUI,
                       onCloseGUI,
                       onFocusGUI,
                       onUnfocusGUI,
                       onKeyGUI,
                       NULL);

    printf("csm_id: %d | gui_id: %d\n", csm_id, gui_id);
    bindGUIToCSM(csm_id, getGUIid(gui_id));
}


void onCloseCSM(int id, CSM_RAM *ram)
{
    quit();
}



int main()
{
    initUsart();
    printf(" [+] main: pid: %d\n", getpid());

    csm_id = createCSM("RadioSi", CoreCSM_GUI, onCreateCSM, onCloseCSM, 0);

    processEvents();
    return 0;
}

