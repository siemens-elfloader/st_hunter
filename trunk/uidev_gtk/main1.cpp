#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <cairo.h>
#include <gtk/gtk.h>

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

static gboolean on_expose_event (GtkWidget *widget, GdkEventExpose *event, gpointer data) {

    Image *img=(Image*)data;

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

    return FALSE;
}

int main (int argc, char **argv) {

    /*AIMG img (200, 200, T_16COL);
    img.Fill (0xFF0000FF); //ARGB

    img.DrawLine (10, 10, 100, 100, 0xFFFFFFFF);

    img.Rotate90 ();

    img.FlipVertic ();*/

    //Image img;
    //int t1=GetMicrosec ();
    //int err=img.CreateFromJPEG ("/home/andre/320.jpg");
    //int err=img.CreateFromJPEG ("/home/andre/1280.jpg");
    //int t2=GetMicrosec ();
    //int t1=GetMicrosec ();
    //img.Rotate270 ();
    //int t2=GetMicrosec ();

    //cout<<"time"<<t2-t1<<'\n';

    //if (err) return -1;

    Image img2;

    int t1=GetMicrosec ();
    //img.FResize (&img2, 0.5);
    //img.FResize (&img2, 0.25);
    //img.Fill (0xFFFF0000);
    //img.DrawLayer (&img, 0, 0);
    //img.FRotate (&img2, 270);
    //img.DrawLine (100, 100, 0, 0, 0xFF00FF00);
    //img2.DrawLine (0, 0, 100, 40, 0xFF00FF00);
    //img2.DrawLineA (0, 10, 100, 40 + 10, 0xFF00FF00);

    //img.Rotate (180);

/*
    img.Resize (&img2, 1, 2);

    Image img3;
    img.Resize (&img3, 1, 4);

    MyRECT rc;
    rc.x=10;
    rc.x2=100;
    rc.y=10;
    rc.y2=100;
    img2.DrawLayerRect (&img3, 10, 10, rc);

    //img2.DrawLine (0, 0, 100, 40, 0xFF00FF00);

  */
    img2.CreateFromJPEG ("/home/andre/1.jpg");
    //img2.CreateFromJPEG ("/home/andre/320.jpg");
    img2.Invert ();
    //img2.GrayScale ();
    int t2=GetMicrosec ();

    ft_font *font=ft_open("/usr/share/fonts/truetype/ubuntu-font-family/Ubuntu-R.ttf", 50);

    if (!font) return -1;
    MyRECT rc = {0,0,240, 320};
    FT_DrawLetter (&img2, font, 'A', 50, 50, &rc,0xFF00FF0000, 1);

    printf ("%d", font->fti->h);

    DrawString_UTF8 (&img2, "Hello\\abcdefghjklmnopqrstu", font, 10, 100, 240,100+GetFontH (font), TEXT_ROTATE, 0xFFFFFFFF);
    if (font) ft_close(font);

    //cout<<"time"<<t2-t1<<'\n';

    int win_w=img2.GetW();
    int win_h=img2.GetH();

    //cout<<win_w<<'x'<<win_h<<'\n';

    //img.FlipHoriz ();

    /* Окно */
    GtkWidget *window;
    //GtkWidget *button;
    //GtkWidget *button_f;
    //GtkWidget *box;

    gtk_init (&argc, &argv);
    window = gtk_window_new (GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title (GTK_WINDOW (window), "TEST");
    g_signal_connect (G_OBJECT (window), "expose-event", G_CALLBACK (on_expose_event), &img2);
    g_signal_connect (G_OBJECT (window), "destroy", G_CALLBACK (gtk_main_quit), NULL);
    g_signal_connect (G_OBJECT (window), "delete_event", G_CALLBACK (gtk_main_quit), NULL);
    gtk_container_set_border_width (GTK_CONTAINER (window), 10);
    gtk_window_set_position (GTK_WINDOW (window), GTK_WIN_POS_CENTER);
    gtk_window_set_default_size (GTK_WINDOW (window), win_w, win_h);
    gtk_widget_set_app_paintable (window, TRUE);
    gtk_widget_show_all (window);
    gtk_main();

    return 0;
}
