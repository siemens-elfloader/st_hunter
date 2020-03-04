#ifdef E71Cv41
#define ADDR_GetCalleeNumber 0xA094D15E+1
#define ADR_PlaySound2  0xA05ABEA4
#endif
#ifdef S7Cv47
#define ADDR_GetCalleeNumber 0xA09833EE+1
#define ADR_PlaySound2  0xA0279E44
#endif
    CODE16
    EXTERN do_check
    RSEG BLACK_LIST_BODY
    PUBLIC GetCalleeNumber
    
GetCalleeNumber:
    PUSH    {R7,LR}
    LDR     R7, =ADDR_GetCalleeNumber
    BLX     R7
    POP     {R7,PC}
    
BODY:
    PUSH    {R0-R7,LR}
    BLX     do_check
    CMP     R0, #1       //此处或者下一句取反可以得到白名单
    BNE     RP
    MOV     R0, #0
    SWI     0x66  //EndCallBusy
    B       EXIT
RP:
    POP     {R0-R7}
    LDR     R7,=ADR_PlaySound2
    BLX     R7
    //SWI     0x33 //PlaySound2
    POP     {PC}
EXIT:
    POP     {R0-R7,PC}
    /*
NONAME:
    PUSH    {R0-R7,LR}
    MOV     R0, #1
    BL      get_it
    CMP     R0, #0
    BEQ     EXITNOME
    MOV     R0, #0
    //LDR     R3, =ADR_EndCallBusy
    //BLX     R3
    SWI     0x66  //EndCallBusy
EXITNOME:
    POP     {R0-R7,PC}*/
    
    CODE32
    RSEG BLACKLIST_DUMP
BLACK_LIST_DUMP:
    LDR     R12, =BODY
    BX      R12
    
    CODE16
    RSEG RHOOKBLACKLIST:CODE(1)
    BLX     BLACK_LIST_DUMP
    
    END