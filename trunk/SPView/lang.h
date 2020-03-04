int InitLangpack(void);
void FreeLangPack(void);


#define LGP_COPYRIGHT "SPView v%s\n(c)FIL\n(r)KreN\nCompiled %s"

enum LGP_ID
{
  lgp_options,
  lgp_exit,
  lgp_enter_number,
  lgp_save_bmp,
  lgp_about,
  lgp_select,
  lgp_back,
  lgp_pic_id,
  lgp_view,
  //lgp_path_not_found,
  lgp_save,
  lgp_first,
  lgp_last,
  lgp_saved,
  lgp_total
};

extern char *lgp[lgp_total];
