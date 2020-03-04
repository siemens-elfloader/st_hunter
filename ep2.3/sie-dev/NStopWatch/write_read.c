#include "../inc/swilib.h"
#include "stopwatch.h"
#include "lang.h"

#define LEN 12 //длина строки результата

extern char *root_folder;

void WriteResults(void)
{
	extern const int CFG_MSG_NO_RESULTS, CFG_MSG_SAVE_RESULTS;
	if(res.total)
	{
		const char *spc="%sResults\\%02d%02d%02d_%02d%02d%02d.nsw";
		char *fname=malloc(strlen(root_folder)+strlen(spc)+1);
		unsigned int err;
		
		TTime time; TDate date;
		GetDateTime(&date, &time);
		sprintf(fname, spc, root_folder, date.year%2000, date.month, date.day, time.hour, time.min, time.sec);
		int fp=fopen(fname, A_WriteOnly+A_Create+A_TXT, P_WRITE, &err);
		mfree(fname);
		if(fp!=-1)
		{
			const int fsize=LEN*res.total;
			char *buf=malloc(fsize+1);
			char *tmp=malloc(LEN+1);
			for(int i=0; i<res.total; i++)
			{
				sprintf(tmp, "%02d:%02d:%02d.%02d\n", res.data[i].hour, res.data[i].min, res.data[i].sec, res.data[i].millisec);
				memcpy(buf+i*LEN, tmp, LEN);
			}
			buf[fsize]='\0';
			fwrite(fp, buf, fsize, &err);
			fclose(fp, &err);
			mfree(tmp);
			mfree(buf);
			if(CFG_MSG_SAVE_RESULTS) ShowMSG(1, (int)LGP[LGP_MSG_WRITE_RESULTS]);
		}
		else
		{
			MsgBoxError(1, (int)LGP[LGP_MSG_ERROR_SAVE]);
			return;
		}
	}
	else if(CFG_MSG_NO_RESULTS)
	MsgBoxError(1, (int)LGP[LGP_MSG_ERROR_RESULTS_FOR_SAVE]);
}

unsigned int offset_buffer(const char *buf)// 00:00:00.00
{
	int i=0;
	const int ar_max=9;
	while(i<=ar_max)
	{
		if(i!=ar_max)
		{
			if(buf[i]<'0' || buf[i]>'5') return i+1;
			if(buf[i+1]<'0' || buf[i+1]>'9') return i+2;
			if(i!=ar_max-3){if(buf[i+2]!=':') return i+3; }
			else if(buf[i+2]!='.') return i+3;
		}
		else
		{
			if(buf[i]<'0' || buf[i]>'9') return i+1;
			if(buf[i+1]<'0' || buf[i+1]>'9') return i+2;
		}
		i+=3;
	}
	return 0;
}
	
void parse_input_file(const char *buffer, const int bsize)
{
	
	int offset=0, buf_id=0;;
	TTime tmp;
	while(buf_id<=bsize)
	{
		if(buffer[buf_id]>='0' && buffer[buf_id]<='9')
		{
			offset=offset_buffer(buffer+buf_id);//return >0 error
			if(!offset)
			{	
				for(int i=0; i<3; i++)//str2TTime
					*(&tmp.hour+i*sizeof(char))=(buffer[buf_id+3*i]-'0')*10+(buffer[buf_id+3*i+1]-'0');
				tmp.millisec=(buffer[buf_id+9]-'0')*10+(buffer[buf_id+10]-'0');
				
				res.data=realloc(res.data, sizeof(TTime)*(res.total+1));
				memcpy(&res.data[res.total++], &tmp, sizeof(TTime));
				buf_id+=LEN-1;
			}
			else buf_id+=offset;
		}
		else buf_id++;
	}
}

int ReadResults(const char *filename)
{
	FSTATS fs;
	unsigned int err;
	if(GetFileStats(filename, &fs, &err)==-1) return -1;
	
	int fp=fopen(filename, A_ReadOnly+A_TXT, P_READ, &err);
	if(fp==-1) return -1;
	
	char *buffer=malloc(fs.size+1);
	int bsize=fread(fp, buffer, fs.size, &err);
	fclose(fp, &err);
	parse_input_file(buffer, bsize);
	mfree(buffer);
	
	if(!res.total) return -1;
	
	memcpy(&time, &res.data[res.total-1], sizeof(TTime));
	return 1;
}

