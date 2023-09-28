#include "contiki.h"
#include "dev/button-sensor.h"
#include "dev/sht11/sht11-sensor.h"
#include <stdio.h>
PROCESS(mote1_process, "Mote 1");
AUTOSTART_PROCESSES(&mote1_process);
PROCESS_THREAD(mote1_process, ev, data)
{
  static int val;
  PROCESS_BEGIN();
  SENSORS_ACTIVATE(button_sensor);
  SENSORS_ACTIVATE(sht11_sensor);
  PROCESS_WAIT_EVENT();
  val = sht11_sensor.value(SHT11_SENSOR_TEMP);
  printf("Mote 1 de Taylor - temperatura: %d\n",val);
  SENSORS_DEACTIVATE(button_sensor);
  SENSORS_DEACTIVATE(sht11_sensor);
  PROCESS_END();
} 