# AD08 - Comunicação entre Nós IoT (UCast)

Objetivo: acender/apagar os leds de um mote vizinho utilizando quadros em unicasting. 

**Passo 1**

Instale a ferramenta [Cooja](https://docs.contiki-ng.org/en/develop/doc/tutorials/Running-Contiki-NG-in-Cooja.html).

**Passo 2 - Codificar os motes emissor e receptor**

Antes, crie o arquivo Makefile ativando o modo RIME.

-> Makefile

```
CONTIKI = /opt/contiki/
all: emissor receptor
CONTIKI_WITH_RIME = 1
include $(CONTIKI)/Makefile.include
```

Crie o código do emissor, que enviará o quadro em Unicast:

-> emissor.c

```
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
  printf("Mote Emissor de Fulano: quadro unicast enviado para %d.%d: status %d num_tx %d (barra-n)", dest->u8[0], dest->u8[1], status, num_tx);
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
       addr.u8[0] = 2;
       addr.u8[1] = 0;
       if(!linkaddr_cmp(&addr, &linkaddr_node_addr)) {
         unicast_send(&uc, &addr);
       }
    }
  }
  SENSORS_DEACTIVATE(button_sensor);
  PROCESS_END();
}
```

Crie o código do nó receptor, que receberá o quadro via Unicast:

-> receptor.c

```
#include "contiki.h"
#include "net/rime/rime.h"
#include <stdio.h>
PROCESS(processo_receptor, "Exemplo Unicast Receptor");
AUTOSTART_PROCESSES(&processo_receptor);
static void recv_uc(struct unicast_conn *c, const linkaddr_t *from) {
  printf("Mote Receptor de Fulano: quadro recebido de %d.%d (barra-n)",from->u8[0], from->u8[1]);
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
```

Substitua "fulano".

Compile:

`$ make TARGET=z1`

Ajuda: os códigos foram baseados nos exemplos de `/opt/contiki/examples/rime/`.

Teoria: [MAC Protocols in ContikiOS](http://anrg.usc.edu/contiki/index.php/MAC_protocols_in_ContikiOS)

**Passo 3 - Criar uma simulação e executar os motes com o Cooja:**

Crie o Mote 1 (Z1) com o código emissor.c -> será endereçado como 1.0

Crie o Mote 2 (Z1) com o código receptor.c -> será endereçado como 2.0

Execute a simulação e note que quando você clicar com o Button no Mote 1, o mesmo irá enviar um quadro unicast para o Mote 2. (Veja os quadros capturados em: Tools-> Radio Messages-> Analyzer -> 6LoWPAN).

Tire o Mote 1 do alcance do Mote 2 e faça o teste novamente.

**Passo 4 - Resolva:**

Codifique 2 motes emissores e 2 motes receptores:
- Ao clicar o botão do mote 1 o mesmo irá enviar um quadro unicast para que o mote 3 alterne (toggle) os status dos LEDs.
- Ao clicar o botão do mote 2 o mesmo irá enviar um quadro unicast para que o mote 4 alterne (toggle) os status dos LEDs.

Execute no simulador Cooja.

<details><summary>Resolução:</summary>

Resolução por [Matheus Mazepa](https://www.linkedin.com/in/matheus-mazepa/)

[emissor.c](emissor.c)

[receptor.c](receptor.c)

[emissor2.c](emissor2.c)

[receptor2.c](receptor2.c)

[Contiki - Cooja - Atividade Unicast](https://www.youtube.com/watch?v=Cg63r_fLAlM)

</details>



