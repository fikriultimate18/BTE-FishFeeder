#include <Servo.h>
#include <EEPROM.h>

/*  GREETINGS!
 *  HELO HELOW WK....
 *  
 *  Before using this source code, pls install the servo library from:
 *  https://github.com/arduino-libraries/Servo
 *  
 *  Pinout:
 *  
 *  Servo                     Arduino
 *  +5V (RED CABLE) --------> +5V
 *  GND (BROWN CABLE) ------> GND
 *  PWM (ORANGE CABLE) -----> Digital Pin 3
 * 
 *  Cheers!
 *  
 *  friansh.2k19
 */

Servo mserv;

#define msrv_pin 3
#define addr 12

uint8_t pos = 0;

unsigned long runtime = 0;
unsigned long recv_epdata = 0;
unsigned long mkn_prvtime = 0;

//------------------------EDIT FROM HERE------------------------

unsigned long eat_dly = 6000;   //delay between fed period(in ms)
uint8_t angl = 180;             //servo rotation (in degree)
uint8_t dly_rot = 1;            //speed of rotation (delay(ms) per degree)

//---------------------END OF EDITING AREA--------------------------

void setup() {
  mserv.attach(msrv_pin);
  mserv.write(0);

  EEPROM.get(addr, recv_epdata);    //load runtime bfr last shtdwn

  //For debugging purpose, open serial monitor
  Serial.begin(9600);
  Serial.println("Getting old clock data....");
  Serial.print(recv_epdata);
  Serial.print(" or ");
  Serial.print(recv_epdata / 1000);
  Serial.println(" s");
}

void loop() {
  runtime = millis() + recv_epdata;
  EEPROM.put(addr, runtime - mkn_prvtime);

  if ((runtime - mkn_prvtime) >= (eat_dly + (2 * dly_rot * angl))) {
    for (pos = 0; pos <= angl; pos++) {
      mserv.write(pos);
      delay(dly_rot);
    }

    for (pos = angl; pos >= 0; pos--) {
      mserv.write(pos);
      delay(dly_rot);
    }

    mkn_prvtime = runtime;
  }


}
