#include <swilib.h>
#include "pmb8876_reg.h"
#include "pmb8876_uart.h"

#define USART0 0
#define USART1 1


unsigned char usart_rx_byte=0;
char mes[256];

int il=10;

GBSTMR tmr;
RAMNET *data;

static void Killer(void)
{
  if(IsTimerProc(&tmr))GBS_StopTimer (&tmr);
  GBS_DelTimer(&tmr);
  kill_elf();
}


void TMRPROC()
{
  uart_poll_tx_string(USART0,"ŒÕÓ ∆€¬Œ≈≈!!!111_by Tonich");
  il--;
  if(il)GBS_StartTimerProc(&tmr, 216, TMRPROC);
  else SUBPROC((void *)Killer);
}

int main()
{
   uart_set_speed(USART0,USART_460800);
   uart_poll_tx_string(USART0,"//=========//========//");

   GBS_StartTimerProc(&tmr, 216, TMRPROC);


  return 0;
}
