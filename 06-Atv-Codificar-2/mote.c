#include "contiki.h" 
#include "dev/leds.h" 
#include "dev/button-sensor.h"
#include "dev/light-sensor.h"
#include "dev/sht11/sht11-sensor.h"
#include <stdio.h> 
PROCESS(process_timer, "Processo com Timer"); 
PROCESS(process_button, "Processo com Button"); 
AUTOSTART_PROCESSES(&process_timer,&process_button); 

static int ledsact = 1; 
static int nextled (int ledval)
{ 
    static int nextled;
    SENSORS_ACTIVATE(light_sensor);
    ledval = light_sensor.value(LIGHT_SENSOR_TOTAL_SOLAR); 
   
    if (ledval >= 0 && ledval <= 255) {
       nextled = 2; 
    } else if (ledval >= 256 && ledval <= 511) { 
       nextled = 1; 
    } else if (ledval >= 512 && ledval <= 767) { 
       nextled = 4; 
    } else if (ledval >= 768 && ledval <= 1023) { 
       nextled = 0; 
    } 
    return nextled;
}

PROCESS_THREAD(process_timer, ev, data) 
{ 
  PROCESS_BEGIN(); 
  PROCESS_PAUSE(); 
  static int ledatual = 0;
  static int lumin = 0;
  static struct etimer et; 
  etimer_set(&et, CLOCK_SECOND * 1);  
  leds_off(7); 
  while (1) {
     PROCESS_WAIT_EVENT(); 
     if (etimer_expired(&et))  {
        if (ledsact) { 
           SENSORS_ACTIVATE(light_sensor);
           lumin = light_sensor.value(LIGHT_SENSOR_TOTAL_SOLAR);
           leds_off(ledatual); 
           ledatual = nextled(ledatual);
           leds_on(ledatual); 
           printf("Luminosidade: %d; Led: %d\n",lumin,ledatual); 
        }
     etimer_reset(&et); 
     } 
  } 
  PROCESS_END(); 
} 
PROCESS_THREAD(process_button, ev, data) 
{ 
  PROCESS_BEGIN(); 
  PROCESS_PAUSE(); 
  SENSORS_ACTIVATE(button_sensor); 
  while (1) { 
    PROCESS_WAIT_EVENT(); 
    if(ev == sensors_event && data == &button_sensor) 
    { 
       if (ledsact) { 
          ledsact = 0; 
          leds_off(7); 
          printf("Taylor - Button = LEDs act off\n"); 
       } else { 
          ledsact = 1; 
          printf("Taylor - Button = LEDs act on\n"); 
       } 
    } 
  } 
  SENSORS_DEACTIVATE(button_sensor); 
  PROCESS_END(); 
} 
