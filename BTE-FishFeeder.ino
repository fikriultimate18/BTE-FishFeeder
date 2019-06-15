#include <Servo.h>
#include <EEPROM.h>

Servo servoaku;  

int addr = 12;
int pos = 0;   
long runtime = 0;
long recv_epdata = 0;
long mkn_prvtime = 0;

//--------------------SETTING DARI SINI--------------------

unsigned long selang_makan = 5000;   //satuanana ms
int sudut = 180;                     //satuanana darajat
int kecepatan_muter = 7;              //beuki alit beuki gancang

//---------------------SAMPE SINI--------------------------

void setup() {
  servoaku.attach(9);
  servoaku.write(0);
  Serial.begin(9600);

  EEPROM.get(addr, recv_epdata);
  Serial.println("Getting old clock data....");
  runtime = recv_epdata;
  Serial.print(recv_epdata / 1000);
  Serial.println(" s");
}

void loop() {
  runtime = millis();
  EEPROM.put(addr, runtime - mkn_prvtime);
  
  if(runtime - mkn_prvtime >= selang_makan){
    mkn_prvtime = runtime;
    
    for (pos = 0; pos <= sudut; pos++) {
      servoaku.write(pos);              
      delay(kecepatan_muter);                       
    }
    
    for (pos = sudut; pos >= 0; pos--) { 
      servoaku.write(pos);              
      delay(kecepatan_muter);                      
    }  
  }
  

}
