# AD09 - Comunicação entre Nós IoT (BCast)

Objetivo: acender/apagar os leds de diversos motes vizinhos utilizando quadros em broadcasting. 

**Passo 1**

Instale a ferramenta [Cooja](https://docs.contiki-ng.org/en/develop/doc/tutorials/Running-Contiki-NG-in-Cooja.html).

**Passo 2 - Codificar os Motes**

Antes, crie o arquivo Makefile ativando o modo RIME.

-> Makefile

```
CONTIKI = /opt/contiki/
all: bcast
CONTIKI_WITH_RIME = 1
include $(CONTIKI)/Makefile.include
```

Apenas um código será necessário para os motes:

-> bcast.c

```
#include "contiki.h"
#include "net/rime/rime.h"
#include "dev/button-sensor.h"
#include <stdio.h>
PROCESS(processo_broadcast, "Exemplo de Processo Broadcast");
AUTOSTART_PROCESSES(&processo_broadcast);
static void
broadcast_recv(struct broadcast_conn *c, const linkaddr_t *from)
{
  printf("Fulano - quadro broadcast recebido de %d.%d: '%s' \n", from->u8[0], from->u8[1], (char *)packetbuf_dataptr());
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
    if(ev == sensors_event && data == &button_sensor) {
       packetbuf_copyfrom("Teste", 5);
       broadcast_send(&broadcast);
       printf("Fulano - quadro broadcast foi enviado!\n");
    }
  }
  SENSORS_DEACTIVATE(button_sensor);
  PROCESS_END();
}
```

Substitua "fulano".

Compile:

`$ make TARGET=z1`

Ajuda: os códigos foram baseados nos exemplos de `/opt/contiki/examples/rime/`.

**Passo 3 - Criar uma simulação e executar os motes com o Cooja:**

Faça uma simulação com 10 motes ou mais utilizando o mesmo código. Clique o botão de um mote e note que somente os nós que estão no alcance irão receber o quadro em broadcast.

**Passo 4 - Resolva:**

Crie uma simulação semelhante ao que aparece em vídeo (não precisa ser exatamente igual, apenas utilize quadros em broadcast):

[Exemplo Broadcast com Contiki](https://www.youtube.com/watch?v=nAKWFW45K_Q)

<details><summary>Resolução:</summary>

Resolução por [Matheus Mazepa](https://www.linkedin.com/in/matheus-mazepa/):

[bcast.c](bcast.c)

[Contiki - Cooja - Atividade Broadcast](https://www.youtube.com/watch?v=hcGXuauxFj8)

</details>


