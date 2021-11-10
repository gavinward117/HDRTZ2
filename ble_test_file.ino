#include <ArduinoBLE.h>
BLEService voltageService("1101");
BLEUnsignedCharCharacteristic voltageConverted("2101", BLERead | BLENotify);

void setup() {
  Serial.begin(9600);
  while (!Serial);
  
  pinMode(LED_BUILTIN, OUTPUT);
  if (!BLE.begin()) 
  {
    Serial.println("starting BLE failed!");
    while (1);
  }
  
  BLE.setLocalName("Crank");
  BLE.setAdvertisedService(voltageService);
  batteryService.addCharacteristic(voltageConverted);
  BLE.addService(voltageService);
  
  BLE.advertise();
  Serial.println("Bluetooth device active, waiting for connections...");
  }

void loop() 
{
  BLEDevice central = BLE.central();
  
  if (central) 
  {
  Serial.print("Connected to central: ");
  Serial.println(central.address());
  digitalWrite(LED_BUILTIN, HIGH);
  
  while (central.connected()) {
  
        int voltage = analogRead(A0);
        //int digitalVoltage = map(battery, 0, 1023, 0, 100);
        Serial.print("Voltage is now: ");
        Serial.println(batteryLevel);
        voltageConverted.writeValue(batteryLevel);
        delay(200);
  
  }
  }
  digitalWrite(LED_BUILTIN, LOW);
  Serial.print("Disconnected from central: ");
  Serial.println(central.address());
}
