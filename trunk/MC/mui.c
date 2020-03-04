#include "inc/mc.h"
#include "inc/lng.h"
#include "inc/mui.h"

const char mclg_deffile[]="default.mclg";
const char mclg_ext[]="mclg";

const MUI_STR mui[]=
{
    {id_no, sz_no},
    {id_yes, sz_yes},

    {id_byte, sz_byte},
    {id_kbyte, sz_kbyte},
    {id_mbyte, sz_mbyte},

    {id_reconfig, sz_reconfig},
    {id_lng, sz_lng},
    {id_lngname, sz_lngname},

    {id_dirmking, sz_dirmking},
    {id_copying, sz_copying},
    {id_moveing, sz_moveing},
    {id_deling, sz_deling},

    {id_mcmenu, sz_mcmenu},
    {id_select, sz_select},
    {id_back, sz_back},

    {id_open, sz_open},
    {id_file_m, sz_file_m},
    {id_chk, sz_chk},
    {id_chkall, sz_chkall},
    {id_invchk, sz_invchk},
    {id_unchall, sz_unchall},
    {id_oper_m, sz_oper_m},
    {id_past, sz_past},
    {id_cancel, sz_cancel},
    {id_copy, sz_copy},
    {id_move, sz_move},
    {id_del, sz_del},
    {id_rename, sz_rename},
    {id_newdir, sz_newdir},
    {id_prop, sz_prop},
    {id_drvinf, sz_drvinf},
    {id_view_m, sz_view_m},
    {id_sort_m, sz_sort_m},
    {id_sortn, sz_sortn},
    {id_sorte, sz_sorte},
    {id_sorts, sz_sorts},
    {id_sortd, sz_sortd},
    {id_sortr, sz_sortr},
    {id_filter, sz_filter},
    {id_refresh, sz_refresh},
    {id_newfile, sz_newfile},
    {id_bm_m, sz_bm_m},
    {id_bm, sz_bm},
    {id_add, sz_add},
    {id_bml, sz_bml},
    {id_misc_m, sz_misc_m},
    {id_settings, sz_settings},
    {id_about_m, sz_about_m},
    {id_exit, sz_exit},

    {id_name, sz_name},
    {id_fullname, sz_fullname},
    {id_size, sz_size},
    {id_global, sz_global},
    {id_folders, sz_folders},
    {id_files, sz_files},
    {id_date, sz_date},
    {id_attr, sz_attr},
    {id_readonly, sz_readonly},
    {id_hidden, sz_hidden},
    {id_system, sz_system},
    {id_arh, sz_arh},
    {id_subdirs, sz_subdirs},
    {id_ncsize, sz_ncsize},
    {id_csize, sz_csize},
    {id_cglobal, sz_cglobal},

    {id_pmt_del, sz_pmt_del},
    {id_pmt_copy, sz_pmt_copy},
    {id_pmt_move, sz_pmt_move},
    {id_pmt_stop, sz_pmt_stop},
    {id_pmt_cancel, sz_pmt_cancel},
    {id_pmt_exists, sz_pmt_exists},
    {id_pmt_exit, sz_pmt_exit},
    {id_pmt_impcs, sz_pmt_impcs},
    {id_pmt_implg, sz_pmt_implg},
    {id_pmt_rodel, sz_pmt_rodel},

    {id_err_attrset, sz_err_attrset},
    {id_err_makedir, sz_err_makedir},
    {id_err_rename, sz_err_rename},
    {id_err_delete, sz_err_delete},
    {id_err_badname_t, sz_err_badname_t},
    {id_err_baddrv_t, sz_err_baddrv_t},
    {id_err_resnok, sz_err_resnok},
    {id_err_nofiles, sz_err_nofiles},
    {id_err_badname, sz_err_badname},
    {id_err_badkey, sz_err_badkey},
    {id_err_badformat, sz_err_badformat},
    {id_err_nomemory, sz_err_nomemory},
    {id_err_makefile, sz_err_makefile},
    {id_err_ziperr, sz_err_ziperr},

    {id_msg_zreading, sz_msg_zreading},
    {id_msg_showhid_files, sz_msg_showhid_files},
    {id_msg_showsys_files, sz_msg_showsys_files},
    {id_msg_showhidsys_files, sz_msg_showhidsys_files},
    {id_msg_showhid_drv, sz_msg_showhid_drv},
    {id_altopen, sz_altopen},
    {id_natexpl, sz_natexpl},
    {id_keyhelp, sz_keyhelp},
    {id_bt, sz_bt},
    {id_empty, sz_empty},

    {id_drv_driver, sz_drv_driver},
    {id_drv_all, sz_drv_all},
    {id_drv_free, sz_drv_free},
    {id_drv_percent, sz_drv_percent},
    {id_pos, sz_pos},
    {id_fullpatch, sz_fullpatch},

    {id_ok, sz_ok},
    {id_cncl, sz_cncl},
    {id_time, sz_time},
    {id_resol, sz_resol},
    {id_artist, sz_artist},
    {id_title, sz_title},
    {id_album, sz_album},
    {id_year, sz_year},
    {id_bitrate, sz_bitrate},

    {id_viz, sz_viz},
    {id_hid_file, sz_hid_file},
    {id_sys_file, sz_sys_file},
    {id_hid_drv, sz_hid_drv},
    {id_do_arch, sz_do_arch},
    {id_create, sz_create},
    {id_find, sz_find},
    {id_arch, sz_arch},
    {id_pass, sz_pass},
    {id_arh_name, sz_arh_name},
    {id_info, sz_info},
    {id_support, sz_support},
    {id_drv_info, sz_drv_info},
    {id_drv_busy, sz_drv_busy},
    {id_gbyte, sz_gbyte},
    {id_err_search, sz_err_search},
    {id_sys_type, sz_sys_type},
    {id_hid_type, sz_hid_type},
    {id_type, sz_type},
    {id_in, sz_in},
    {id_met_compres, sz_met_compres},
    {id_no_compres , sz_no_compres},
    {id_hi_speed , sz_hi_speed},
    {id_default, sz_default},
    {id_hi_compress, sz_hi_compress},
    {id_noadd2arh, sz_noadd2arh},
    {id_thisdir, sz_thisdir},

    {id_getff, sz_getff},
    {id_rec, sz_rec},

    {id_wall, sz_wall},
    {id_logo, sz_logo},
    {id_ss, sz_ss},

    {id_call, sz_call},
    {id_mess, sz_mess},
    {id_alarm, sz_alarm},
    {id_org, sz_org},
    {id_set_ass, sz_set_ass},

    {id_mcvol, sz_mcvol},
    {id_mcsearch, sz_mcsearch},

    {id_treak, sz_treak},
    {id_coment, sz_coment},

    {id_md5, sz_md5},
    {id_md5_file, sz_md5_file},
    {id_cmd5, sz_cmd5},

    {id_corect, sz_corect},
    {id_no_corect, sz_no_corect},
    {id_ms, sz_ms},
    {id_not_exist, sz_not_exist}
};

#define MUI_COUNT (sizeof(mui)/sizeof(MUI_STR))
char* mui_ld[MUI_COUNT];

int getmuiind(int id)
{
    for(uint32_t cc=0; cc < MUI_COUNT; cc++)
        if (mui[cc].id == id) return cc;
    return -1;
}

char* muitxt(int ind)
{
    if (ind < 0 || ind >= (int)MUI_COUNT) return NULL;
    return mui_ld[ind] ? mui_ld[ind] : mui[ind].df_str;
}

void SaveMUI(char* filename)
{
    char* fn = filename?filename:MCFilePath(mclg_deffile);

    int f;
    if (fn && (f=_open(fn,A_ReadWrite+A_Create+A_Truncate,P_READ+P_WRITE,&err))!=-1)
    {
        MCLG_hdr hdr;
        hdr.sig=mclg_sig;
        hdr.ver=mclg_ver;
//hdr.size=mclg_size;
        hdr.offset=mclg_offset;

        int bufsz=0;
        for(uint32_t cc=0; cc<MUI_COUNT; cc++)
            bufsz+=strlen(muitxt(cc))+1;
        char* buff;
        char* lp;
        lp=buff=malloc(bufsz);

        hdr.size=mclg_size | (bufsz<<16);

        MC_LG* lgtbl;
        MC_LG* lg;
        lg=lgtbl=malloc(MUI_COUNT*sizeof(MC_LG));
        for(uint32_t cc=0; cc<MUI_COUNT; ++cc)
        {
            lg->id=mui[cc].id;
            lg->ofs=(int)lp-(int)buff;
            strcpy(lp, muitxt(cc));
            lp+=strlen(muitxt(cc))+1;
            lg=(MC_LG*)((int)lg+sizeof(MC_LG));
        }

        _write(f,&hdr,sizeof(hdr),&err);
        _write(f,lgtbl,MUI_COUNT*sizeof(MC_LG),&err);
        _write(f,buff,bufsz,&err);
        mfree(lgtbl);
        mfree(buff);
        _close(f,&err);
    }
}

void DoLang()
{
    int f;
    if ((f=_open(MCFilePath(mclg_deffile),A_ReadWrite+A_Create+A_Truncate,P_READ+P_WRITE,&err))!=-1)
    {
        char* buf = malloc(256);
        if (buf)
        {
            for(uint32_t cc=0; cc<MUI_COUNT; ++cc)
            {
                strncpy(buf, mui_ld[cc]?mui_ld[cc]:mui[cc].df_str,255);
                for(int ii=0; buf[ii]; ii++)
                    if (buf[ii]=='\n')buf[ii]='^';

                sprintf(pathbuf, "%.4d=%s\n", mui[cc].id, buf);
                _write(f,pathbuf,strlen(pathbuf),&err);
            }
            mfree(buf);
        }
        _close(f,&err);
    }
}

#ifdef DUMP_MUI
void SaveMUI2(char* filename)
{
    char* fn = filename?filename:MCFilePath(mclg_deffile);

    int f;
    if (fn && (f=_open(fn,A_ReadWrite+A_Create+A_Truncate,P_READ+P_WRITE,&err))!=-1)
    {
        char* buf = malloc(256);
        if (buf)
        {
            for(int cc=0; cc<MUI_COUNT; cc++)
            {
                strncpy(buf, mui_ld[cc]?mui_ld[cc]:mui[cc].df_str,255);
                for(int ii=0; buf[ii]; ii++)
                    if (buf[ii]=='\n')buf[ii]='^';

                sprintf(pathbuf, "%.4d=%s\n", mui[cc].id, buf);
                _write(f,pathbuf,strlen(pathbuf),&err);
            }
            mfree(buf);
        }
        _close(f,&err);
    }
}
#endif

char* muibuff;
int muibuffsz;

int LoadMUI(char* filename)
{
    int res = 0;
    int v1=0;
    char* fn = filename?filename:MCFilePath(mclg_deffile);

    int f;
    if (fn && (f=_open(fn,A_ReadOnly+A_BIN,P_READ,&err))!=-1)
    {
        MCLG_hdr hdr;
        if (_read(f, &hdr, sizeof(hdr), &err)==sizeof(hdr))
        {
            if (hdr.sig==mclg_sig)
            {
                _lseek(f,hdr.offset,S_SET,&err,&err);
                int tblcn=(hdr.size & 0xffff);
                int tblsz=tblcn*sizeof(MC_LG);
                int bufsz=hdr.size>>16;
                FreeMUI();
                muibuff=malloc(bufsz);
                muibuffsz=bufsz;
                MC_LG* tbl;
                MC_LG* lg;
                lg=tbl=malloc(tblsz);
                res =
                    (_read(f, tbl, tblsz, &err)==tblsz) &&
                    (_read(f, muibuff, bufsz, &err)==bufsz);
                if (res)
                {
                    LockSched();
                    for(int cc=0; cc<tblcn; cc++)
                    {
                        int ind=getmuiind(lg->id);
                        mui_ld[ind]=(char*)((int)muibuff + lg->ofs);
                        lg=(MC_LG*)((int)lg+sizeof(MC_LG));
                    }
                    UnlockSched();
                }
                else
                    mfree(muibuff);
                mfree(tbl);
            }
            else v1=1;
        }
        _close(f, &err);
    }
    int LoadMUIold(char* fname);
    if (v1)res=LoadMUIold(filename);
    else InitMUI();
    return res;
}

void freeb(char* buf)
{
    if (buf)
        if ((int)buf<(int)muibuff || (int)buf>=(int)muibuff+muibuffsz)
            mfree(buf);
}

void FreeMUI()
{
    for(uint32_t cc=0; cc<MUI_COUNT; ++cc)
        if (mui_ld[cc])
        {
            freeb(mui_ld[cc]);
            mui_ld[cc]=NULL;
        }
    if (muibuff)
    {
        mfree(muibuff);
        muibuff=0;
    }
}

void MUIProc(char *name, char *value)
{
    int id=strtol(name,0,10);
    int ind=getmuiind(id);
    if (ind<0)return;

    LockSched();
    freeb(mui_ld[ind]);
    char* buf = malloc(strlen(value)+1);
    strcpy(buf, value);
    for(int cc=0; buf[cc]; cc++)
        if (buf[cc]=='^')buf[cc]='\n';
    mui_ld[ind] = buf;
    UnlockSched();
}

int LoadMUIold(char* fname)
{
    int l;
    char* fn;
    if (fname)
    {
        l=0;
        fn=fname;
    }
    else
    {
        l=1;
        fn=(char*)&mclg_deffile;
    }
    int res = EnumIni(l, fn, MUIProc);
    InitMUI();
    return res;
}

void InitMUI()
{

}

