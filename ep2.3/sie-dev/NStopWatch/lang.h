int Langpack_Init(void);
void Langpack_Free(void);


#define LGP_COPYRIGHT "NStopWatch v%s\n(c)FIL\n(r)Ganster, SliGo\nCompiled %s"

enum LGP_ID
{
  LGP_LOADING_SCREEN,
  LGP_CSM_NAME,
  LGP_MSG_ERROR_OPEN_FILE,
  LGP_MSG_ERROR_RESULTS_FOR_SAVE,
  LGP_MSG_ERROR_SAVE,
  LGP_MSG_WRITE_RESULTS,
  LGP_MSG_REQ,
  LGP_CFM_ON_EXIT,
  LGP_TOTAL
};

extern char *LGP[LGP_TOTAL];
