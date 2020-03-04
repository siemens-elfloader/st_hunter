用于65机型支持半透明色的东西，ELFLoader部分未获得源码

DRAW_HOOK / Capture of function DrawObject
(c)Dimadze
(!)To complete all work needed ELFPAck 2.3 24bit+alpha!
V1.0

Accessible:
-------------------------------------------
CX75v25 - (it is just a test)
SL65v53 - (Everything works!)
CX70v56 - (No frame addresses)
S65v58  - (not tested)
-------------------------------------------

ELFPack and 2.3 24bit+alpha for SGold X65 it is possible to take from here \elfpacks\

You can put a full VKP\MODELvFIRMWARE\FF.bin for automatic calculation of the old data for patch

For example:
MODELvFIRMWARE - CX75v25

Description.
patch,follows for function DrawObject(), through which rendered virtually everything on the Siemens,
ie when a parameter contains a pointer to a fifth object rendering (`IMGHDR + RECT), patch checks 
on the alpha channel, if it is not (bpnum! = 0x0A), all rendered in the standard way, otherwise 
in the case enters samopisnaya piano Ia drawing, she made the most optimal. 
At the core of its work is recording bitmap images directly into the intermediate buffer rendering, 
, I called him RamScreenPhoneCache (is similar to Java) c computation of color in the alpha - channel. 
As a result, at SGold X65 can otrisovyvtsya 32 bit (24bit + alpha) image in the elves, as well as change
schedule using all 32 bits instead of 8 and 16.
Even on the CX75 visible difference to the standard seen "divorce", with a patch image is drawn very clearly.
Speed draw with patch and without, in my observation not menyaetsya.

At X75 in media player (standard viewer) can sometimes be a "split", reducing, distortion of the image color
(All at once), the reason for this is clear, but this bug is not eliminated, at X65 like this all right.

Why on X65 need spetsalny ELFPack.
The fact that support for loading 32 bit bitmap to a png X65 spetsalno cut in ELFPack 2.3,
because the phone all the same can not draw it, why too much space to hold true is it?
But now the phone has learned, and correspondingly he needs ELFPack 24 bit + alpha.
 
How to port.
The project is set up and ready to be compiled
I have already made 3 configuration, CX75v25, SL65v53, CX70v56, S65v58
For CX75v25, SL65v53 are all set, all by MySQL and it works for the 2 remaining until непонятки
We must find RamScreenPhoneCache in file defines.h

When porting should look at these 3x faila folder \config\:
 
 - MODELswFIRMWARE.xcl     (CX75sw25.xcl)
 - sub_MODELswFIRMWARE.asm (sub_CX75sw25.asm)
 - defines.h

//MODELswFIRMWARE.xcl
PATCH_BODY,CODE ... - a blank space under the body of the patch
PATCH_DRAWOBJECT    - address f-uu DrawObject() (be sure to read the note in drawhook.asm!)

//sub_MODELswFIRMWARE.asm
oldDrawObj_sub_1    - this address takes you to the first team BL f-uu DrawObject() 
oldDrawObj_sub_2    - this address is a transition in the second team BL f-uu DrawObject() 

//defines.h
RamScreenPhoneCache - address of beginning of intermediate buffer of rendering (not RamScreenBuffer!) 

For its location a full opening of IDA, go to the address of f-uu DrawObject
(Take out swilib.vkp, perhaps they will be there a few, take any)
Dizassemblim (THUMB), videm first available command BL, turn on her, look around
find command LDR c loaded into register RAM-addresses found?
Turn ArmDebugger, go to this address, see the pointer, the debugger it is highlighted in yellow
turn (just click on it), now in a position we take this address(At CX75v25: 0xA84AE7B8)
and add to it 0xAC (Well this is on CX75v25, and what have you got there, I xs), obtain 0xA84AE7B8+0xAC = 0xA84AE864.
This is address of the buffer itself. If you turn on Monitor (M) ?ArmDebugger,
and on the phone to put a white background, white headband, anything, we'll see an array of bytes 0xFF,
where he started there and the initial buffer address.
True to SL65v53 I added not  0xAC, but 0x1C5B4, something like this ...


