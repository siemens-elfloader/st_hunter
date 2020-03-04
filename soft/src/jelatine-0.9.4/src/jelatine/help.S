.globl kill_elf

kill_elf:
  swi  0x8014  // malloc_addr
  mov  r1, r0
  ldr  r0, .ex
  bx   r1

.ex:
  .word __ex

// Cheat =)

.globl sie_start_thread

// R0 - main thread function
sie_start_thread:
  push {lr}
  str  sp, .th_data
  mov  r1, r0
  mov  r0, #4
  add  r0, pc
  bl   atexit
  blx  r1

sie_thread_end:
  ldr  sp, .th_data
  pop  {lr}
  bx   lr

.th_data:
  .word 0x00000000
