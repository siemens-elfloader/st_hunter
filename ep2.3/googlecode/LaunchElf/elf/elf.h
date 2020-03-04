#ifndef ELF_H
#define ELF_H

/*typedef struct
{
  char name[16];//name shortcuts
  void* pointer;//function address
  int unk_zero;
  int unk_1;
  int lgp_full_name;//full name
  int unk_2;
  int unk_3;
  int lgp_short_name;//Short name
  int unk_4;
  int lgp_comment_msg;//help text for this shortcut, in the patch - the path to elf
}shortcut;*/

const char percent_t[]="%t";
const char percent_s[]="%s";
const char txt_null[]="";

char icon1[] = "0:\\Zbin\\img\\ELF_SMALL.png";//ýëelfüô
char icon2[] = "0:\\Zbin\\img\\UNK_SMALL.png";//øîShortcut
char icon3[] = "0:\\Zbin\\img\\ADD.png";      //adding a new elf

const char shortcuts_filename[]="0:\\Zbin\\etc\\shortcuts.dat";
const char shortcut_names_filename[]="0:\\Zbin\\etc\\shortcut_names.dat";
const char fast_run_filename[]="0:\\Zbin\\etc\\fr_shortcuts.dat";
const char shortcuts_last_name[]="XTRA_NO_APPLICA";
const char fast_run_last_name[]="XTRA_LAST_ENTRY";
const char newelfpath[]="0:\\Zbin\\LaunchElf.elf";

#endif
