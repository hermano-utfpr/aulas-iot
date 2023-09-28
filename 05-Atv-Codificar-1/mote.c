#include "contiki.h"
#include "dev/button-sensor.h"
#include "dev/sht11/sht11-sensor.h"
#include <stdio.h>
PROCESS(t1_process, "Mote T1");
PROCESS(t2_process, "Mote T2");
AUTOSTART_PROCESSES(&t1_process,&t2_process);

static int contador;

PROCESS_THREAD(t1_process, ev, data)
{
  PROCESS_BEGIN();
  static int val_temp, val_umid;
  SENSORS_ACTIVATE(button_sensor);
  SENSORS_ACTIVATE(sht11_sensor);
      
  while(1)
  {
    PROCESS_WAIT_EVENT();
    if(ev == sensors_event && data == &button_sensor)
    {
      contador = contador + 1;
      val_temp = sht11_sensor.value(SHT11_SENSOR_TEMP);
      val_umid = sht11_sensor.value(SHT11_SENSOR_HUMIDITY);
      printf("Contador %u da Thread 1 de Taylor! - temperatura: %d, umidade: %d\n", contador, val_temp, val_umid);
    }
  }
  SENSORS_DEACTIVATE(button_sensor);
  PROCESS_END();
}

PROCESS_THREAD(t2_process, ev, data)
{
  PROCESS_BEGIN();
  static int val_temp, val_umid;
  static struct etimer et;
  etimer_set(&et, CLOCK_SECOND * 10);
  while (1)
  {
     PROCESS_WAIT_EVENT();
     if (etimer_expired(&et))
     {
        contador = contador + 1;
        val_temp = sht11_sensor.value(SHT11_SENSOR_TEMP);
        val_umid = sht11_sensor.value(SHT11_SENSOR_HUMIDITY);
        printf("Contador %u da Thread 2 de Taylor! - temperatura: %d, umidade: %d\n", contador, val_temp, val_umid);        
        etimer_reset(&et);
     }
  }
  PROCESS_END();
}
