#include "contiki.h"
#include "dev/light-sensor.h"
#include "dev/button-sensor.h"
#include "dev/sht11/sht11-sensor.h"
#include <stdio.h>
#include <math.h>
PROCESS(mote3_process, "Mote 3");
AUTOSTART_PROCESSES(&mote3_process);
PROCESS_THREAD(mote3_process, ev, data)
{
  static int val;
  PROCESS_BEGIN();
  SENSORS_ACTIVATE(button_sensor);
  SENSORS_ACTIVATE(light_sensor);
  PROCESS_WAIT_EVENT();
  val = light_sensor.value(LIGHT_SENSOR_TOTAL_SOLAR);
  printf("Mote 3 de Taylor - luminosidade: %d\n",val);
  SENSORS_DEACTIVATE(button_sensor);
  SENSORS_DEACTIVATE(light_sensor);
  PROCESS_END();
} 