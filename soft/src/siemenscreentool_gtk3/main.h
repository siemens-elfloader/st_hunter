#ifndef __MAIN_H
#define __MAIN_H
#define ROUND(x, y) floor(x * pow(10, y)) / pow(10, y)
#define VERSION "1.0.1"

#include <iostream>
#include <stdio.h>
#include <gtk/gtk.h>
#include <sys/time.h>
#include <sys/stat.h>
#include <sys/file.h>
#include <cairo.h>
#include <math.h>
#include <pthread.h>
#include <pwd.h>
#include <cstdio>
#include <malloc.h>
#include "comport.h"
#include "bfc.h"

#define RGB565_MODE 0x04
#define RGB888_MODE 0x05
#define RGBWTF_MODE 0x09

#define SCREENS_PADDING 2
#define SIZE_2x1 0x01
#define SIZE_3x1 0x02
#define SIZE_4x1 0x03
#define SIZE_2x2 0x04
#define SIZE_3x2 0x05
#define SIZE_4x2 0x06

#pragma pack(push, 1)
typedef union {
	struct {
		unsigned short speed; 
		char           device[64]; 
		char           last_dir[256]; 
	}; 
	char data[324]; 
} SST_Settings; 
#pragma pack(pop)

unsigned short screenshots_grid_size = SIZE_2x1; 
struct passwd *pw; 
SST_Settings *sst_settings; 
char error[1024]; 
const int speed_list[7] = {
	B57600, 
	B115200, 
	B230400, 
	B460800, 
	B500000, 
	B576000, 
	B921600
}; 

const char *speed_list_names[7] = {
	"57600", 
	"115200", 
	"230400", 
	"460800", 
	"500000", 
	"576000", 
	"921600"
}; 

enum {
	TARGET_ROOTWIN
}; 

static GtkTargetEntry target_list[] = {
	{(gchar *)"application/x-rootwindow-drop", 0, TARGET_ROOTWIN}
}; 
static guint n_targets = G_N_ELEMENTS(target_list); 

ComPort *com; 
Bfc *bfc; 
bool is_connected = false; 
bool is_locked = false; 

BFC_DisplayBufferInfo *display_info_buffer; 
BFC_DisplayInfo *display_info; 

GtkWidget *screen; 
GtkWidget *screen_frame; 
static cairo_surface_t *screen_surface; 

GtkWidget *screen_0; 
GtkWidget *screen_frame_0; 
static cairo_surface_t *screen_surface_0; 

GtkWidget *screen_1; 
GtkWidget *screen_frame_1; 
static cairo_surface_t *screen_surface_1; 

GtkWidget *screen_2; 
GtkWidget *screen_frame_2; 
static cairo_surface_t *screen_surface_2; 

GtkWidget *screen_3; 
GtkWidget *screen_frame_3; 
static cairo_surface_t *screen_surface_3; 

GtkWidget *screen_4; 
GtkWidget *screen_frame_4; 
static cairo_surface_t *screen_surface_4; 

GtkWidget *screen_5; 
GtkWidget *screen_frame_5; 
static cairo_surface_t *screen_surface_5; 

GtkWidget *screen_6; 
GtkWidget *screen_frame_6; 
static cairo_surface_t *screen_surface_6; 

GtkWidget *screen_7; 
GtkWidget *screen_frame_7; 
static cairo_surface_t *screen_surface_7; 

GtkWidget *set_size_2x1; 
GtkWidget *set_size_3x1; 
GtkWidget *set_size_4x1; 
GtkWidget *set_size_2x2; 
GtkWidget *set_size_3x2; 
GtkWidget *set_size_4x2; 

GtkWidget *save_menu; 
GtkWidget *save_menu_item; 

GtkWidget *__current_widget; // Костылище!1111
GtkWidget *set_size_grid; 
GtkWidget *about_button; 
GtkWidget *take_screenshot_button; 
GtkWidget *save_all_button; 
GtkWidget *main_screen_window; 
GtkWidget *main_screen_grid; 
GtkWidget *connect_grid; 
GtkWidget *window; 
GtkWidget *settings_grid; 
GtkWidget *settings; 
GtkWidget *popup; 
GtkWidget *progress; 
GtkWidget *speed_select; 
GtkWidget *device_entry; 
GtkWidget *main_grid; 
GtkWidget *grid; 
GtkWidget *frame; 
GtkWidget *reload_button; 
GtkWidget *connect_button; 
GtkWidget *disconnect_button; 
pthread_t thread; 
pthread_t bfc_thread; 

static void read_settings(); 
static void write_settings(); 
static void on_destroy(GtkWidget *widget, gpointer data); 
static void set_screen_size(int width, int height); 
static void connect_action(GtkWidget *widget, GdkEvent *event, gpointer data); 
static void *connect_action_thread(void *ptr); 
static void disconnect_action(GtkWidget *widget, GdkEvent *event, gpointer data); 
static void set_grid_size(GtkWidget *widget, GdkEvent *event, gpointer data); 
static void disconnect_action(GtkWidget *widget, GdkEvent *event, gpointer data); 
static void grab_screenshot(GtkWidget *widget, GdkEvent *event, gpointer data); 
static void *get_screen_thread(void *ptr); 
static void on_destroy(GtkWidget *widget, gpointer data); 
static void alert(const char *title, const char *message); 
static void save_surface(cairo_surface_t *surface); 
static void save_all_screens_action(GtkWidget *widget, GdkEvent *event, gpointer data); 
static void about_action(GtkWidget *widget, GdkEvent *event, gpointer data); 
static gboolean on_context_menu(GtkWidget *widget, GdkEventButton *event, gpointer gdata); 
static gboolean on_draw_screen(GtkWidget *widget, cairo_t *cr, gpointer data); 
static gboolean on_screen_drag_drop(GtkWidget *widget, GdkDragContext *dc, gint x, gint y, guint t, gpointer data); 
static gboolean on_configure_event(GtkWidget *widget, GdkEventButton *event, gpointer data); 
void *get_screen(void *ptr); 

int main(int argc, char *argv[]); 

#endif


