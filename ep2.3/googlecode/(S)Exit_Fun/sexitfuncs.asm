  RSEG  DATA_N

defadr  MACRO a,b
  PUBLIC  a
a EQU b
  ENDM

  RSEG  CODE:CODE
  
  //SGold:
  //CX75v12
  defadr  StoreErrInfoAndAbort_cx75v12,0xA07CADB0
  defadr  StoreErrString_cx75v12,0xA07CABD4
  //CX75v13
  defadr  StoreErrInfoAndAbort_cx75v13,0xA07CBA70
  defadr  StoreErrString_cx75v13,0xA07CB894
  //CX75v25 
  defadr  StoreErrInfoAndAbort_cx75v25,0xA07CD184
  defadr  StoreErrString_cx75v25,0xA07CCFA8
  //M75v25
  defadr  StoreErrInfoAndAbort_m75v25,0xA07CC610
  defadr  StoreErrString_m75v25,0xA07CC434
  //C72v22
  defadr  StoreErrInfoAndAbort_c72v22,0xA11309A8
  defadr  StoreErrString_c72v22,0xA11307CC
  //C75v22
  defadr  StoreErrInfoAndAbort_c75v22,0xA07811DC
  defadr  StoreErrString_c75v22,0xA0781000
  //CX70v56
  defadr  StoreErrInfoAndAbort_cx70v56,0xA113BE98
  defadr  StoreErrString_cx70v56,0xA113BCBC
  //S65v58
  defadr  StoreErrInfoAndAbort_s65v58,0xA113BE98
  defadr  StoreErrString_s65v58,0xA113BCBC
  //SK65v50
  defadr  StoreErrInfoAndAbort_sk65v50,0xA0946598
  defadr  StoreErrString_sk65v50,0xA09463BC
  //SL65v53
  defadr  StoreErrInfoAndAbort_sl65v53,0xA121CFC0
  defadr  StoreErrString_sl65v53,0xA121CDE4
  
  //NewSGold:
  //S68v47
  defadr  StoreErrInfoAndAbort_s68v47,0xA018D9E8
  defadr  StoreErrString_s68v47,0xA018D8B4
  //S7Cv47
  defadr  StoreErrInfoAndAbort_S7Cv47,0xA01CF2E8
  defadr  StoreErrString_S7Cv47,0xA01CF1B4
  //S75v47
  defadr  StoreErrInfoAndAbort_s75v47,0xA01CEDBC
  defadr  StoreErrString_s75v47,0xA01CEC88
  //S75v52
  defadr  StoreErrInfoAndAbort_s75v52,0xA01CEE50
  defadr  StoreErrString_s75v52,0xA01CED1C
  //SL75v47
  defadr  StoreErrInfoAndAbort_sl75v47,0xA01CEDBC
  defadr  StoreErrString_sl75v47,0xA01CEC88
  //SL75v52
  defadr  StoreErrInfoAndAbort_sl75v52,0xA01CEE50
  defadr  StoreErrString_sl75v52,0xA01CED1C
  //C81v51
  defadr  StoreErrInfoAndAbort_c81v51,0xA01CE3F8
  defadr  StoreErrString_c81v51,0xA01CE2C0
  //M81v51
  defadr  StoreErrInfoAndAbort_m81v51,0xA01CE44C
  defadr  StoreErrString_m81v51,0xA01CE318
  
  //NewSGold (E/EL71):
  //E71v45
  defadr  StoreErrInfoAndAbort_e71v45,0xA04CCDD4
  defadr  StoreErrString_e71v45,0xA04CCCA0
  //EL71v41
  defadr  StoreErrInfoAndAbort_el71v41,0xA04D2304
  defadr  StoreErrString_el71v41,0xA04D21D0
  //EL71v45 
  defadr  StoreErrInfoAndAbort_el71v45,0xA04D319C
  defadr  StoreErrString_el71v45,0xA04D3068
  
  
  
  END