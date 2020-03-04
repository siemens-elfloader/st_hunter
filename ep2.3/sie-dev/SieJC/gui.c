#include "../inc/swilib.h"
#include "gui.h"


IMGHDR* background;
IMGHDR* cursor;

UI_STATE stateUI = NONE;

void setUI()
{
	extern const char PATH_TO_PIC[];
	char temp_path[128];
	unsigned int err;
	FSTATS stat;


	strcpy(temp_path, PATH_TO_PIC);
	strcat(temp_path, "bg.png");

	if (GetFileStats(temp_path, &stat, &err) != -1)
	{
		stateUI += 1;
		background = CreateIMGHDRFromPngFile(temp_path, 0);
	}

	strcpy(temp_path, PATH_TO_PIC);
	strcat(temp_path, "cursor.png");

	if (GetFileStats(temp_path, &stat, &err) != -1)
	{
		stateUI += 2;
		cursor = CreateIMGHDRFromPngFile(temp_path, 0);
	}

#ifdef ELKA
	if (background->h == 320)
	{
		char* map = malloc(296 * background->w * background->bpnum);
		zeromem(map, 296 * background->w * background->bpnum);

		memcpy(map, background->bitmap + CalcBitmapSize(background->w, 24, background->bpnum), CalcBitmapSize(background->w, 296, background->bpnum));
		zeromem(background->bitmap, CalcBitmapSize(background->w, background->h, background->bpnum));
		background->h = 296;
		memcpy(background->bitmap, map, CalcBitmapSize(background->w, 296, background->bpnum));
		mfree(map);
	}
#endif
}


void freeUI ()
{
	if(background)
	{
		if(background->bitmap)
			mfree(background->bitmap);
		mfree(background);
	}
	if (cursor)
	{
		if(cursor->bitmap)
			mfree(cursor->bitmap);
		mfree(cursor);
	}
}
