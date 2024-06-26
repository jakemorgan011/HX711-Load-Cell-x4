/*
   -------------------------------------------------------------------------------------
   HX711_ADC
   Arduino library for HX711 24-Bit Analog-to-Digital Converter for Weight Scales
   Olav Kallhovd sept2017
   -------------------------------------------------------------------------------------
*/

/*
   Settling time (number of samples) and data filtering can be adjusted in the config.h file
   For calibration and storing the calibration value in eeprom, see example file "Calibration.ino"

   The update() function checks for new data and starts the next conversion. In order to acheive maximum effective
   sample rate, update() should be called at least as often as the HX711 sample rate; >10Hz@10SPS, >80Hz@80SPS.
   If you have other time consuming code running (i.e. a graphical LCD), consider calling update() from an interrupt routine,
   see example file "Read_1x_load_cell_interrupt_driven.ino".

   This is an example sketch on how to use this library
*/

#include <HX711_ADC.h>
#if defined(ESP8266)|| defined(ESP32) || defined(AVR)
#include <EEPROM.h>
#endif

int sensorVal[4];


//pins:
const int HX711_dout01 =6 ; //mcu > HX711 dout pin
const int HX711_sck01 = 7; //mcu > HX711 sck pin

//HX711 constructor:
//HX711_ADC LoadCell01(6,7); //(dout, sck)
HX711_ADC LoadCell02(4,5); //(dout, sck)
HX711_ADC LoadCell03(6,7); //(dout, sck)
HX711_ADC LoadCell04(8,9); //(dout, sck)




const int calVal_eepromAdress = 0;
unsigned long t = 0;

void setup() {
  Serial.begin(57600); delay(10);
  Serial.println();
  Serial.println("Starting...");

  //LoadCell01.begin();  
  LoadCell02.begin();   LoadCell03.begin();   LoadCell04.begin();
  //LoadCell.setReverseOutput(); //uncomment to turn a negative output value to positive
  float calibrationValue01; float calibrationValue02; float calibrationValue03; float calibrationValue04; // calibration value (see example file "Calibration.ino")
  calibrationValue01 = 600; // uncomment this if you want to set the calibration value in the sketch
  calibrationValue02 = 600; // uncomment this if you want to set the calibration value in the sketch
  calibrationValue03 = 600; // uncomment this if you want to set the calibration value in the sketch
  calibrationValue04 = 600; // uncomment this if you want to set the calibration value in the sketch
#if defined(ESP8266)|| defined(ESP32)
  //EEPROM.begin(512); // uncomment this if you use ESP8266/ESP32 and want to fetch the calibration value from eeprom
#endif
  //EEPROM.get(calVal_eepromAdress, calibrationValue); // uncomment this if you want to fetch the calibration value from eeprom

  unsigned long stabilizingtime = 2000; // preciscion right after power-up can be improved by adding a few seconds of stabilizing time
  boolean _tare = true; //set this to false if you don't want tare to be performed in the next step
  //LoadCell01.start(stabilizingtime, _tare);
  LoadCell02.start(stabilizingtime, _tare);
  LoadCell03.start(stabilizingtime, _tare);
  LoadCell04.start(stabilizingtime, _tare);
/*
  if (LoadCell01.getTareTimeoutFlag()) {
    Serial.println("Timeout, check MCU>HX711 wiring and pin designations_01");
    while (1);
  }
  else {
    LoadCell01.setCalFactor(calibrationValue01); // set calibration value (float)
    Serial.println("Startup01 is complete");
  }
*/
  if (LoadCell02.getTareTimeoutFlag()) {
    Serial.println("Timeout, check MCU>HX711 wiring and pin designations_02");
    while (1);
  }
  else {
    LoadCell02.setCalFactor(calibrationValue02); // set calibration value (float)
    Serial.println("Startup02 is complete");
  }

  if (LoadCell03.getTareTimeoutFlag()) {
    Serial.println("Timeout, check MCU>HX711 wiring and pin designations_03");
    while (1);
  }
  else {
    LoadCell03.setCalFactor(calibrationValue03); // set calibration value (float)
    Serial.println("Startup03 is complete");
  }

  if (LoadCell04.getTareTimeoutFlag()) {
    Serial.println("Timeout, check MCU>HX711 wiring and pin designations_04");
    while (1);
  }
  else {
    LoadCell04.setCalFactor(calibrationValue04); // set calibration value (float)
    Serial.println("Startup04 is complete");
  }
}

void loop() {
  //static boolean newDataReady01 = 0;
  static boolean newDataReady02 = 0;
  static boolean newDataReady03 = 0;
  static boolean newDataReady04 = 0;

  const int serialPrintInterval = 0; //increase value to slow down serial print activity

  // check for new data/start next conversion:
  //if (LoadCell01.update()) newDataReady01 = true;
  if (LoadCell02.update()) newDataReady02 = true;
  if (LoadCell03.update()) newDataReady03 = true;
  if (LoadCell04.update()) newDataReady04 = true;

  // get smoothed value from the dataset:
  /*
  if (newDataReady01) {
    if (millis() > t + serialPrintInterval) {
      sensorVal[0] = LoadCell01.getData();

      //Serial.print("Load_cell_01 output val: ");   
      newDataReady01 = 0;
      t = millis();
    }
  }
*/

  if (newDataReady02) {
    if (millis() > t + serialPrintInterval) {
      sensorVal[1] = LoadCell02.getData();

      //Serial.print("Load_cell_02 output val: "); 
      newDataReady02 = 0;
      t = millis();
    }
  }

  if (newDataReady03) {
    if (millis() > t + serialPrintInterval) {
      sensorVal[2] = LoadCell03.getData();

      //Serial.print("Load_cell_03 output val: ");
      newDataReady03 = 0;
      t = millis();
    }
  }

  if (newDataReady04) {
    if (millis() > t + serialPrintInterval) {
      sensorVal[3] = LoadCell04.getData();

      //Serial.print("Load_cell_04 output val: ");
      newDataReady04 = 0;
      t = millis();
    }
  }

  //serialPrint(sensorVal[0],sensorVal[1],sensorVal[2],sensorVal[3]);
  //Serial.print("[");
  //Serial.print(sensorVal[0]);
  //Serial.print(",");
  Serial.print(sensorVal[1]);
  Serial.print(",");
  Serial.print(sensorVal[2]);
  Serial.print(",");
  Serial.println(sensorVal[3]);
  delay(10);
  //Serial.print("]");
  //Serial.print("\n");


  // receive command from serial terminal, send 't' to initiate tare operation:
  if (Serial.available() > 0) {
    char inByte = Serial.read();
    if (inByte == 't') {
      //LoadCell01.tareNoDelay();
      LoadCell02.tareNoDelay();
      LoadCell03.tareNoDelay();
      LoadCell04.tareNoDelay();
    }
  }

  // check if last tare operation is complete:
  /*
  if (LoadCell01.getTareStatus() == true) {
    Serial.println("Tare complete_01");
  }
  */
  if (LoadCell02.getTareStatus() == true) {
    Serial.println("Tare complete_02");
  }
  if (LoadCell03.getTareStatus() == true) {
    Serial.println("Tare complete_03");
  }
  if (LoadCell04.getTareStatus() == true) {
    Serial.println("Tare complete_04");
  }

}

void serialPrint(int x, int y, int z, int u){
  //Serial.print("[");
  Serial.print(x);
  //Serial.print(",");
  Serial.print(y);
  //Serial.print(",");
  Serial.print(z);
 // Serial.print(",");
  Serial.print(u);
  //Serial.print("]");
  Serial.print("/n");
  delay(10);
}
