#include <ArduinoBLE.h>
#define motor_IN1 16
#define motor_IN2 17
#define motor_IN3 18
#define motor_IN4 19
BLEService RGBledService("19B10000-E8F2-537E-4F6C-D104768A1214"); // Bluetooth® Low Energy LED Service
 
// Bluetooth® Low Energy LED Switch Characteristic - custom 128-bit UUID, read and writable by central
BLEByteCharacteristic switchCharacteristicR("19B10001-E8F2-537E-4F6C-D104768A1215", BLERead | BLEWrite);
BLEByteCharacteristic switchCharacteristicG("19B10001-E8F2-537E-4F6C-D104768A1216", BLERead | BLEWrite);
BLEByteCharacteristic switchCharacteristicB("19B10001-E8F2-537E-4F6C-D104768A1217", BLERead | BLEWrite); 
const int ledPinR = 22; // pin to use for the LEDR
const int ledPinG = 23; // pin to use for the LEDG
const int ledPinB = 21; // pin to use for the LEDB
void setup() {
  Serial.begin(9600);
  delay(200);//some delay time for the serial device to start
 
  // set LED pin to output mode
  pinMode(ledPinR, OUTPUT);  digitalWrite(ledPinR, HIGH);   
  pinMode(ledPinG, OUTPUT);  digitalWrite(ledPinG, HIGH); 
  pinMode(ledPinB, OUTPUT);  digitalWrite(ledPinB, HIGH); 
  pinMode(motor_IN1,OUTPUT);
  pinMode(motor_IN2,OUTPUT);
  pinMode(motor_IN3,OUTPUT);
  pinMode(motor_IN4,OUTPUT);
  // begin initialization
  if (!BLE.begin()) {
    Serial.println("starting Bluetooth® Low Energy module failed!");
 
    while (1);
  }
 
  // set advertised local name and service UUID:
  BLE.setLocalName("LED");
  BLE.setAdvertisedService(RGBledService);
 
  // add the characteristic to the service
  RGBledService.addCharacteristic  (switchCharacteristicR);
  RGBledService.addCharacteristic  (switchCharacteristicG);
  RGBledService.addCharacteristic  (switchCharacteristicB);
 
  // add service
  BLE.addService(RGBledService);
 
  // set the initial value for the characeristic:
  switchCharacteristicR.writeValue(0);
  switchCharacteristicG.writeValue(0);
  switchCharacteristicB.writeValue(0);
  // start advertising
  BLE.advertise();
 
  Serial.println("BLE LED Peripheral");
}

void loop() {
  // listen for Bluetooth® Low Energy peripherals to connect:
  BLEDevice central = BLE.central();

  //the device is not connected to any central ... do a fun RGB carousel
    
  // if a central is connected to peripheral:
  if (central) {
    Serial.print("Connected to central: ");
    // print the central's MAC address:
    Serial.println(central.address());
 
    // while the central is still connected to peripheral:
  while (central.connected()) {
  digitalWrite(ledPinR, HIGH); //first turn OFF all leds.  
  digitalWrite(ledPinG, HIGH);
  digitalWrite(ledPinB, HIGH); 
   while (central.connected()) {
        if (switchCharacteristicR.written()) {//if the characteristic was just written
          if (switchCharacteristicR.value()) {//if the characteristic contains a value other than 0
            tien();
            digitalWrite(ledPinR, LOW);    
          } else {                              
            lui(); 
            digitalWrite(ledPinR, HIGH);
          }
        }

         if (switchCharacteristicG.written()) {
          if (switchCharacteristicG.value()) {   
            trai();  
            digitalWrite(ledPinG, LOW);
          } else {                              
            phai();
            digitalWrite(ledPinR, HIGH);
          }
        }
        if (switchCharacteristicB.written()) {
          if (switchCharacteristicB.value()) {   
            dung();
            Serial.println(switchCharacteristicB.value());
            digitalWrite(ledPinB, LOW); // changed from HIGH to LOW       
          } else {                              
            Serial.println(F("LED off"));
            digitalWrite(ledPinB, HIGH); // changed from LOW to HIGH     
          }
        }
      }
  }

    // when the central disconnects, print it out:
    Serial.print(F("Disconnected from central: "));
    Serial.println(central.address());
  }
}
void tien(){
  digitalWrite(16,0);
  digitalWrite(17,1);
  digitalWrite(19,0);
  digitalWrite(18,1);
}
void lui(){
  digitalWrite(16,1);
  digitalWrite(17,0);
  digitalWrite(19,1);
  digitalWrite(18,0);
}
void trai(){
  digitalWrite(16,0);
  digitalWrite(17,1);
  digitalWrite(19,1);
  digitalWrite(18,0);
}
void phai(){
  digitalWrite(16,1);
  digitalWrite(17,0);
  digitalWrite(19,0);
  digitalWrite(18,1);
}
void dung(){
  digitalWrite(16,0);
  digitalWrite(17,0);
  digitalWrite(19,0);
  digitalWrite(18,0);
}
