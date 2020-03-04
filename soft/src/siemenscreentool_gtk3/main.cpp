#include "main.h"

using namespace std; 

static void *get_screen_thread(void *ptr) {
	if (is_locked)
		pthread_exit(ptr); 
	
	if (!bfc -> getDisplayInfo(display_info)) {
		cout << "Error: can't get display info!\n"; 
		pthread_exit(ptr); 
	}
	
	if (!bfc -> getDisplayBufferInfo(display_info_buffer, display_info -> client_id)) {
		cout << "Error: can't get display buffer info!\n"; 
		pthread_exit(ptr); 
	}
	
	is_locked = true; 
	int pixel_len = 0; 
	
	switch (display_info_buffer -> type) {
		case RGBWTF_MODE: // Эта хрень на елке o_O
			pixel_len = 4; 
		break; 
		
		case RGB888_MODE: 
			pixel_len = 4; 
		break; 
		
		default: 
			pixel_len = 2; 
		break; 
	}
	
	char title[64]; 
	int len = 0; 
	double progress_step = 0; 
	double progress_curr = 0; 
	unsigned long color = 0; 
	unsigned char *scr = new unsigned char[display_info_buffer -> width * pixel_len]; 
	cairo_t *cr = cairo_create(screen_surface); 
	
	gdk_threads_enter(); 
	gtk_progress_bar_set_fraction(GTK_PROGRESS_BAR(progress), 0); 
	gtk_window_set_title(GTK_WINDOW(window), title); 
	gdk_threads_leave(); 
	
	printf(
		"\tBFC_DisplayBufferInfo.subcmd    = 0x%02X\n\
\tBFC_DisplayBufferInfo.client_id = 0x%02X\n\
\tBFC_DisplayBufferInfo.width     = %d\n\
\tBFC_DisplayBufferInfo.height    = %d\n\
\tBFC_DisplayBufferInfo.offet_x   = %d\n\
\tBFC_DisplayBufferInfo.offet_y   = %d\n\
\tBFC_DisplayBufferInfo.address   = 0x%X\n\
\tBFC_DisplayBufferInfo.type      = 0x%02X\n", 
		display_info_buffer -> subcmd, 
		display_info_buffer -> client_id, 
		display_info_buffer -> width, 
		display_info_buffer -> height, 
		display_info_buffer -> offet_x, 
		display_info_buffer -> offet_y, 
		display_info_buffer -> address, 
		display_info_buffer -> type
	); 
	
	// display_info_buffer -> address = 0xACD00020; 
	
	progress_step = 1 / (double)display_info_buffer -> height; 
	for (int y = 0; y < display_info_buffer -> height; y++) {
		len = bfc -> readMem(display_info_buffer -> address + y * display_info_buffer -> width * pixel_len, scr, display_info_buffer -> width * pixel_len); 
		if (len == display_info_buffer -> width * pixel_len)
			for (int x = 0; x < len; x += pixel_len) {
				gdk_threads_enter(); 
				switch (display_info_buffer -> type) {
					case RGBWTF_MODE: // Эта хрень на елке o_O
						cairo_set_source_rgb(
							cr, 
							(float)scr[x + 2] / 0xFF, 
							(float)scr[x + 1] / 0xFF, 
							(float)scr[x] / 0xFF
						); 
					break; 
					
					case RGB888_MODE: 
						cairo_set_source_rgb(
							cr, 
							(float)scr[x + 2] / 0xFF, 
							(float)scr[x + 1] / 0xFF, 
							(float)scr[x] / 0xFF
						); 
					break; 
					
					default: 
						color = (scr[x + 1] << 8) + scr[x]; 
						cairo_set_source_rgb(
							cr, 
							(float)(((color >> 11) << 3) & 0xFF) / 0xFF, 
							(float)(((color >> 5) << 2) & 0xFF) / 0xFF, 
							(float)((color << 3) & 0xFF) / 0xFF
						); 
					break; 
				}
				cairo_rectangle(cr, x / pixel_len, y, 1, 1); 
				cairo_fill(cr); 
				gdk_threads_leave(); 
			}
		gdk_threads_enter(); 
		progress_curr += progress_step; 
		snprintf(title, 64, "Siemens Screenshot Tool | Downloading... %d of %d bytes", (y + 1) * display_info_buffer -> width * pixel_len, display_info_buffer -> height * display_info_buffer -> width * pixel_len); 
		gtk_window_set_title(GTK_WINDOW(window), title); 
		gtk_progress_bar_set_fraction(GTK_PROGRESS_BAR(progress), progress_curr); 
		gtk_widget_queue_draw_area(screen, 0, y, display_info_buffer -> width, 1); 
		gdk_threads_leave(); 
	}
	is_locked = false; 
	
	gdk_threads_enter(); 
	cairo_destroy(cr); 
	gtk_widget_queue_draw_area(screen, 0, 0, display_info_buffer -> width, display_info_buffer -> height); // На тот случай, если криво перерисовалось. 
	gdk_threads_leave(); 
	
	delete[] scr; 
	pthread_exit(ptr); 
}

static void *connect_action_thread(void *ptr) {
	char buff[1024]; 
	
	strncpy(sst_settings -> device, gtk_entry_get_text(GTK_ENTRY(device_entry)), 64); 
	sst_settings -> speed = gtk_combo_box_get_active(GTK_COMBO_BOX(speed_select)); 
	
	com -> setDevice(sst_settings -> device); 
	com -> setSpeed(speed_list[sst_settings -> speed]); 
	com -> setTimeout(0); 
	
	write_settings(); 
	gdk_threads_enter(); 
	gtk_progress_bar_set_fraction(GTK_PROGRESS_BAR(progress), 0.0); 
	gdk_threads_leave(); 
	
	try {
		com -> open(); 
	} catch (ComPortException e) { 
		com -> close(); 
		gdk_threads_enter(); 
		alert("Error!", e.getMessage()); 
		gdk_threads_leave(); 
		pthread_exit(ptr); 
	}
	
	com -> setTimeout(200); 
	com -> setMultiplierTimeout(1); 
	while (com -> readChar() >= 0); 
	
	gdk_threads_enter(); 
	gtk_progress_bar_set_fraction(GTK_PROGRESS_BAR(progress), 0.2); 
	gdk_threads_leave(); 
	
	if (!bfc -> auth(0x11)) {
		com -> write("ATE0\r", 5); 
		
		if (com -> read(buff, 11) <= 0 || strstr(buff, "\r\nOK\r\n") == NULL) {
			gdk_threads_enter(); 
			alert("Error!", "Can't send AT ping!"); 
			gdk_threads_leave(); 
			com -> close(); 
			pthread_exit(ptr); 
		}
		
		gdk_threads_enter(); 
		gtk_progress_bar_set_fraction(GTK_PROGRESS_BAR(progress), 0.4); 
		gdk_threads_leave(); 
		
		com -> write("AT^SQWE=1\r", 10); 
		if (com -> read(buff, 6) != 6 || strncmp(buff, "\r\nOK\r\n", 6) != 0) {
			gdk_threads_enter(); 
			alert("Error!", "Can't set BFC mode!"); 
			gdk_threads_leave(); 
			com -> close(); 
			pthread_exit(ptr); 
		}
		
		gdk_threads_enter(); 
		gtk_progress_bar_set_fraction(GTK_PROGRESS_BAR(progress), 0.6); 
		gdk_threads_leave(); 
		
		usleep(300000); 
		if (!bfc -> auth(0x11)) {
			gdk_threads_enter(); 
			alert("Error!", "Can't send BFC ping!"); 
			gdk_threads_leave(); 
			com -> close(); 
			pthread_exit(ptr); 
		}
		
		gdk_threads_enter(); 
		gtk_progress_bar_set_fraction(GTK_PROGRESS_BAR(progress), 0.8); 
		gdk_threads_leave(); 
	} else {
		gdk_threads_enter(); 
		alert("Warning!", "Already in BFC mode!"); 
		gtk_progress_bar_set_fraction(GTK_PROGRESS_BAR(progress), 0.8); 
		gdk_threads_leave(); 
	}
	
	if (!bfc -> getDisplayInfo(display_info)) {
		gdk_threads_enter(); 
		alert("Error!", "Can't get display information!"); 
		gdk_threads_leave(); 
		com -> close(); 
		pthread_exit(ptr); 
	}
	
	cout << "BFC_DisplayInfo dump:\n"; 
	printf(
		"\tBFC_DisplayInfo.subcmd    = 0x%02X\n\
\tBFC_DisplayInfo.width     = %d\n\
\tBFC_DisplayInfo.height    = %d\n\
\tBFC_DisplayInfo.client_id = 0x%02X\n", 
		display_info -> subcmd, 
		display_info -> width, 
		display_info -> height, 
		display_info -> client_id
	); 
	
	if (!bfc -> getDisplayBufferInfo(display_info_buffer, display_info -> client_id)) {
		gdk_threads_enter(); 
		alert("Error!", "Can't get display buffer information!"); 
		gdk_threads_leave(); 
		com -> close(); 
		pthread_exit(ptr); 
	}
	
	cout << "BFC_DisplayBufferInfo dump:\n"; 
	printf(
		"\tBFC_DisplayBufferInfo.subcmd    = 0x%02X\n\
\tBFC_DisplayBufferInfo.client_id = 0x%02X\n\
\tBFC_DisplayBufferInfo.width     = %d\n\
\tBFC_DisplayBufferInfo.height    = %d\n\
\tBFC_DisplayBufferInfo.offet_x   = %d\n\
\tBFC_DisplayBufferInfo.offet_y   = %d\n\
\tBFC_DisplayBufferInfo.address   = 0x%X\n\
\tBFC_DisplayBufferInfo.type      = 0x%02X\n", 
		display_info_buffer -> subcmd, 
		display_info_buffer -> client_id, 
		display_info_buffer -> width, 
		display_info_buffer -> height, 
		display_info_buffer -> offet_x, 
		display_info_buffer -> offet_y, 
		display_info_buffer -> address, 
		display_info_buffer -> type
	); 
	
	gdk_threads_enter(); 
	set_screen_size(display_info_buffer -> width, display_info_buffer -> height); 
	gtk_widget_set_sensitive(disconnect_button, TRUE); 
	gtk_widget_set_sensitive(main_screen_window, TRUE); 
	gtk_widget_set_sensitive(connect_button, FALSE); 
	gtk_widget_set_sensitive(speed_select, FALSE); 
	gtk_widget_set_sensitive(device_entry, FALSE); 
	gtk_widget_set_sensitive(frame, TRUE); 
	gtk_widget_set_sensitive(save_all_button, TRUE); 
	gtk_progress_bar_set_fraction(GTK_PROGRESS_BAR(progress), 1.0); 
	gdk_threads_leave();
	
	is_connected = true; 
	pthread_exit(ptr); 
}

static void grab_screenshot(GtkWidget *widget, GdkEvent *event, gpointer data) {
	if (!is_locked)
		pthread_create(&bfc_thread, NULL, get_screen_thread, NULL); 
}

static void about_action(GtkWidget *widget, GdkEvent *event, gpointer data) {
	gtk_show_about_dialog(
		GTK_WINDOW(window), 
		"program-name", "Siemens Screenshot Tool", 
		"copyright", "Copyright © 2012 Kiril Zhumarin", 
		"version", VERSION, 
		"comments", "Tool for taking screenshots. ", 
		"website", "http://zhumarin.ru/siemens/sst", 
		NULL
	); 
}

static void save_all_screens_action(GtkWidget *widget, GdkEvent *event, gpointer data) {
	int width = 0, height = 0, x = 0, y = 0; 
	cairo_t *cr_dest; 
	cairo_surface_t *all_screens_surface; 
	
	switch (screenshots_grid_size) {
		case SIZE_2x1:
			width = display_info_buffer -> width * 2 + SCREENS_PADDING * 3; 
			height = display_info_buffer -> height + SCREENS_PADDING * 2; 
			
			all_screens_surface = gdk_window_create_similar_surface(
				gtk_widget_get_window(widget), 
				CAIRO_CONTENT_COLOR, 
				width, 
				height
			); 
			cr_dest = cairo_create(all_screens_surface); 
			
			cairo_set_source_rgb(cr_dest, 1, 1, 1); 
			cairo_rectangle(cr_dest, 0, 0, width, height); 
			cairo_fill(cr_dest); 
			
			x = SCREENS_PADDING; 
			y = SCREENS_PADDING; 
			cairo_set_source_surface(cr_dest, screen_surface_0, x, y); 
			cairo_paint(cr_dest); 
			
			x += display_info_buffer -> width + SCREENS_PADDING; 
			cairo_set_source_surface(cr_dest, screen_surface_1, x, y); 
			cairo_paint(cr_dest); 
		break; 
		
		case SIZE_3x1:
			width = display_info_buffer -> width * 3 + SCREENS_PADDING * 4; 
			height = display_info_buffer -> height + SCREENS_PADDING * 2; 
			
			all_screens_surface = gdk_window_create_similar_surface(
				gtk_widget_get_window(widget), 
				CAIRO_CONTENT_COLOR, 
				width, 
				height
			); 
			cr_dest = cairo_create(all_screens_surface); 
			
			cairo_set_source_rgb(cr_dest, 1, 1, 1); 
			cairo_rectangle(cr_dest, 0, 0, width, height); 
			cairo_fill(cr_dest); 
			
			x = SCREENS_PADDING; 
			y = SCREENS_PADDING; 
			cairo_set_source_surface(cr_dest, screen_surface_0, x, y); 
			cairo_paint(cr_dest); 
			
			x += display_info_buffer -> width + SCREENS_PADDING; 
			cairo_set_source_surface(cr_dest, screen_surface_1, x, y); 
			cairo_paint(cr_dest); 
			
			x += display_info_buffer -> width + SCREENS_PADDING; 
			cairo_set_source_surface(cr_dest, screen_surface_2, x, y); 
			cairo_paint(cr_dest); 
		break; 
		
		case SIZE_4x1:
			width = display_info_buffer -> width * 4 + SCREENS_PADDING * 5; 
			height = display_info_buffer -> height + SCREENS_PADDING * 2; 
			
			all_screens_surface = gdk_window_create_similar_surface(
				gtk_widget_get_window(widget), 
				CAIRO_CONTENT_COLOR, 
				width, 
				height
			); 
			cr_dest = cairo_create(all_screens_surface); 
			
			cairo_set_source_rgb(cr_dest, 1, 1, 1); 
			cairo_rectangle(cr_dest, 0, 0, width, height); 
			cairo_fill(cr_dest); 
			
			x = SCREENS_PADDING; 
			y = SCREENS_PADDING; 
			cairo_set_source_surface(cr_dest, screen_surface_0, x, y); 
			cairo_paint(cr_dest); 
			
			x += display_info_buffer -> width + SCREENS_PADDING; 
			cairo_set_source_surface(cr_dest, screen_surface_1, x, y); 
			cairo_paint(cr_dest); 
			
			x += display_info_buffer -> width + SCREENS_PADDING; 
			cairo_set_source_surface(cr_dest, screen_surface_2, x, y); 
			cairo_paint(cr_dest); 
			
			x += display_info_buffer -> width + SCREENS_PADDING; 
			cairo_set_source_surface(cr_dest, screen_surface_3, x, y); 
			cairo_paint(cr_dest); 
		break; 
		
		case SIZE_2x2:
			width = display_info_buffer -> width * 2 + SCREENS_PADDING * 3; 
			height = display_info_buffer -> height * 2 + SCREENS_PADDING * 3; 
			
			all_screens_surface = gdk_window_create_similar_surface(
				gtk_widget_get_window(widget), 
				CAIRO_CONTENT_COLOR, 
				width, 
				height
			); 
			cr_dest = cairo_create(all_screens_surface); 
			
			cairo_set_source_rgb(cr_dest, 1, 1, 1); 
			cairo_rectangle(cr_dest, 0, 0, width, height); 
			cairo_fill(cr_dest); 
			
			x = SCREENS_PADDING; 
			y = SCREENS_PADDING; 
			cairo_set_source_surface(cr_dest, screen_surface_0, x, y); 
			cairo_paint(cr_dest); 
			
			x += display_info_buffer -> width + SCREENS_PADDING; 
			cairo_set_source_surface(cr_dest, screen_surface_1, x, y); 
			cairo_paint(cr_dest); 
			
			x = SCREENS_PADDING; 
			y += display_info_buffer -> height + SCREENS_PADDING; 
			cairo_set_source_surface(cr_dest, screen_surface_4, x, y); 
			cairo_paint(cr_dest); 
			
			x += display_info_buffer -> width + SCREENS_PADDING; 
			cairo_set_source_surface(cr_dest, screen_surface_5, x, y); 
			cairo_paint(cr_dest); 
		break; 
		
		case SIZE_3x2:
			width = display_info_buffer -> width * 3 + SCREENS_PADDING * 4; 
			height = display_info_buffer -> height * 2 + SCREENS_PADDING * 3; 
			
			all_screens_surface = gdk_window_create_similar_surface(
				gtk_widget_get_window(widget), 
				CAIRO_CONTENT_COLOR, 
				width, 
				height
			); 
			cr_dest = cairo_create(all_screens_surface); 
			
			cairo_set_source_rgb(cr_dest, 1, 1, 1); 
			cairo_rectangle(cr_dest, 0, 0, width, height); 
			cairo_fill(cr_dest); 
			
			x += SCREENS_PADDING; 
			y += SCREENS_PADDING; 
			cairo_set_source_surface(cr_dest, screen_surface_0, x, y); 
			cairo_paint(cr_dest); 
			
			x += display_info_buffer -> width + SCREENS_PADDING; 
			cairo_set_source_surface(cr_dest, screen_surface_1, x, y); 
			cairo_paint(cr_dest); 
			
			x += display_info_buffer -> width + SCREENS_PADDING; 
			cairo_set_source_surface(cr_dest, screen_surface_2, x, y); 
			cairo_paint(cr_dest); 
			
			x = SCREENS_PADDING; 
			y += display_info_buffer -> height + SCREENS_PADDING; 
			cairo_set_source_surface(cr_dest, screen_surface_4, x, y); 
			cairo_paint(cr_dest); 
			
			x += display_info_buffer -> width + SCREENS_PADDING; 
			cairo_set_source_surface(cr_dest, screen_surface_5, x, y); 
			cairo_paint(cr_dest); 
			
			x += display_info_buffer -> width + SCREENS_PADDING; 
			cairo_set_source_surface(cr_dest, screen_surface_6, x, y); 
			cairo_paint(cr_dest); 
		break; 
		
		case SIZE_4x2:
			width = display_info_buffer -> width * 4 + SCREENS_PADDING * 5; 
			height = display_info_buffer -> height * 2 + SCREENS_PADDING * 3; 
			
			all_screens_surface = gdk_window_create_similar_surface(
				gtk_widget_get_window(widget), 
				CAIRO_CONTENT_COLOR, 
				width, 
				height
			); 
			cr_dest = cairo_create(all_screens_surface); 
			
			cairo_set_source_rgb(cr_dest, 1, 1, 1); 
			cairo_rectangle(cr_dest, 0, 0, width, height); 
			cairo_fill(cr_dest); 
			
			x += SCREENS_PADDING; 
			y += SCREENS_PADDING; 
			cairo_set_source_surface(cr_dest, screen_surface_0, x, y); 
			cairo_paint(cr_dest); 
			
			x += display_info_buffer -> width + SCREENS_PADDING; 
			cairo_set_source_surface(cr_dest, screen_surface_1, x, y); 
			cairo_paint(cr_dest); 
			
			x += display_info_buffer -> width + SCREENS_PADDING; 
			cairo_set_source_surface(cr_dest, screen_surface_2, x, y); 
			cairo_paint(cr_dest); 
			
			x += display_info_buffer -> width + SCREENS_PADDING; 
			cairo_set_source_surface(cr_dest, screen_surface_3, x, y); 
			cairo_paint(cr_dest); 
			
			x = SCREENS_PADDING; 
			y += display_info_buffer -> height + SCREENS_PADDING; 
			cairo_set_source_surface(cr_dest, screen_surface_4, x, y); 
			cairo_paint(cr_dest); 
			
			x += display_info_buffer -> width + SCREENS_PADDING; 
			cairo_set_source_surface(cr_dest, screen_surface_5, x, y); 
			cairo_paint(cr_dest); 
			
			x += display_info_buffer -> width + SCREENS_PADDING; 
			cairo_set_source_surface(cr_dest, screen_surface_6, x, y); 
			cairo_paint(cr_dest); 
			
			x += display_info_buffer -> width + SCREENS_PADDING; 
			cairo_set_source_surface(cr_dest, screen_surface_7, x, y); 
			cairo_paint(cr_dest); 
		break; 
		
		default: 
			all_screens_surface = gdk_window_create_similar_surface(
				gtk_widget_get_window(widget), 
				CAIRO_CONTENT_COLOR, 
				132, 
				176
			); 
			cr_dest = cairo_create(all_screens_surface); 
		break; 
	}
	cairo_destroy(cr_dest); 
	save_surface(all_screens_surface); 
	cairo_surface_destroy(all_screens_surface); 
}

static void on_save_screen(GtkWidget *widget, GdkEvent *event, gpointer data) {
	if (__current_widget == screen) {
		save_surface(screen_surface); 
	} else if (__current_widget == screen_0) {
		save_surface(screen_surface_0); 
	} else if (__current_widget == screen_1) {
		save_surface(screen_surface_1); 
	} else if (__current_widget == screen_2) {
		save_surface(screen_surface_2); 
	} else if (__current_widget == screen_3) {
		save_surface(screen_surface_3); 
	} else if (__current_widget == screen_4) {
		save_surface(screen_surface_4); 
	} else if (__current_widget == screen_5) {
		save_surface(screen_surface_5); 
	} else if (__current_widget == screen_6) {
		save_surface(screen_surface_6); 
	} else if (__current_widget == screen_7) {
		save_surface(screen_surface_7); 
	}
}

static void save_surface(cairo_surface_t *surface) {
	time_t now = time(0); 
	struct tm tstruct; 
	char filename[64]; 
	
	GtkWidget *dialog = gtk_file_chooser_dialog_new(
		"Save screenshot", 
		GTK_WINDOW(window), 
		GTK_FILE_CHOOSER_ACTION_SAVE, 
		GTK_STOCK_CANCEL, 
		GTK_RESPONSE_CANCEL, 
		GTK_STOCK_SAVE, 
		GTK_RESPONSE_ACCEPT, 
		NULL
	); 
	
	tstruct = *localtime(&now); 
	strftime(filename, sizeof(filename), "Screenshot at %Y-%m-%d %X.png", &tstruct); 
	
	gtk_file_chooser_set_do_overwrite_confirmation(GTK_FILE_CHOOSER(dialog), TRUE); 
	gtk_file_chooser_set_current_folder(GTK_FILE_CHOOSER(dialog), sst_settings -> last_dir); 
	gtk_file_chooser_set_current_name(GTK_FILE_CHOOSER(dialog), filename); 
	
	if (gtk_dialog_run(GTK_DIALOG(dialog)) == GTK_RESPONSE_ACCEPT) {
		char *filename = gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(dialog)); 
		cairo_status_t status = cairo_surface_write_to_png(surface, filename); 
		strncpy(sst_settings -> last_dir, gtk_file_chooser_get_current_folder(GTK_FILE_CHOOSER(dialog)), 256); 
		
		if (status != CAIRO_STATUS_SUCCESS)
			alert("Error!", cairo_status_to_string(status)); 
		
		g_free(filename); 
	}
	
	gtk_widget_destroy(dialog); 
}

static gboolean on_screen_drag_drop(GtkWidget *widget, GdkDragContext *ctx, gint x, gint y, guint t, gpointer data) {
	GtkWidget *source_widget = gtk_drag_get_source_widget(ctx); 
	if (source_widget == widget)
		return FALSE; 
	
	cairo_t *cr_dest; 
	if (widget == screen_0) {
		cr_dest = cairo_create(screen_surface_0); 
	} else if (widget == screen_1) {
		cr_dest = cairo_create(screen_surface_1); 
	} else if (widget == screen_2) {
		cr_dest = cairo_create(screen_surface_2); 
	} else if (widget == screen_3) {
		cr_dest = cairo_create(screen_surface_3); 
	} else if (widget == screen_4) {
		cr_dest = cairo_create(screen_surface_4); 
	} else if (widget == screen_5) {
		cr_dest = cairo_create(screen_surface_5); 
	} else if (widget == screen_6) {
		cr_dest = cairo_create(screen_surface_6); 
	} else if (widget == screen_7) {
		cr_dest = cairo_create(screen_surface_7); 
	}
	
	if (source_widget == screen) {
		cairo_set_source_surface(cr_dest, screen_surface, 0, 0); 
	} else if (source_widget == screen_0) {
		cairo_set_source_surface(cr_dest, screen_surface_0, 0, 0); 
	} else if (source_widget == screen_1) {
		cairo_set_source_surface(cr_dest, screen_surface_1, 0, 0); 
	} else if (source_widget == screen_2) {
		cairo_set_source_surface(cr_dest, screen_surface_2, 0, 0); 
	} else if (source_widget == screen_3) {
		cairo_set_source_surface(cr_dest, screen_surface_3, 0, 0); 
	} else if (source_widget == screen_4) {
		cairo_set_source_surface(cr_dest, screen_surface_4, 0, 0); 
	} else if (source_widget == screen_5) {
		cairo_set_source_surface(cr_dest, screen_surface_5, 0, 0); 
	} else if (source_widget == screen_6) {
		cairo_set_source_surface(cr_dest, screen_surface_6, 0, 0); 
	} else if (source_widget == screen_7) {
		cairo_set_source_surface(cr_dest, screen_surface_7, 0, 0); 
	}
	
	cairo_paint(cr_dest); 
	gtk_widget_queue_draw_area(screen, 0, 0, display_info_buffer -> width, display_info_buffer -> height); 
	cairo_destroy(cr_dest); 
	
	return TRUE; 
}

static void set_grid_size(GtkWidget *widget, GdkEvent *event, gpointer data) {
	if (widget == set_size_2x1) {
		screenshots_grid_size = SIZE_2x1; 
		gtk_widget_show(screen_frame_0); 
		gtk_widget_show(screen_frame_1); 
		gtk_widget_hide(screen_frame_2); 
		gtk_widget_hide(screen_frame_3); 
		gtk_widget_hide(screen_frame_4); 
		gtk_widget_hide(screen_frame_5); 
		gtk_widget_hide(screen_frame_6); 
		gtk_widget_hide(screen_frame_7); 
	} else if (widget == set_size_2x2) {
		screenshots_grid_size = SIZE_2x2; 
		gtk_widget_show(screen_frame_0); 
		gtk_widget_show(screen_frame_1); 
		gtk_widget_hide(screen_frame_2); 
		gtk_widget_hide(screen_frame_3); 
		gtk_widget_show(screen_frame_4); 
		gtk_widget_show(screen_frame_5); 
		gtk_widget_hide(screen_frame_6); 
		gtk_widget_hide(screen_frame_7); 
	} else if (widget == set_size_3x1) {
		screenshots_grid_size = SIZE_3x1; 
		gtk_widget_show(screen_frame_0); 
		gtk_widget_show(screen_frame_1); 
		gtk_widget_show(screen_frame_2); 
		gtk_widget_hide(screen_frame_3); 
		gtk_widget_hide(screen_frame_4); 
		gtk_widget_hide(screen_frame_5); 
		gtk_widget_hide(screen_frame_6); 
		gtk_widget_hide(screen_frame_7); 
	} else if (widget == set_size_3x2) {
		screenshots_grid_size = SIZE_3x2; 
		gtk_widget_show(screen_frame_0); 
		gtk_widget_show(screen_frame_1); 
		gtk_widget_show(screen_frame_2); 
		gtk_widget_hide(screen_frame_3); 
		gtk_widget_show(screen_frame_4); 
		gtk_widget_show(screen_frame_5); 
		gtk_widget_show(screen_frame_6); 
		gtk_widget_hide(screen_frame_7); 
	} else if (widget == set_size_4x1) {
		screenshots_grid_size = SIZE_4x1; 
		gtk_widget_show(screen_frame_0); 
		gtk_widget_show(screen_frame_1); 
		gtk_widget_show(screen_frame_2); 
		gtk_widget_show(screen_frame_3); 
		gtk_widget_hide(screen_frame_4); 
		gtk_widget_hide(screen_frame_5); 
		gtk_widget_hide(screen_frame_6); 
		gtk_widget_hide(screen_frame_7); 
	} else if (widget == set_size_4x2) {
		screenshots_grid_size = SIZE_4x2; 
		gtk_widget_show(screen_frame_0); 
		gtk_widget_show(screen_frame_1);  
		gtk_widget_show(screen_frame_2); 
		gtk_widget_show(screen_frame_3); 
		gtk_widget_show(screen_frame_4); 
		gtk_widget_show(screen_frame_5); 
		gtk_widget_show(screen_frame_6); 
		gtk_widget_show(screen_frame_7); 
	}
}

static void connect_action(GtkWidget *widget, GdkEvent *event, gpointer data) {
	pthread_create(&thread, NULL, connect_action_thread, NULL); 
}

static void disconnect_action(GtkWidget *widget, GdkEvent *event, gpointer data) {
	pthread_cancel(bfc_thread); 
	if (!bfc -> sendAT("AT^SQWE=2\r", 10))
		alert("Warning!", "Can't set AT mode!"); 
	com -> close(); 
	
	gtk_widget_set_sensitive(disconnect_button, FALSE); 
	gtk_widget_set_sensitive(connect_button, TRUE); 
	gtk_widget_set_sensitive(device_entry, TRUE); 
	gtk_widget_set_sensitive(speed_select, TRUE); 
	gtk_widget_set_sensitive(frame, FALSE); 
	gtk_widget_set_sensitive(save_all_button, FALSE); 
	gtk_progress_bar_set_fraction(GTK_PROGRESS_BAR(progress), 0); 
	
	is_connected = false; 
}

static void on_destroy(GtkWidget *widget, gpointer data) {
	write_settings(); 
	cairo_surface_destroy(screen_surface); 
	cairo_surface_destroy(screen_surface_0); 
	cairo_surface_destroy(screen_surface_1); 
	cairo_surface_destroy(screen_surface_2); 
	cairo_surface_destroy(screen_surface_3); 
	cairo_surface_destroy(screen_surface_4); 
	cairo_surface_destroy(screen_surface_5); 
	cairo_surface_destroy(screen_surface_6); 
	cairo_surface_destroy(screen_surface_7); 
	
	delete display_info_buffer; 
	delete display_info; 
	delete sst_settings; 
	delete bfc; 
	delete com; 
	
	gtk_main_quit(); 
}

static gboolean on_context_menu(GtkWidget *widget, GdkEventButton *event, gpointer gdata) {
	if (event -> button == 3) {
		__current_widget = widget; 
		gtk_menu_popup(GTK_MENU(save_menu), NULL, NULL, NULL, NULL, event -> button, event -> time); 
		return TRUE; 
	}
	return FALSE; 
}

static gboolean on_draw_screen(GtkWidget *widget, cairo_t *cr, gpointer data) {
	if (widget == screen) {
		cairo_set_source_surface(cr, screen_surface, 0, 0); 
	} else if (widget == screen_0) {
		cairo_set_source_surface(cr, screen_surface_0, 0, 0); 
	} else if (widget == screen_1) {
		cairo_set_source_surface(cr, screen_surface_1, 0, 0); 
	} else if (widget == screen_2) {
		cairo_set_source_surface(cr, screen_surface_2, 0, 0); 
	} else if (widget == screen_3) {
		cairo_set_source_surface(cr, screen_surface_3, 0, 0); 
	} else if (widget == screen_4) {
		cairo_set_source_surface(cr, screen_surface_4, 0, 0); 
	} else if (widget == screen_5) {
		cairo_set_source_surface(cr, screen_surface_5, 0, 0); 
	} else if (widget == screen_6) {
		cairo_set_source_surface(cr, screen_surface_6, 0, 0); 
	} else if (widget == screen_7) {
		cairo_set_source_surface(cr, screen_surface_7, 0, 0); 
	}
	cairo_paint(cr); 
	return TRUE; 
}

static gboolean on_configure_event(GtkWidget *widget, GdkEventButton *event, gpointer data) {
	if (widget == screen) {
		if (screen_surface)
			cairo_surface_destroy(screen_surface); 
		screen_surface = gdk_window_create_similar_surface(
			gtk_widget_get_window(widget), 
			CAIRO_CONTENT_COLOR, 
			display_info_buffer -> width, 
			display_info_buffer -> height
		); 
	} else if (widget == screen_0) {
		if (screen_surface_0)
			cairo_surface_destroy(screen_surface_0); 
		screen_surface_0 = gdk_window_create_similar_surface(
			gtk_widget_get_window(widget), 
			CAIRO_CONTENT_COLOR, 
			display_info_buffer -> width, 
			display_info_buffer -> height
		); 
	} else if (widget == screen_1) {
		if (screen_surface_1)
			cairo_surface_destroy(screen_surface_1); 
		screen_surface_1 = gdk_window_create_similar_surface(
			gtk_widget_get_window(widget), 
			CAIRO_CONTENT_COLOR, 
			display_info_buffer -> width, 
			display_info_buffer -> height
		); 
	} else if (widget == screen_2) {
		if (screen_surface_2)
			cairo_surface_destroy(screen_surface_2); 
		screen_surface_2 = gdk_window_create_similar_surface(
			gtk_widget_get_window(widget), 
			CAIRO_CONTENT_COLOR, 
			display_info_buffer -> width, 
			display_info_buffer -> height
		); 
	} else if (widget == screen_3) {
		if (screen_surface_3)
			cairo_surface_destroy(screen_surface_3); 
		screen_surface_3 = gdk_window_create_similar_surface(
			gtk_widget_get_window(widget), 
			CAIRO_CONTENT_COLOR, 
			display_info_buffer -> width, 
			display_info_buffer -> height
		); 
	} else if (widget == screen_4) {
		if (screen_surface_4)
			cairo_surface_destroy(screen_surface_4); 
		screen_surface_4 = gdk_window_create_similar_surface(
			gtk_widget_get_window(widget), 
			CAIRO_CONTENT_COLOR, 
			display_info_buffer -> width, 
			display_info_buffer -> height
		); 
	} else if (widget == screen_5) {
		if (screen_surface_5)
			cairo_surface_destroy(screen_surface_5); 
		screen_surface_5 = gdk_window_create_similar_surface(
			gtk_widget_get_window(widget), 
			CAIRO_CONTENT_COLOR, 
			display_info_buffer -> width, 
			display_info_buffer -> height
		); 
	} else if (widget == screen_6) {
		if (screen_surface_6)
			cairo_surface_destroy(screen_surface_6); 
		screen_surface_6 = gdk_window_create_similar_surface(
			gtk_widget_get_window(widget), 
			CAIRO_CONTENT_COLOR, 
			display_info_buffer -> width, 
			display_info_buffer -> height
		); 
	} else if (widget == screen_7) {
		if (screen_surface_7)
			cairo_surface_destroy(screen_surface_7); 
		screen_surface_7 = gdk_window_create_similar_surface(
			gtk_widget_get_window(widget), 
			CAIRO_CONTENT_COLOR, 
			display_info_buffer -> width, 
			display_info_buffer -> height
		); 
	}
	return TRUE; 
}

static void alert(const char *title, const char *message) {
	GtkWidget *dialog, *label, *content_area; 
	dialog = gtk_dialog_new_with_buttons(title, GTK_WINDOW(window), GTK_DIALOG_DESTROY_WITH_PARENT, GTK_STOCK_OK, GTK_RESPONSE_NONE, NULL); 
	content_area = gtk_dialog_get_content_area(GTK_DIALOG(dialog)); 
	label = gtk_label_new(message); 
	gtk_container_set_border_width(GTK_CONTAINER(dialog), 10); 
	gtk_window_set_resizable(GTK_WINDOW(dialog), FALSE); 
	gtk_window_set_modal(GTK_WINDOW(dialog), TRUE); 
	g_signal_connect_swapped(dialog, "response", G_CALLBACK(gtk_widget_destroy), dialog); 
	gtk_container_add(GTK_CONTAINER(content_area), label); 
	gtk_widget_show_all(dialog); 
}

static void set_screen_size(int width, int height) {
	gtk_widget_set_size_request(screen, width, height); 
	gtk_widget_set_size_request(screen_0, width, height); 
	gtk_widget_set_size_request(screen_1, width, height); 
	gtk_widget_set_size_request(screen_2, width, height); 
	gtk_widget_set_size_request(screen_3, width, height); 
	gtk_widget_set_size_request(screen_4, width, height); 
	gtk_widget_set_size_request(screen_5, width, height); 
	gtk_widget_set_size_request(screen_6, width, height); 
	gtk_widget_set_size_request(screen_7, width, height); 
}

static void read_settings() {
	char path[256]; 
	snprintf(path, 256, "%s/.siemens-screenshot-tool-%s/config.azq2", pw -> pw_dir, VERSION); 
	
	if (access(path, F_OK) != 0)
		write_settings(); 
	
	int fd = open(path, O_RDWR); 
	flock(fd, LOCK_EX); 
	if (read(fd, sst_settings -> data, sizeof(SST_Settings)) != sizeof(SST_Settings))
		cout << "Warning: Can't read settings!\n"; 
	flock(fd, LOCK_UN); 
	close(fd);
}

static void write_settings() {
	char path[256]; 
	snprintf(path, 256, "%s/.siemens-screenshot-tool-%s", pw -> pw_dir, VERSION); 
	 
	if (access(path, F_OK) != 0)
		if (mkdir(path, 0755) != 0) {
			cout << "Can't create dir '" << path << "'!\n"; 
			return; 
		}
	
	snprintf(path, 256, "%s/.siemens-screenshot-tool-%s/config.azq2", pw -> pw_dir, VERSION); 
	int fd = open(path, O_CREAT | O_TRUNC | O_RDWR); 
	
	if (fd == -1) {
		cout << "Can't write file '" << path << "'!\n"; 
		return; 
	}
	
	flock(fd, LOCK_EX); 
	if (write(fd, sst_settings -> data, sizeof(SST_Settings)) != sizeof(SST_Settings))
		cout << "Warning: Can't write settings!\n"; 
	flock(fd, LOCK_UN); 
	close(fd);
	
	chmod(path, 0666);  
}

int main(int argc, char *argv[]) {
	setbuf(stdout, NULL); 
	
	
	cout << UINT_MAX; 
	
	g_thread_init(NULL); 
	gdk_threads_init(); 
	gdk_threads_enter(); 
	
	gtk_init(&argc, &argv);
	
	bfc = new Bfc(); 
	com = new ComPort(); 
	sst_settings = new SST_Settings; 
	
	pw = getpwuid(getuid()); 
	sst_settings -> speed = 1; 
	strncpy(sst_settings -> device, "/dev/ttyS0", 64); 
	strncpy(sst_settings -> last_dir, pw -> pw_dir, 256); 
	
	bfc -> setComPort(com); 
	read_settings(); 
	
	display_info = new BFC_DisplayInfo; 
	display_info_buffer = new BFC_DisplayBufferInfo; 
	
	is_connected = false; 
	display_info_buffer -> width = 132; 
	display_info_buffer -> height = 176; 
	
	screen = gtk_drawing_area_new(); 
	screen_frame = gtk_frame_new(NULL); 
	screen_0 = gtk_drawing_area_new(); 
	screen_frame_0 = gtk_frame_new(NULL); 
	screen_1 = gtk_drawing_area_new(); 
	screen_frame_1 = gtk_frame_new(NULL); 
	screen_2 = gtk_drawing_area_new(); 
	screen_frame_2 = gtk_frame_new(NULL); 
	screen_3 = gtk_drawing_area_new(); 
	screen_frame_3 = gtk_frame_new(NULL); 
	screen_4 = gtk_drawing_area_new(); 
	screen_frame_4 = gtk_frame_new(NULL); 
	screen_5 = gtk_drawing_area_new(); 
	screen_frame_5 = gtk_frame_new(NULL); 
	screen_6 = gtk_drawing_area_new(); 
	screen_frame_6 = gtk_frame_new(NULL); 
	screen_7 = gtk_drawing_area_new(); 
	screen_frame_7 = gtk_frame_new(NULL); 
	
	set_size_2x1 = gtk_radio_button_new_with_label(NULL, "2x1"); 
	set_size_3x1 = gtk_radio_button_new_with_label(
		gtk_radio_button_get_group(GTK_RADIO_BUTTON(set_size_2x1)), 
		"3x1"
	); 
	set_size_4x1 = gtk_radio_button_new_with_label(
		gtk_radio_button_get_group(GTK_RADIO_BUTTON(set_size_3x1)), 
		"4x1"
	); 
	set_size_2x2 = gtk_radio_button_new_with_label(
		gtk_radio_button_get_group(GTK_RADIO_BUTTON(set_size_4x1)), 
		"2x2"
	); 
	set_size_3x2 = gtk_radio_button_new_with_label(
		gtk_radio_button_get_group(GTK_RADIO_BUTTON(set_size_2x2)), 
		"3x2"
	); 
	set_size_4x2 = gtk_radio_button_new_with_label(
		gtk_radio_button_get_group(GTK_RADIO_BUTTON(set_size_3x2)), 
		"4x2"
	); 
	
	save_menu = gtk_menu_new(); 
	save_menu_item = gtk_menu_item_new_with_label("Save"); 
	 
	grid = gtk_grid_new(); 
	main_grid = gtk_grid_new(); 
	settings_grid = gtk_grid_new(); 
	set_size_grid = gtk_grid_new(); 
	connect_grid = gtk_grid_new(); 
	main_screen_grid = gtk_grid_new(); 
	progress = gtk_progress_bar_new(); 
	speed_select = gtk_combo_box_text_new(); 
	device_entry = gtk_entry_new(); 
	frame = gtk_frame_new("Screenshots"); 
	settings = gtk_frame_new("Settings"); 
	
	main_screen_window = gtk_window_new(GTK_WINDOW_TOPLEVEL); 
	window = gtk_window_new(GTK_WINDOW_TOPLEVEL); 
	
	connect_button = gtk_button_new_with_label("Open"); 
	disconnect_button = gtk_button_new_with_label("Close"); 
	reload_button = gtk_button_new_with_label("Refresh"); 
	take_screenshot_button = gtk_button_new_with_label("Grab!"); 
	save_all_button = gtk_button_new_with_label("Save all!"); 
	about_button = gtk_button_new_with_label("?"); 
	
	gtk_menu_shell_append(GTK_MENU_SHELL(save_menu), save_menu_item); 
	
	gtk_drag_source_set(screen, GDK_BUTTON1_MASK, target_list, n_targets, GDK_ACTION_COPY); 
	gtk_drag_source_set(screen_0, GDK_BUTTON1_MASK, target_list, n_targets, GDK_ACTION_COPY); 
	gtk_drag_source_set(screen_1, GDK_BUTTON1_MASK, target_list, n_targets, GDK_ACTION_COPY); 
	gtk_drag_source_set(screen_2, GDK_BUTTON1_MASK, target_list, n_targets, GDK_ACTION_COPY); 
	gtk_drag_source_set(screen_3, GDK_BUTTON1_MASK, target_list, n_targets, GDK_ACTION_COPY); 
	gtk_drag_source_set(screen_4, GDK_BUTTON1_MASK, target_list, n_targets, GDK_ACTION_COPY); 
	gtk_drag_source_set(screen_5, GDK_BUTTON1_MASK, target_list, n_targets, GDK_ACTION_COPY); 
	gtk_drag_source_set(screen_6, GDK_BUTTON1_MASK, target_list, n_targets, GDK_ACTION_COPY); 
	gtk_drag_source_set(screen_7, GDK_BUTTON1_MASK, target_list, n_targets, GDK_ACTION_COPY); 
	
	gtk_drag_dest_set(screen_0, (GtkDestDefaults)(GTK_DEST_DEFAULT_MOTION | GTK_DEST_DEFAULT_HIGHLIGHT), target_list, n_targets, GDK_ACTION_COPY);
	gtk_drag_dest_set(screen_1, (GtkDestDefaults)(GTK_DEST_DEFAULT_MOTION | GTK_DEST_DEFAULT_HIGHLIGHT), target_list, n_targets, GDK_ACTION_COPY);
	gtk_drag_dest_set(screen_2, (GtkDestDefaults)(GTK_DEST_DEFAULT_MOTION | GTK_DEST_DEFAULT_HIGHLIGHT), target_list, n_targets, GDK_ACTION_COPY);
	gtk_drag_dest_set(screen_3, (GtkDestDefaults)(GTK_DEST_DEFAULT_MOTION | GTK_DEST_DEFAULT_HIGHLIGHT), target_list, n_targets, GDK_ACTION_COPY);
	gtk_drag_dest_set(screen_4, (GtkDestDefaults)(GTK_DEST_DEFAULT_MOTION | GTK_DEST_DEFAULT_HIGHLIGHT), target_list, n_targets, GDK_ACTION_COPY);
	gtk_drag_dest_set(screen_5, (GtkDestDefaults)(GTK_DEST_DEFAULT_MOTION | GTK_DEST_DEFAULT_HIGHLIGHT), target_list, n_targets, GDK_ACTION_COPY);
	gtk_drag_dest_set(screen_6, (GtkDestDefaults)(GTK_DEST_DEFAULT_MOTION | GTK_DEST_DEFAULT_HIGHLIGHT), target_list, n_targets, GDK_ACTION_COPY);
	gtk_drag_dest_set(screen_7, (GtkDestDefaults)(GTK_DEST_DEFAULT_MOTION | GTK_DEST_DEFAULT_HIGHLIGHT), target_list, n_targets, GDK_ACTION_COPY);
	
	set_screen_size(display_info_buffer -> width, display_info_buffer -> height); 
	gtk_window_set_resizable(GTK_WINDOW(window), FALSE); 
	gtk_window_set_title(GTK_WINDOW(window), "Siemens Screenshot Tool"); 
	gtk_container_set_border_width(GTK_CONTAINER(window), 8); 
	gtk_container_set_border_width(GTK_CONTAINER(settings), 3); 
	gtk_container_set_border_width(GTK_CONTAINER(frame), 3); 
	gtk_container_set_border_width(GTK_CONTAINER(settings_grid), 3); 
	
	gtk_entry_set_text(GTK_ENTRY(device_entry), sst_settings -> device); 
	gtk_entry_set_width_chars(GTK_ENTRY(device_entry), 12); 
	gtk_entry_set_max_length(GTK_ENTRY(device_entry), 1024); 
	
	gtk_window_set_title(GTK_WINDOW(main_screen_window), "Screenshot"); 
	gtk_window_set_transient_for(GTK_WINDOW(main_screen_window), GTK_WINDOW(window)); 
	gtk_window_set_resizable(GTK_WINDOW(main_screen_window), FALSE); 
	
	g_signal_connect(screen, "draw", G_CALLBACK(on_draw_screen), NULL); 
	g_signal_connect(screen_0, "draw", G_CALLBACK(on_draw_screen), NULL); 
	g_signal_connect(screen_1, "draw", G_CALLBACK(on_draw_screen), NULL); 
	g_signal_connect(screen_2, "draw", G_CALLBACK(on_draw_screen), NULL); 
	g_signal_connect(screen_3, "draw", G_CALLBACK(on_draw_screen), NULL); 
	g_signal_connect(screen_4, "draw", G_CALLBACK(on_draw_screen), NULL); 
	g_signal_connect(screen_5, "draw", G_CALLBACK(on_draw_screen), NULL); 
	g_signal_connect(screen_6, "draw", G_CALLBACK(on_draw_screen), NULL); 
	g_signal_connect(screen_7, "draw", G_CALLBACK(on_draw_screen), NULL); 
	
	g_signal_connect(main_screen_window, "destroy", G_CALLBACK(on_destroy), NULL); 
	g_signal_connect(window, "destroy", G_CALLBACK(on_destroy), NULL); 
	g_signal_connect(screen, "configure-event", G_CALLBACK(on_configure_event), NULL); 
	g_signal_connect(screen_0, "configure-event", G_CALLBACK(on_configure_event), NULL); 
	g_signal_connect(screen_1, "configure-event", G_CALLBACK(on_configure_event), NULL); 
	g_signal_connect(screen_2, "configure-event", G_CALLBACK(on_configure_event), NULL); 
	g_signal_connect(screen_3, "configure-event", G_CALLBACK(on_configure_event), NULL); 
	g_signal_connect(screen_4, "configure-event", G_CALLBACK(on_configure_event), NULL); 
	g_signal_connect(screen_5, "configure-event", G_CALLBACK(on_configure_event), NULL); 
	g_signal_connect(screen_6, "configure-event", G_CALLBACK(on_configure_event), NULL); 
	g_signal_connect(screen_7, "configure-event", G_CALLBACK(on_configure_event), NULL); 	
	
	gtk_container_add(GTK_CONTAINER(main_screen_window), main_screen_grid); 
	gtk_container_add(GTK_CONTAINER(screen_frame), screen); 
	gtk_grid_attach(GTK_GRID(main_screen_grid), screen_frame, 0, 0, 1, 1); 
	gtk_grid_attach(GTK_GRID(main_screen_grid), take_screenshot_button, 0, 1, 1, 1); 
	
	gtk_container_add(GTK_CONTAINER(screen_frame_0), screen_0); 
	gtk_container_add(GTK_CONTAINER(screen_frame_1), screen_1); 
	gtk_container_add(GTK_CONTAINER(screen_frame_2), screen_2); 
	gtk_container_add(GTK_CONTAINER(screen_frame_3), screen_3); 
	gtk_container_add(GTK_CONTAINER(screen_frame_4), screen_4); 
	gtk_container_add(GTK_CONTAINER(screen_frame_5), screen_5); 
	gtk_container_add(GTK_CONTAINER(screen_frame_6), screen_6); 
	gtk_container_add(GTK_CONTAINER(screen_frame_7), screen_7); 
	
	gtk_grid_attach(GTK_GRID(grid), screen_frame_0, 0, 0, 1, 1); 
	gtk_grid_attach(GTK_GRID(grid), screen_frame_1, 1, 0, 1, 1); 
	gtk_grid_attach(GTK_GRID(grid), screen_frame_2, 2, 0, 1, 1); 
	gtk_grid_attach(GTK_GRID(grid), screen_frame_3, 3, 0, 1, 1); 
	gtk_grid_attach(GTK_GRID(grid), screen_frame_4, 0, 1, 1, 1); 
	gtk_grid_attach(GTK_GRID(grid), screen_frame_5, 1, 1, 1, 1); 
	gtk_grid_attach(GTK_GRID(grid), screen_frame_6, 2, 1, 1, 1); 
	gtk_grid_attach(GTK_GRID(grid), screen_frame_7, 3, 1, 1, 1); 
	
	gtk_container_set_border_width(GTK_CONTAINER(screen_frame_0), 3); 
	gtk_container_set_border_width(GTK_CONTAINER(screen_frame_1), 3); 
	gtk_container_set_border_width(GTK_CONTAINER(screen_frame_2), 3); 
	gtk_container_set_border_width(GTK_CONTAINER(screen_frame_3), 3); 
	gtk_container_set_border_width(GTK_CONTAINER(screen_frame_4), 3); 
	gtk_container_set_border_width(GTK_CONTAINER(screen_frame_5), 3); 
	gtk_container_set_border_width(GTK_CONTAINER(screen_frame_6), 3); 
	gtk_container_set_border_width(GTK_CONTAINER(screen_frame_7), 3); 
	
	gtk_container_add(GTK_CONTAINER(frame), grid); 
	gtk_container_add(GTK_CONTAINER(settings), settings_grid); 
	
	gtk_grid_attach(GTK_GRID(main_grid), frame, 0, 1, 1, 1); 
	gtk_grid_attach(GTK_GRID(main_grid), settings, 0, 0, 1, 1); 
	gtk_grid_attach(GTK_GRID(main_grid), progress, 0, 2, 2, 1); 
	gtk_grid_attach(GTK_GRID(connect_grid), device_entry, 0, 0, 1, 1); 
	
	for (unsigned int i = 0; i < sizeof(speed_list) / sizeof(char *); i++)
		gtk_combo_box_text_insert(GTK_COMBO_BOX_TEXT(speed_select), i, speed_list_names[i], speed_list_names[i]); 
	gtk_combo_box_set_active(GTK_COMBO_BOX(speed_select), sst_settings -> speed); 
	
	gtk_grid_attach(GTK_GRID(connect_grid), speed_select, 1, 0, 1, 1); 
	gtk_grid_attach(GTK_GRID(connect_grid), connect_button, 2, 0, 1, 1); 
	gtk_grid_attach(GTK_GRID(connect_grid), disconnect_button, 3, 0, 1, 1); 
	gtk_grid_attach(GTK_GRID(connect_grid), save_all_button, 4, 0, 1, 1); 
	gtk_grid_attach(GTK_GRID(connect_grid), about_button, 5, 0, 1, 1); 
	
	gtk_grid_attach(GTK_GRID(settings_grid), connect_grid, 0, 0, 1, 1);
	
	gtk_grid_attach(GTK_GRID(set_size_grid), set_size_2x1, 0, 1, 1, 1); 
	gtk_grid_attach(GTK_GRID(set_size_grid), set_size_3x1, 1, 1, 1, 1); 
	gtk_grid_attach(GTK_GRID(set_size_grid), set_size_4x1, 2, 1, 1, 1); 
	gtk_grid_attach(GTK_GRID(set_size_grid), set_size_2x2, 3, 1, 1, 1); 
	gtk_grid_attach(GTK_GRID(set_size_grid), set_size_3x2, 4, 1, 1, 1); 
	gtk_grid_attach(GTK_GRID(set_size_grid), set_size_4x2, 5, 1, 1, 1); 
	gtk_grid_attach(GTK_GRID(settings_grid), set_size_grid, 0, 1, 1, 1);
	
	gtk_container_add(GTK_CONTAINER(window), main_grid); 
	
	gtk_widget_set_sensitive(disconnect_button, FALSE); 
	gtk_widget_set_sensitive(main_screen_window, FALSE); 
	gtk_widget_set_sensitive(save_all_button, FALSE); 
	
	g_signal_connect(screen_0, "drag-drop", G_CALLBACK(on_screen_drag_drop), NULL); 
	g_signal_connect(screen_1, "drag-drop", G_CALLBACK(on_screen_drag_drop), NULL); 
	g_signal_connect(screen_2, "drag-drop", G_CALLBACK(on_screen_drag_drop), NULL); 
	g_signal_connect(screen_3, "drag-drop", G_CALLBACK(on_screen_drag_drop), NULL); 
	g_signal_connect(screen_4, "drag-drop", G_CALLBACK(on_screen_drag_drop), NULL); 
	g_signal_connect(screen_5, "drag-drop", G_CALLBACK(on_screen_drag_drop), NULL); 
	g_signal_connect(screen_6, "drag-drop", G_CALLBACK(on_screen_drag_drop), NULL); 
	g_signal_connect(screen_7, "drag-drop", G_CALLBACK(on_screen_drag_drop), NULL); 
	
	g_signal_connect(G_OBJECT(take_screenshot_button), "clicked", G_CALLBACK(grab_screenshot), NULL); 
	g_signal_connect(G_OBJECT(connect_button), "clicked", G_CALLBACK(connect_action), NULL); 
	g_signal_connect(G_OBJECT(disconnect_button), "clicked", G_CALLBACK(disconnect_action), NULL); 
	g_signal_connect(G_OBJECT(save_all_button), "clicked", G_CALLBACK(save_all_screens_action), NULL); 
	g_signal_connect(G_OBJECT(about_button), "clicked", G_CALLBACK(about_action), NULL); 
	
	g_signal_connect(G_OBJECT(set_size_2x1), "clicked", G_CALLBACK(set_grid_size), NULL); 
	g_signal_connect(G_OBJECT(set_size_3x1), "clicked", G_CALLBACK(set_grid_size), NULL); 
	g_signal_connect(G_OBJECT(set_size_4x1), "clicked", G_CALLBACK(set_grid_size), NULL); 
	g_signal_connect(G_OBJECT(set_size_2x2), "clicked", G_CALLBACK(set_grid_size), NULL); 
	g_signal_connect(G_OBJECT(set_size_3x2), "clicked", G_CALLBACK(set_grid_size), NULL); 
	g_signal_connect(G_OBJECT(set_size_4x2), "clicked", G_CALLBACK(set_grid_size), NULL); 
	
	g_signal_connect(G_OBJECT(screen), "button-press-event", G_CALLBACK(on_context_menu), NULL); 
	g_signal_connect(G_OBJECT(screen_0), "button-press-event", G_CALLBACK(on_context_menu), NULL); 
	g_signal_connect(G_OBJECT(screen_1), "button-press-event", G_CALLBACK(on_context_menu), NULL); 
	g_signal_connect(G_OBJECT(screen_2), "button-press-event", G_CALLBACK(on_context_menu), NULL); 
	g_signal_connect(G_OBJECT(screen_3), "button-press-event", G_CALLBACK(on_context_menu), NULL); 
	g_signal_connect(G_OBJECT(screen_4), "button-press-event", G_CALLBACK(on_context_menu), NULL); 
	g_signal_connect(G_OBJECT(screen_5), "button-press-event", G_CALLBACK(on_context_menu), NULL); 
	g_signal_connect(G_OBJECT(screen_6), "button-press-event", G_CALLBACK(on_context_menu), NULL); 
	g_signal_connect(G_OBJECT(screen_7), "button-press-event", G_CALLBACK(on_context_menu), NULL); 
	
	g_signal_connect(G_OBJECT(save_menu_item), "activate", G_CALLBACK(on_save_screen), NULL); 
	
	gtk_widget_show_all(window); 
	gtk_widget_show_all(main_screen_window); 
	
	gtk_widget_hide(screen_frame_2); 
	gtk_widget_hide(screen_frame_3); 
	gtk_widget_hide(screen_frame_4); 
	gtk_widget_hide(screen_frame_5); 
	gtk_widget_hide(screen_frame_6); 
	gtk_widget_hide(screen_frame_7); 
	
	gtk_widget_show(save_menu_item); 
	
	gtk_main(); 
	gdk_threads_leave(); 
	
	return 0; 
}
