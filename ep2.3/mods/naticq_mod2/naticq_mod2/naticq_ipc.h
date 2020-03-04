#define IPC_NATICQ_NAME "NatICQ"
#define IPC_CHECK_DOUBLERUN 1
#define IPC_SMILE_PROCESSED 2
#define IPC_XSTATUSIMG_PROCESSED 3
#define IPC_TENSECONDS 4
#define IPC_SENDMSG 5
#define IPC_ONESECOND 6

typedef struct{
  unsigned long uin;
  char *msg;
}IPCMsg;
