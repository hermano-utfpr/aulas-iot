#include "contiki.h"
#include "dev/button-sensor.h"
#include "dev/sht11/sht11-sensor.h"
#include <stdio.h>
PROCESS(mote2_process, "Mote 2");
AUTOSTART_PROCESSES(&mote2_process);
PROCESS_THREAD(mote2_process, ev, data)
{
  static int val;
  PROCESS_BEGIN();
  SENSORS_ACTIVATE(button_sensor);
  SENSORS_ACTIVATE(sht11_sensor);
  PROCESS_WAIT_EVENT();
  val = sht11_sensor.value(SHT11_SENSOR_HUMIDITY);
  printf("Mote 2 de Taylor - umidade: %d\n",val);
  SENSORS_DEACTIVATE(button_sensor);
  SENSORS_DEACTIVATE(sht11_sensor);
  PROCESS_END();
} 