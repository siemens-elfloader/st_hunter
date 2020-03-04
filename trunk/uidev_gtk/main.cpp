#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <cairo.h>
#include <gtk/gtk.h>

#include <gdk/gdkkeysyms.h>

#include "alib/img.h"
#include "alib/font.h"
#include "alib/ui.h"

#include "mymenu.h"
//#include <iostream>

#include <sys/time.h>

using namespace std;

int GetMicrosec (){
    struct timeval tv;
    struct timezone tz;

    gettimeofday (&tv,&tz);
    return tv.tv_usec;
}

int DrawImageWithCairo (GtkWidget *widget, Image *img){

    int w=img->GetW();
    int h=img->GetH();

    cairo_format_t cairo_type;

    switch (img->GetBtype()){
        case T_16COL:
            cairo_type=CAIRO_FORMAT_RGB16_565;
        break;

        case T_32COL:
            cairo_type=CAIRO_FORMAT_ARGB32;
        break;

        default:
            return TRUE;
    }

    int cairo_line_size = cairo_format_stride_for_width (cairo_type, w);

    //int buffer_pix_size = cairo_line_size/w;
    cairo_t *cr = gdk_cairo_create (widget->window);
    cairo_surface_t *surface = cairo_image_surface_create_for_data (*img->GetBitmap (), cairo_type, w, h, cairo_line_size);
    cairo_set_operator (cr, CAIRO_OPERATOR_SOURCE);

    //int t1=GetMicrosec ();
    //cairo_scale (cr, 0.5, 0.5);
    //cairo_rotate (cr, 45*3.14/180);

    cairo_set_source_surface (cr, surface, 0, 0);
    cairo_rectangle (cr, 0, 0, w, h);
    cairo_fill (cr);
    //cairo_paint (cr);

    //int t2=GetMicrosec ();
    //cout<<"cairo scale:"<<t2-t1<<'\n';
    cairo_surface_destroy (surface);
    cairo_destroy (cr);

    return 0;
}

#define CFG_FONT_SIZE 16
int draw_count=0;
char *GetCurFont (){ return "/usr/share/fonts/truetype/ubuntu-font-family/Ubuntu-R.ttf"; }

bool IsWinStarted=0;
bool IsFontCfgUpdate=1;
int add_size=0;
UI myui;
MyMenu menu;
ft_font *ftf;
//Image *mybuffer;
Image buffer;
void UI_OnCreate (){
    IsWinStarted=1;
    int scrH=ScreenH ();
    int scrW=ScreenW ();

    init_color_set ();

    ftf = ft_open(GetCurFont (), CFG_FONT_SIZE);


    //img.CreateFromJPEG ("4:\\Zbin\\wallpaper.jpg");

    buffer.Create (scrW, scrH, T_16COL);
    //buffer.Fill (0xFF0000FF);

    myui.SetHeader ("Заголовок");
    myui.SetSofts ("Лево", "Право");

    char tmp[128];
    MenuItem item;
    for (int i=0; i<25; i++){
        menu.IncItemCount ();
        if (i==5) sprintf (tmp, "Item_with_long_name_font_ubuntu%d", i);
        else sprintf (tmp, "Item%d", i);
        item.SetName (tmp);
        menu.List.push_back (item);
    }

    //mybuffer=GetImgBuffer();
    //mybuffer=&buffer;
    menu.SetMenuRect (0, get_color_from_set(PANEL_UP_SIZE)+CFG_HEADER_Y+(get_color_from_set(PANEL_HEADER_SIZE)-GetFontH (ftf))/2+GetFontH (ftf)+5, ScreenW(),  buffer.GetH()-get_color_from_set(PANEL_DOWN_SIZE));
    menu.SetCoordinates (10, 4, 0, 0, 0, 0);//10,4

    myui.OnFocus ();
}

void UI_OnRedraw (GtkWidget *widget){
    bool need_redraw=0;
    if (IsFontCfgUpdate){
        ftf = ft_open(GetCurFont (), CFG_FONT_SIZE+add_size);
        menu.ReInitShow ();
        IsFontCfgUpdate=0;
    }

    if (myui.IsFocus ()){
        DrawBG (&buffer);
        DrawUpPanel (&buffer);
        DrawDownPanel(&buffer);
        DrawHeaderPanel (&buffer);
        myui.DrawHeader(&buffer, ftf);
        myui.DrawSofts (&buffer, ftf);

        need_redraw=menu.DrawMenuList (&myui, &buffer, ftf);

        DrawImageWithCairo (widget, &buffer);
    }

}

void UI_OnClose (){
    ft_close(ftf);
}

int DrawWindow (GtkWidget *widget){

    if (!IsWinStarted){
        UI_OnCreate ();
    }
    UI_OnRedraw (widget);
    /*
    Image buffer;

    buffer.CreateFromJPEG ("wallpaper.jpg");
    int win_w=buffer.GetW();
    int win_h=buffer.GetH();

    ft_font *font=ft_open(GetCurFont (), CFG_FONT_SIZE);

    if (!font) return -2;

    //UI
    init_color_set ();

    DrawUpPanel (&buffer);
    DrawDownPanel (&buffer);
    DrawHeaderPanel (&buffer);

    DrawHeaderText (&buffer, font, "Header", 0);
    DrawSoftsText (&buffer, font, "Left", "Right");

    if (font) ft_close(font);

    return DrawImageWithCairo (widget, &buffer);
    */

    return 0;
}

static gboolean on_expose_event (GtkWidget *widget, GdkEventExpose *event, gpointer data) {

    DrawWindow (widget);

    return FALSE;
}

int MyUIOnKey (GtkWidget *widget, int mess, int key){
    if (mess==KEY_DOWN || mess==LONG_PRESS){

        menu.MenuOnKey (mess, key);

        switch(key){

            case '#' :

            break;

            case RIGHT_SOFT:
            return (1);
            break;

            case VOL_UP_BUTTON:
            break;

            case VOL_DOWN_BUTTON:
            break;
        }

        DrawWindow (widget);
    }

    return 0;

}

int TranslateKey (int key){
    int key_t=0;
    switch (key){
        case GDK_Left: key_t=LEFT_BUTTON; break;
        case GDK_Up: key_t=UP_BUTTON; break;
        case GDK_Right: key_t=RIGHT_BUTTON; break;
        case GDK_Down: key_t=DOWN_BUTTON; break;
    }

    return key_t;
}
//обработчик нажатия клавиш
static gboolean callback_press_key(GtkWidget *widget, GdkEventKey *event, gpointer user_data)
{
    // если нажали клавишу 'пробел'
    if (event->keyval==GDK_space)// код независим от раскладки клавиатуры и языка ввода
    {
        // Узнаём состояние окна
        GdkWindowState state = gdk_window_get_state(widget->window);
        if(state & GDK_WINDOW_STATE_FULLSCREEN)
            // Если окно на весь экран - сворачиваем к нормальным размерам
            gtk_window_unfullscreen(GTK_WINDOW(widget));
        else
            // Если окно не на весь экран - разворачиваем его на весь экран
            gtk_window_fullscreen(GTK_WINDOW(widget));
    }


    MyUIOnKey (widget, KEY_DOWN, TranslateKey (event->keyval));

    return FALSE;// разрешаем программе дальше обрабатывать этот сигнал
}
/*
void hello( GtkWidget *widget,	gpointer data){
	g_print ("Здравствуй, мир!\n");
}
*/


int main (int argc, char **argv) {

    /* Окно */
    GtkWidget *window;
    GtkWidget *button;
    //GtkWidget *button_f;
    GtkWidget *box;

    gtk_init (&argc, &argv);
    window = gtk_window_new (GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title (GTK_WINDOW (window), "ALibs Linux Demo");
    g_signal_connect (G_OBJECT (window), "expose-event", G_CALLBACK (on_expose_event), 0);
    g_signal_connect (G_OBJECT (window), "destroy", G_CALLBACK (gtk_main_quit), NULL);
    g_signal_connect (G_OBJECT (window), "delete_event", G_CALLBACK (gtk_main_quit), NULL);
    gtk_signal_connect( GTK_OBJECT( window ),"key-press-event", G_CALLBACK(callback_press_key), NULL);

    gtk_container_set_border_width (GTK_CONTAINER (window), 10);
    gtk_window_set_position (GTK_WINDOW (window), GTK_WIN_POS_CENTER);


    GdkScreen *g_screen = gdk_screen_get_default ();
    int screen_w=gdk_screen_get_width  (g_screen);
    int screen_h=gdk_screen_get_height (g_screen);
    printf ("%dx%d\n", screen_w, screen_h);


    gtk_window_set_default_size (GTK_WINDOW (window), 240, 320);
    gtk_widget_set_app_paintable (window, TRUE);

/*
    button = gtk_button_new_with_label ("Здорово, Мир!");
    GtkWidget *label =  gtk_label_new ("Здорово, Мир!");
    gtk_signal_connect (GTK_OBJECT (button), "clicked", G_CALLBACK (hello), NULL);
    //gtk_container_add (GTK_CONTAINER (window), button);

    box = gtk_hbox_new(FALSE, 2);
    gtk_container_add(GTK_CONTAINER(window), box);

    gtk_box_pack_start(GTK_BOX(box), label, FALSE, FALSE, 3);
    gtk_box_pack_start(GTK_BOX(box), button, FALSE, FALSE, 3);
*/

    gtk_widget_show(window);
    //gtk_widget_show(box);
    //gtk_widget_show(button);
    //gtk_widget_show(label);

    gtk_main();


    return 0;
}
