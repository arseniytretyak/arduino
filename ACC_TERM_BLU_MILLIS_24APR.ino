#include <ArduinoBLE.h>
#include <Arduino_LSM9DS1.h>
//These  values are in the datasheet
#define RT0 100000   // Ω
#define B 4600      //  K
//--------------------------------------
#define VCC 3.3    //Supply  voltage
#define R 100000  //R=100KΩ

//Variables
float RT, VR, ln, TX0, TX1,  T0, VRT1, VRT0;
unsigned long last_time; 
float x, y, z,angleX,angleY;

// Define the BLE service and characteristic
BLEService greetingService("180C");
BLEStringCharacteristic greetingCharacteristic("2A56", BLERead | BLENotify, 30);

void setup() {
  
   digitalWrite(LED_PWR, LOW); 
  // Initialize BLE
  BLE.begin();
  
  // Set up the service and characteristic
  greetingCharacteristic.setValue("Hi");
  greetingService.addCharacteristic(greetingCharacteristic);
  BLE.addService(greetingService);
  
  // Start advertising the service
  BLE.advertise();
  Serial.begin(9600);
  T0 = 25 + 273.15;  

  if (!IMU.begin()) {
    Serial.println("Failed to initialize IMU!");
    while (1);
  }

  Serial.print("Accelerometer sample rate = ");
  Serial.print(IMU.accelerationSampleRate());
  Serial.println("Hz");
   // Set the initial rotation angles to 0
  angleX = 0;
  angleY = 0;
}

void loop() {
  
  if (millis()-1000>last_time){
  last_time=millis();
  VRT0 = analogRead(A0);              //Acquisition analog value of VRT
  Serial.println(analogRead(A0));
  VRT0  = (3.30 / 1023.00) * VRT0;      //Conversion to voltage
  VR = VCC - VRT0;
  RT = VRT0 / (VR / R);               //Resistance of RT
  ln = log(RT / RT0);
  TX0 = (1 / ((ln / B) + (1 / T0))); //Temperature from thermistor
  TX0 =  TX0 - 273.15;                 //Conversion to Celsius
  Serial.print("Temperature1:");
  Serial.print("\	");
  Serial.print(TX0);
  Serial.println("\ C");
  VRT1 = analogRead(A1);              //Acquisition analog value of VRT
  Serial.println(analogRead(A1));
  VRT1  = (3.30 / 1023.00) * VRT1;      //Conversion to voltage
  VR = VCC - VRT1;
  RT = VRT1 / (VR / R);               //Resistance of RT
  ln = log(RT / RT0);
  TX1 = (1 / ((ln / B) + (1 / T0))); //Temperature from thermistor
  TX1 =  TX1 - 273.15;                 //Conversion to Celsius
  Serial.print("Temperature2:");
  Serial.print("\	");
  Serial.print(TX1);
  Serial.println("\ C");

  IMU.readAcceleration(x, y, z);
    Serial.print("x=");
    Serial.print(x);
    Serial.print("y=");
    Serial.print(y);
    Serial.print("z=");
    Serial.print(z);

    // Calculate the rotation angles using the atan2 function
    angleX = atan2(y, z) * 180 / PI;
    angleY = atan2(x, sqrt(y * y + z * z)) * 180 / PI;

    // Map the rotation angles to the range [0, 360]
    angleX = fmod(angleX + 360, 360);
    angleY = fmod(angleY + 360, 360);


  // Print the rotation angles
  Serial.print("Rotation X: ");
  Serial.print(angleX);
  Serial.print(" degrees, Rotation Y: ");
  Serial.print(angleY);
  Serial.println(" degrees");
  
  BLEDevice central = BLE.central();
  if (central) {
    // Send the value of x to the connected device
    String message = String(TX0)+"/"+String(TX1)+"/"+String(angleX)+"/"+String(angleY);
    greetingCharacteristic.writeValue(message);
   }
  
  }
  
  
}


