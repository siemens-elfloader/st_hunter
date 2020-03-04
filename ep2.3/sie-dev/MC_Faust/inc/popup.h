extern int msgopen;
extern int smsgopen;
void DoMsgBox(const char *lgpid, void CallFunc(int));
void MsgBoxClose();
void DrawMsgBox();
void DoMsgBoxKey(int key);

void DoShowMsg(const char *lgpid);
void ShowMsgClose();
void DoShowMsgKey(int key);
void DrawShowMsg();
