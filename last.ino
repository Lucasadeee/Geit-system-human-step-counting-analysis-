#include <Wire.h>  // Wire library - used for I2C communication
#include <SPI.h>
#include <SD.h>
File myFile;


int ADXL345 = 0x53; // The ADXL345 sensor I2C address
float X_out, Y_out, Z_out;  // Outputs

void setup() {
  Serial.begin(9600); // Initiate serial communication for printing the results on the Serial monitor

 while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }

  Serial.print("Initializing SD card...");

  if (!SD.begin(10)) {
    Serial.println("initialization failed!");
    while (1);
  }
  Serial.println("initialization done.");


  
  Wire.begin(); // Initiate the Wire library
  // Set ADXL345 in measuring mode
  Wire.beginTransmission(ADXL345); // Start communicating with the device 
  Wire.write(0x2D); // Access/ talk to POWER_CTL Register - 0x2D
  // Enable measurement
  Wire.write(8); // (8dec -> 0000 1000 binary) Bit D3 High for measuring enable 
  Wire.endTransmission();
  delay(10);
}
float cnt = 0;


// Distance logger 
float distance = 5 ; 
double  stptime ;
double Speed ; 
double Cadence ; 
double stplength;
double stance_time;
double swing_time;

void loop() {
  // === Read acceleromter data === //
  Wire.beginTransmission(ADXL345);
  Wire.write(0x32); // Start with register 0x32 (ACCEL_XOUT_H)
  Wire.endTransmission(false);
  Wire.requestFrom(ADXL345, 6, true); // Read 6 registers total, each axis value is stored in 2 registers
  X_out = ( Wire.read()| Wire.read() << 8); // X-axis value
  X_out = X_out/256; //For a range of +-2g, we need to divide the raw values by 256, according to the datasheet
  Y_out = ( Wire.read()| Wire.read() << 8); // Y-axis value
  Y_out = Y_out/256;
  Z_out = ( Wire.read()| Wire.read() << 8); // Z-axis value
  Z_out = Z_out/256;
 if (abs(Z_out) > 1.60){
  Serial.println("Step Counted") ; 
  cnt = cnt+1 ;
  Serial.println(cnt) ; 
  delay (200) ; 
  stptime= millis() /1000 ;
 // Serial.println(stptime) ;  
   Cadence = (cnt/(stptime/60));
   stplength= distance/cnt;
 double(Speed) = distance / stptime ;
 stance_time= (stptime*0.65/cnt)*2;
 swing_time= (stptime*0.35/cnt)*2;
 
 Serial.println("Speed is (m/s) ");
 Serial.println(Speed) ; 
 Serial.println("Distance walked (m) "); 
 Serial.println(distance) ;
 Serial.println("Step Time (s) ");
Serial.println(stptime) ;
Serial.println("Cadence is (step/min) ");
 Serial.println(Cadence) ;
Serial.println("Step length is (m) ");
 Serial.println(stplength) ;
 Serial.print("Stance Time (ms):  ");
       Serial.println(stance_time);
  Serial.print("Swing Time (ms):  ");
  Serial.println(swing_time);
 
 myFile = SD.open("test.txt", FILE_WRITE);
  // if the file opened okay, write to it:
  if (myFile) {
    Serial.print("Writing to test.txt...");
    myFile.println("Time (s) ");
     myFile.println(stptime);
    myFile.println("Speed (m/s): ");
     myFile.println(Speed);
    myFile.println("Step Counted ");
     myFile.println(cnt);
    myFile.println("Cadence (step/min) ");
     myFile.println(Cadence);
    myFile.println("Step length (m) ");
     myFile.println(stplength);
      myFile.println("Stance Time (s) ");
     myFile.println(stance_time);
      myFile.println("Swing Time (s) ");
     myFile.println(swing_time);
    // close the file:
    myFile.close();
    Serial.println("done.");
  } else {
    // if the file didn't open, print an error:
    Serial.println("error opening test.txt");
  }

 
 }
 
}
