
 public asm_ExecuteChannel;
 extern LastVals;
 extern samplerate;
 extern ??div32_a;
//inline short ExecuteChannel( CHANNEL * chptr )]

#define pos 0
#define repeat 4
#define end 8
#define freq 16
#define vol 24
#define voicemode 36
#define fractpos 20
#define LastVal 40

asm_ExecuteChannel:
#define SAMPLERATE R1
#define FRACTPOS R2
#define FREQ R3
#define VOICEMODE R4
#define POS R5
#define _END R6
#define LASTVAL R7
#define VOL R8

  push{r4-r8,lr}     
  //if ((!chptr->pos )|(!chptr->vol)) return ( 0 );
  ldr POS,[r0,#pos]
  cmp POS,#0
  BEQ Quit
  ldr VOL,[r0,#vol]
  cmp VOL,#0
  bne VolOk
Quit:
  eor r0,r0,r0
  pop {r4-r8,pc}
VolOk:  
  //
  
  ldr LASTVAL,[r0,#LastVal]  
  ldr FRACTPOS,[r0,#fractpos]
  ldr FREQ,[r0,#freq]
  ldr SAMPLERATE,=samplerate
  ldr SAMPLERATE,[SAMPLERATE]
  
 // chptr->fractpos += chptr->freq;
 ADD FRACTPOS,FREQ,FRACTPOS
Loop1:
  //if ( chfractpos >= sr )
  cmp FRACTPOS,SAMPLERATE
  ldrcc _END,[r0,#end]
  BCC QuitLoop1
  ldr _END,[r0,#end]
  //{ 
   // chptr->fractpos -= sr;
   sub FRACTPOS,FRACTPOS,SAMPLERATE
   
   // if ( chpos < chptr->end ) chptr->LastVal = * chpos;
   cmp POS,_END
   LDRCCSB LASTVAL,[POS,#0]
   
    //chpos++;
   ADD POS,POS,#1
   
    //if ( chpos >= chptr->end )
  //{//
      //CMP POS,_END
      //BLT NotEndSample
   // if ( chptr->voicemode==1 )return ( 0 );
      //ldr VOICEMODE,[r0,#voicemode]
      //cmp VOICEMODE,#1
      //eoreq r0,r0,r0
      //popeq{r4-r8,pc}
   //     chptr->pos = chptr->repeat;
     //LDR POS,[r0,#repeat]
  //  }
//NotEndSample:
  //goto Loop1
  B Loop1
  //}
QuitLoop1:

   //if ( chpos >= chptr->end )
  //{//
      CMP POS,_END
      BLT NotEndSample1
   // if ( chptr->voicemode==1 )return ( 0 );
      ldr VOICEMODE,[r0,#voicemode]
      cmp VOICEMODE,#1
      eoreq r0,r0,r0
      popeq{r4-r8,pc}
   //     chptr->pos = chptr->repeat;
     LDR VOICEMODE,[r0,#repeat]
     ADD POS,VOICEMODE,POS
     SUB POS,POS,_END
  //  }
NotEndSample1:
   
  STR FRACTPOS,[r0,#fractpos]
  STR POS,[r0,#pos]
  STR LASTVAL,[r0,#LastVal]
  //
  LDRSB R0,[POS,#0]
  //return ( ( ( * chpos * chfractpos ) + ( chptr->LastVal * ( sr - chfractpos ) ) )
  //     / sr * chptr->vol );
  
  //( * chpos * chfractpos )
  //;MUL R0,FRACTPOS,R0
  //( sr - chfractpos )
  SUB r6,SAMPLERATE,FRACTPOS        //R6=SAMPLERATE-FRACTPOS
  //( chptr->LastVal * ( sr - chfractpos )
  MUL LASTVAL,r6,LASTVAL            //LASTVAL=(SAMPLERATE-FRACTPOS)*LASTVAL
  //( ( * chpos * chfractpos ) + ( chptr->LastVal * ( sr - chfractpos ) )
  MLA R0,FRACTPOS,R0,LASTVAL        //R0=(FRACTPOS*Dat)+(SAMPLERATE-FRACTPOS)*LASTVAL
  //BL ??div32_a
  Mov r1,r0, asr #14
  add r0,r0,r1, lsr #17
  mov r0,r0, asr #15
  
  MUL R0,VOL,R1  
  pop{r4-r8,pc}