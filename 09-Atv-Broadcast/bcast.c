#include "contiki.h"
#include "net/rime/rime.h"
#include "dev/button-sensor.h"
#include "dev/leds.h" 
#include <stdio.h>
PROCESS(processo_broadcast, "Exemplo de Processo Broadcast");
PROCESS(processo_led, "Processoe led");
AUTOSTART_PROCESSES(&processo_broadcast,&processo_led);
static int pacote = 0;
static int recebeu = 0;
static void
broadcast_recv(struct broadcast_conn *c, const linkaddr_t *from)
{
    if(recebeu == 0) {
      printf("Mazepa - quadro broadcast recebido de %d.%d: '%s' \n", from->u8[0], from->u8[1], (char *)packetbuf_dataptr());
      recebeu = 1;
      pacote = 1;
    }
}
static const struct broadcast_callbacks broadcast_call = {broadcast_recv};
static struct broadcast_conn broadcast;

PROCESS_THREAD(processo_broadcast, ev, data)
{
  PROCESS_EXITHANDLER(broadcast_close(&broadcast);)
  PROCESS_BEGIN();
  broadcast_open(&broadcast, 129, &broadcast_call);
  SENSORS_ACTIVATE(button_sensor);
  while(1) {
    PROCESS_WAIT_EVENT();
    if((ev == sensors_event && data == &button_sensor)) {
       packetbuf_copyfrom("Teste", 5);
       broadcast_send(&broadcast);
       printf("Mazepa - quadro broadcast foi enviado \n");
    }
  }
  SENSORS_DEACTIVATE(button_sensor);
  PROCESS_END();
} 
PROCESS_THREAD(processo_led, ev, data)
{
  PROCESS_BEGIN();
  static struct etimer et;
  etimer_set(&et, CLOCK_SECOND * 2);
  while(1)
  {
     PROCESS_WAIT_EVENT();
     if (etimer_expired(&et))
     {
        if (pacote == 1) {
          leds_on(7);
	  pacote = 0; 
          packetbuf_copyfrom("Teste", 5);
          broadcast_send(&broadcast);
          printf("Mazepa - quadro broadcast foi enviado \n");
        } else {
	  leds_off(7); 
        }
        etimer_reset(&et);
     } 
    
  }
  PROCESS_END();
}
