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

//defining vrbls:

#define msrv_pin  3                 //Servo pin
#define addr      12                //EEPROM addrs to put last runtime

int angl = 0;                       //Servo angle

unsigned long runtime = 0;          //Arduino runtime
unsigned long recv_epdata = 0;      //recvd last runtime data from EEPROM
unsigned long fed_prvtime = 0;      //prev runtime at fed state

//------------------------EDIT FROM HERE------------------------

unsigned long eat_dly = 6 * 1000;   //delay between fed period(in ms)
int angl_set = 180;                 //servo rotation (in degree)
int dly_rot = 1;                    //speed of rotation (delay(ms) per degree)

//---------------------END OF EDITING AREA--------------------------

void setup() {
  mserv.attach(msrv_pin);           //initialize srvo at slcted pin
  mserv.write(0);                   //rst servo angle pos to 0

  EEPROM.get(addr, recv_epdata);    //load runtime bfr last shtdwn

  //For debugging purpose, open serial monitor
  Serial.begin(9600);
  Serial.println("Welcome!!!\nservo controlled fish feeder");
  Serial.println("prog ver alpha\n\nOld runtime data recvd....");
  Serial.print(recv_epdata);
  Serial.print(" ms or ");
  Serial.print(recv_epdata / 1000);
  Serial.println(" s\n");
}

void loop() {
  runtime = millis() + recv_epdata;             //delay w.out delay function, so another process can run
  EEPROM.put(addr, runtime - fed_prvtime);      //put runtime to EEPROM, data is safe from poweroff/reset

  if ((runtime - fed_prvtime) >=
    (eat_dly + (2 * dly_rot * angl_set))) {     //anthr part of delay w.out delay function
      Serial.println("mserv rotating...");      //for debugging purpose
  
      //rotating process:
      for (angl = 0; angl <= angl_set; angl++) {
        mserv.write(angl);
        delay(dly_rot);
      }
  
      for (angl = angl_set; angl >= 0; angl--) {
        mserv.write(angl);
        delay(dly_rot);
      }
  
      fed_prvtime = runtime;                     //anthr part of delay w.out delay function
  }
}
