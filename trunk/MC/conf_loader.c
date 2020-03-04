#include <cfg_items.h>
#include <conf_loader.h>
#include "inc/mc.h"
#include "inc/config.h"
#include "inc/gui.h"

__CONFIG_EXTERN(0, cfghdr0)

char successed_config_filename[MAX_PATH] = "";

int SaveConfigData(const char *fname)
{
	int f;
	unsigned int ul;
	int result = -1;
	unsigned int len =(int)&__config_end0-(int)&__config_begin0;

	if ((f = _open(fname, A_ReadWrite+A_Create+A_Truncate+A_BIN, P_READ+P_WRITE, &ul)) != -1)
	{
		if (_write(f, (void*)&cfghdr0, len, &ul) == (int)len) result = 0;
		_close(f, &ul);
	}

	return(result);
}


int LoadConfigData(const char *fname)
{
	int f;
	unsigned int ul;
	char *buf;
	int result=0;
	void *cfg;
	unsigned int rlen, end;

	cfg=(char *)&cfghdr0;
	unsigned int len=(int)&__config_end0-(int)&__config_begin0;

	if (!(buf=malloc(len))) return -1;
	if ((f=_open(fname,A_ReadOnly+A_BIN,P_READ,&ul))!=-1)
	{
		rlen=_read(f,buf,len,&ul);
		end=_lseek(f,0,S_END,&ul,&ul);
		_close(f,&ul);
		if (rlen!=end || rlen!=len) goto L_SAVENEWCFG;
		memcpy(cfg,buf,len);
	}
	else
	{
L_SAVENEWCFG:
		// Прописываем дефолтный путь для mcconfig.cfg в папку mc
		strcpy((char*)CONFIG_MCCONFIG_PATH,mcpath);
                char grafic[128];
                sprintf(grafic, "%s\\Images\\Gui\\",mcpath);
                strcpy((char*)graf_folder, grafic);

#ifdef ELKA
                strcpy((char*)CONFIG_VIEW_PUT,mcpath);
                strcat((char*)CONFIG_VIEW_PUT,"\\MiniView.elf");
#endif
		// Сохраняем
		result = SaveConfigData(fname);
	}
	mfree(buf);
	return(result);
}

void InitTempDir()
{
	if (!CONFIG_TEMP_PATH[0])
		strcpy((char*)CONFIG_TEMP_PATH, DEFAULT_DISK":\\Temp");

	int pathLen = strlen((const char*)CONFIG_TEMP_PATH);
	if (pathLen > 0 && ((char*)CONFIG_TEMP_PATH)[pathLen - 1] == '\\')
		((char*)CONFIG_TEMP_PATH)[pathLen - 1] = 0;

	mktree((char*)CONFIG_TEMP_PATH);
}

void InitConfig()
{
  extern char *conf_name;
  if(strlen(successed_config_filename)==0)
  {
    sprintf(successed_config_filename, _s_sbcfg, etc_path, conf_name);
    // Сначала пробуем взять информацию с того диска с которого запускаемся
    successed_config_filename[0] = mcpath[0];
  }
  if (LoadConfigData(successed_config_filename) < 0)
  {
    // Пробуем с 0:
    successed_config_filename[0] = '0';
    if (mcpath[0] == '0' || LoadConfigData(successed_config_filename) < 0)
    {
      // Пробуем с 4:
      successed_config_filename[0] = '4';
      LoadConfigData(successed_config_filename);
    }
  }

	// Setup drives info
	Drives[DRV_IDX_Cache].enabled	= CONFIG_SHOW_SYSDRV;
	Drives[DRV_IDX_Config].enabled	= CONFIG_SHOW_SYSDRV;
	Drives[DRV_IDX_MMC].enabled	= CONFIG_SHOW_MMC;

	// Setup fonts
	NormalFont = FONT_SMALL;
	BoldFont = FONT_SMALL_BOLD;

	// Init temp dir
	InitTempDir();

	// Setup hidden/system files visibility
	show_hidden = CONFIG_SHOW_HIDDEN;
	show_system = CONFIG_SHOW_SYSTEM;
	show_hiddrv = CONFIG_SHOW_SYSDRV;
}
