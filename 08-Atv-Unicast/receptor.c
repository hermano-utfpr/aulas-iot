#include "contiki.h"
#include "net/rime/rime.h"
#include "dev/leds.h" 
#include <stdio.h>
PROCESS(processo_receptor, "Exemplo Unicast Receptor");
AUTOSTART_PROCESSES(&processo_receptor);
static void recv_uc(struct unicast_conn *c, const linkaddr_t *from) {
  if (from->u8[0] == 2) {
    leds_toggle(7); 
    printf("Mote Receptor 1 de Mazepa: quadro recebido de %d.%d \n", from->u8[0], from->u8[1]);
  }
}
static void sent_uc(struct unicast_conn *c, int status, int num_tx) {
}
static const struct unicast_callbacks unicast_callbacks = {recv_uc, sent_uc};
static struct unicast_conn uc;
PROCESS_THREAD(processo_receptor, ev, data)
{
  PROCESS_EXITHANDLER(unicast_close(&uc);)
  PROCESS_BEGIN();
  unicast_open(&uc, 146, &unicast_callbacks);
  while(1) {
    static struct etimer et;
    etimer_set(&et, CLOCK_SECOND * 1);
    PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&et));
  }
  PROCESS_END();
} 