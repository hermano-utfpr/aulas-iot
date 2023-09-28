#include "contiki.h"
#include "net/rime/rime.h"
#include "dev/button-sensor.h"
#include <stdio.h>
PROCESS(processo_emissor, "Exemplo Unicast Emissor");
AUTOSTART_PROCESSES(&processo_emissor);
static void recv_uc(struct unicast_conn *c, const linkaddr_t *from) {
}
static void sent_uc(struct unicast_conn *c, int status, int num_tx) {
  const linkaddr_t *dest = packetbuf_addr(PACKETBUF_ADDR_RECEIVER);
  if(linkaddr_cmp(dest, &linkaddr_null)) {
    return;
  }
  printf("Mote Emissor 2 de Mazepa: quadro unicast enviado para %d.%d: status %d num_tx %d \n", dest->u8[0], dest->u8[1], status, num_tx);
}
static const struct unicast_callbacks unicast_callbacks = {recv_uc, sent_uc};
static struct unicast_conn uc;
PROCESS_THREAD(processo_emissor, ev, data)
{
  PROCESS_EXITHANDLER(unicast_close(&uc);)
  PROCESS_BEGIN();
  unicast_open(&uc, 146, &unicast_callbacks);
  SENSORS_ACTIVATE(button_sensor);
  while(1) {
    linkaddr_t addr;
    PROCESS_WAIT_EVENT();
    if(ev == sensors_event && data == &button_sensor) {
       packetbuf_copyfrom("Teste", 5);
       addr.u8[0] = 3;
       addr.u8[1] = 0;
       if(!linkaddr_cmp(&addr, &linkaddr_node_addr)) {
         unicast_send(&uc, &addr);
       }
    }
  }
  SENSORS_ACTIVATE(button_sensor);
  PROCESS_END();
} 