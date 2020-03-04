
#include "inc/mc.h"
#include "inc/mui.h"

const char mcbm_path[] = "Bookmarks";
const char mcbm_ext[] = "mcbm";

void SaveBM(char* name, char* path)
{
	if (pathbuf && name && path)
	{
		MCFilePath(mcbm_path);
		mktree(pathbuf);
		sprintf(pathbuf, _s_s, pathbuf, name);

		int f;
		if ((f=_open(pathbuf,A_ReadWrite+A_Create+A_Truncate,P_READ+P_WRITE,&err))!=-1)
		{
			_write(f,path,strlen(path)+1,&err);
			_close(f,&err);
		}
	}
}



void UseBM(char* filename)
{
  if (filename)
  {
    int f;
    if ((f=_open(filename,A_ReadOnly+A_BIN,P_READ,&err))!=-1)
    {
      if (_read(f, pathbuf, MAX_PATH, &err))
      {
        if(fexist(pathbuf) || isdir(pathbuf, &err))
        {
          if (curtab >= MAX_TABS)
	    curtab = back_tab < MAX_TABS ? back_tab : 0;

          if (isdir2(pathbuf, &err))
	    cd(curtab, pathbuf);
	  else
	  if (wsbuf)
	  {
	    str_2ws(wsbuf,pathbuf,MAX_PATH);
	    ExecuteFile(wsbuf,0,0);
	  }
        }else
          DoMsgBox(muitxt(ind_not_exist), CB_Del);
      }
      _close(f, &err);
    }
  }
}

void _AddBM(WSHDR *wsname)
{
	if (pathbuf && pathbuf2)
	{
		FILEINF *file = _CurFile();
		if (file)
		{
			char buf[MAX_PATH];
			ws_2str(wsname, buf, MAX_PATH);
			sprintf(pathbuf2, "%s.%s", buf, mcbm_ext);

			CurFullPathBM(buf, file->sname);
			SaveBM(pathbuf2, buf);
		}
	}
}

void DoBMAdd()
{
	FILEINF *file = _CurFile();
	if (file && wsbuf)
	{
		str_2ws(wsbuf,file->sname,MAX_PATH);
		TextInput(muitxt(ind_bm), muitxt(ind_name), 1, wsbuf, _AddBM);
	}
}

void DoBMList()
{
	if (pathbuf)
	{
		MCFilePath(mcbm_path);
		mktree(pathbuf);
		cdsys(pathbuf);
	}
}
