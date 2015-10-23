/*
 This file is to watch and report the amount of coffee
 left in the coffee pot for every minute.
 */
#include <Energia.h>
#include <Wire.h>
#include <HX711.h>
#include <LiquidCrystal_I2C.h>
#include <WiFi.h>
#include <WiFiClient.h>


// This is for the scale HX711
#define DOUT  8 //PIN_62
#define CLK  30 //PIN_50
// The difference between POT_FULL and POT_EMPTY
// is the coffee weight
#define POT_FULL 8 //lbs
#define POT_EMPTY 4 //lbs

float calibration_factor = -7050; //-7050 worked for my 440lb max scale setup
//float calibration_factor = -209385; // This number worked for me 
float pot_weight = 0;  // Initilize the pot weight

// your network name also called SSID
char ssid[] = "test";
// your network password
char password[] = "test";

// Create HX711 object by passing the pin used on the launchpad
HX711 scale(DOUT,CLK);
LiquidCrystal_I2C lcd(0x27,16,2);  // set the LCD address to 0x27 for a 20 chars and 4 line display

void setup() {
  //Initialize serial and wait for port to open:
  Serial.begin(115200);
  lcd.init();                      // initialize the lcd 
  lcd.backlight();  
  lcd.setCursor(0,0); // col,row
  // attempt to connect to Wifi network:
  Serial.print("Attempting to connect to Network named: ");
  // print the network name (SSID);
  lcd.print("SSID: ");
  Serial.println(ssid); 
  lcd.print(ssid);
  lcd.home();
  // Connect to WPA/WPA2 network. Change this line if using open or WEP network:
  WiFi.begin(ssid, password);
  while ( WiFi.status() != WL_CONNECTED) {
    // print dots while we wait to connect
    Serial.print(".");
    lcd.print(".");
    delay(300);
  }

  Serial.println("\nYou're connected to the network");
  lcd.print("Connected");
  lcd.home();
  Serial.println("Waiting for an ip address");
  lcd.print("Wait for IP");
  lcd.home();
  while (WiFi.localIP() == INADDR_NONE) {
    // print dots while we wait for an ip addresss
    Serial.print(".");
    lcd.print(".");
    delay(300);
  }

  Serial.println("\nIP Address obtained");
  lcd.print("IP obtained!");
  lcd.home();
  // you're connected now, so print out the status  
  printCurrentNet();
  printWifiData();
      
  // Scale initialization
  scale.set_scale();
  scale.tare(); //Reset the scale to 0
  lcd.print("Place 1000g on scale");
  lcd.home();
  delay(1000); // to calibrate place 1000g wheight on scale
  noInterrupts();
  float weight = scale.get_units(10);
  interrupts();
  scale.set_scale(weight/1000);
  //long zero_factor = scale.read_average(); //Get a baseline reading
  //Serial.print("Zero factor: "); //This can be used to remove the need to tare the scale. Useful in //permanent scale projects.
  //\\\\\Serial.println(zero_factor);
}

void loop() {
  // check the network connection once every 10 seconds:
  delay(1000);
  //printCurrentNet();
  noInterrupts();
  float weight = scale.get_units(1);
  interrupts();
  Serial.print(weight, 1);
  Serial.println();
  lcd.setCursor(0,1); // col,row
  lcd.print("weight:");
  lcd.print(weight);
  lcd.print("g");
  lcd.print("     "); // clear the row
  lcd.home();
}


void printWifiData() {
  // print your WiFi IP address:
  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);
  Serial.println(ip);
  lcd.print(ip);
  lcd.print("          ");  // clear the row

  // print your MAC address:
  byte mac[6];
  WiFi.macAddress(mac);
  Serial.print("MAC address: ");
  Serial.print(mac[5], HEX);
  Serial.print(":");
  Serial.print(mac[4], HEX);
  Serial.print(":");
  Serial.print(mac[3], HEX);
  Serial.print(":");
  Serial.print(mac[2], HEX);
  Serial.print(":");
  Serial.print(mac[1], HEX);
  Serial.print(":");
  Serial.println(mac[0], HEX);

}

void printCurrentNet() {
  // print the SSID of the network you're attached to:
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());

  // print the MAC address of the router you're attached to:
  byte bssid[6];
  WiFi.BSSID(bssid);
  Serial.print("BSSID: ");
  Serial.print(bssid[5], HEX);
  Serial.print(":");
  Serial.print(bssid[4], HEX);
  Serial.print(":");
  Serial.print(bssid[3], HEX);
  Serial.print(":");
  Serial.print(bssid[2], HEX);
  Serial.print(":");
  Serial.print(bssid[1], HEX);
  Serial.print(":");
  Serial.println(bssid[0], HEX);

  // print the received signal strength:
  long rssi = WiFi.RSSI();
  Serial.print("signal strength (RSSI):");
  Serial.println(rssi);

  // print the encryption type:
  byte encryption = WiFi.encryptionType();
  Serial.print("Encryption Type:");
  Serial.println(encryption, HEX);
  Serial.println();
}




