#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <cairo.h>
#include <gtk/gtk.h>

#include <gdk/gdkkeysyms.h>

//#include <jpeglib.h>

#include "alib/img.h"
#include "alib/font.h"

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

int draw_count=0;

int DrawWindow (GtkWidget *widget){
    Image img2;

    int t1=GetMicrosec ();
/*
    if (argc < 2) return -1;

    int ext=1;
    switch (ext){
        case 0: img2.CreateFromJPEG (argv[1]); break;
        case 1: img2.CreateFromPNG (argv[1]); break;
    }
    */

    Image img1; img1.CreateFromPNG ("/home/andre/1.png");
    img1.ResizeQuality (&img2, 1, 4);
    //img2.CreateFromJPEG ("/home/andre/1.jpg");
    //img2.Invert ();
    //img2.GrayScale ();
    int win_w=img2.GetW();
    int win_h=img2.GetH();

    int t2=GetMicrosec ();

    ft_font *font=ft_open("/usr/share/fonts/truetype/ubuntu-font-family/Ubuntu-R.ttf", 20);

    if (!font) return -2;

    MyRECT rc = {0,0,240, 320};
    FT_DrawLetter (&img2, font, 'A', 50, 50, &rc,0xFF00FF0000, 1);

    //printf ("%d", font->fti->h);
    draw_count++;
    char text_buf[128];
    sprintf (text_buf, "%dx%dHello? world djqYPg", win_w, draw_count);

    DrawString_UTF8 (&img2, text_buf, font, 10, 100, win_w, 100+GetFontH (font), 0, 0xFFFFFFFF);
    if (font) ft_close(font);

    img2.DrawFillCircle (150, 50, 150, 0x7FFF00FF);


    //cout<<"time"<<t2-t1<<'\n';

    //cout<<win_w<<'x'<<win_h<<'\n';

    //img.FlipHoriz ();

    return DrawImageWithCairo (widget, &img2);
}

static gboolean on_expose_event (GtkWidget *widget, GdkEventExpose *event, gpointer data) {

    DrawWindow (widget);

    return FALSE;
}


/*
#define GDK_Left 0xff51
#define GDK_Up 0xff52
#define GDK_Right 0xff53
#define GDK_Down 0xff54
*/
//обработчик нажатия клавиш
static gboolean callback_press_key(GtkWidget *widget, GdkEventKey *event, gpointer user_data)
{
    // если нажали клавишу 'пробел'
    if(event->keyval==GDK_space)// код независим от раскладки клавиатуры и языка ввода
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

    if(event->keyval==GDK_Up)
    {
        DrawWindow (widget);

    }
    return FALSE;// разрешаем программе дальше обрабатывать этот сигнал
}

int main (int argc, char **argv) {

  /* Окно */
  GtkWidget *window;
  //GtkWidget *button;
  //GtkWidget *button_f;
  //GtkWidget *box;

  gtk_init (&argc, &argv);
  window = gtk_window_new (GTK_WINDOW_TOPLEVEL);
  gtk_window_set_title (GTK_WINDOW (window), "xDraw-test");
  g_signal_connect (G_OBJECT (window), "expose-event", G_CALLBACK (on_expose_event), 0);
  g_signal_connect (G_OBJECT (window), "destroy", G_CALLBACK (gtk_main_quit), NULL);
  g_signal_connect (G_OBJECT (window), "delete_event", G_CALLBACK (gtk_main_quit), NULL);
  gtk_container_set_border_width (GTK_CONTAINER (window), 10);
  gtk_window_set_position (GTK_WINDOW (window), GTK_WIN_POS_CENTER);


  GdkScreen *g_screen = gdk_screen_get_default ();
  int screen_w=gdk_screen_get_width  (g_screen);
  int screen_h=gdk_screen_get_height (g_screen);
  printf ("%dx%d\n", screen_w, screen_h);


  gtk_window_set_default_size (GTK_WINDOW (window), screen_w/2, screen_h/2);
  gtk_widget_set_app_paintable (window, TRUE);
  gtk_widget_show_all (window);

  gtk_signal_connect( GTK_OBJECT( window ),"key-press-event",GTK_SIGNAL_FUNC(callback_press_key), NULL);

  gtk_main();


  return 0;
}
