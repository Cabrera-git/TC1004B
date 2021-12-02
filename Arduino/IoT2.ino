#include <WiFiEsp.h>
#include <EEPROM.h>

// Emulate Serial1 on pins 6/7 if not present
#ifndef HAVE_HWSERIAL1
#include "SoftwareSerial.h"
SoftwareSerial Serial1(6, 7); // RX, TX
#endif


// -------------------------------------- Valores
char ssid[] = "Tenda_457EB0";
char pass[] = "taleduck300";

int status = WL_IDLE_STATUS;

String adress = "192.168.0.120";
String host = "Host: " + adress;
String server = adress;

int sensors[] = {A0, A1, A2};

char buf[100];

String UUID;
bool hasUUID = false;
bool startReading = false;

// Initialize the Ethernet client object
WiFiEspClient client;

unsigned long lastConnectionTime = 0;         // last time you connected to the server, in milliseconds
const unsigned long postingInterval = 90000L; // delay between updates, in milliseconds

void setup()
{
  // --------------------------------------------- Serial setup
  // initialize serial for debugging
  Serial.begin(9600);
  // initialize serial for ESP module
  Serial1.begin(9600);


  // ------------------------------------------ wifi settup
  // initialize ESP module
  WiFi.init(&Serial1);

  // check for the presence of the shield
  if (WiFi.status() == WL_NO_SHIELD) {
    Serial.println("WiFi shield not present");
    // don't continue
    while (true);
  }

  // attempt to connect to WiFi network
  while ( status != WL_CONNECTED) {
    Serial.print("Attempting to connect to WPA SSID: ");
    Serial.println(ssid);
    // Connect to WPA/WPA2 network
    status = WiFi.begin(ssid, pass);
  }

  // you're connected now, so print out the data
  Serial.println("You're connected to the network");

  printWifiStatus();

  // --------------------------------------- register
  HTTPrequest("/register/");

  // --------------------------------------- pinmodes
  pinMode(sensors[0], INPUT);
  pinMode(sensors[1], INPUT);
  pinMode(sensors[2], INPUT);
}

void loop()
{

  // ---------------------------------------------------------- Wifi get
  // if there are incoming bytes available
  // from the server, read them and print them
  while (client.available()) {
    char c = client.read();

    if (startReading && !hasUUID){
      if (c == ','){
        hasUUID = true;
        Serial.print("  RECORDED UUID ---------------");
      }
      else
        UUID += c;
    }

    if (c == '@'){
      Serial.print("@ detected!");
      startReading = true;
    }
    Serial.write(c);

  }

  // ---------------------------------------------------------- timer
  // if 90 seconds have passed since your last connection,
  // then connect again and send data
  if (millis() - lastConnectionTime > postingInterval) {
    // get measurment string
    HTTPrequest("/measure/" + UUID
      + "/" + analogRead(sensors[0])
      + "/" + analogRead(sensors[1])
      + "/" + analogRead(sensors[2])
      + "/" );
    lastConnectionTime = millis();
  }
}

// -------------------------------------------------------- Http requests/conn
void HTTPrequest(String request) {
  // make sure client is avaible
  client.stop();

  Serial.println();
  Serial.println("Starting connection to server...");

  // if you get a connection, report back via serial
  server.toCharArray(buf, server.length() + 1);
  if (client.connect(buf, 8000)) {
    Serial.println("Connected to server");
    // Make a HTTP request
    // build conn path
    request = "POST " + request + " HTTP/1.1";
    request.toCharArray(buf, request.length() + 1);
    client.println(buf);
    host.toCharArray(buf, host.length() + 1);
    client.println(buf);
    client.println("Connection: close");
    client.println();
  }
}

void printWifiStatus()
{
  // print the SSID of the network you're attached to
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());

  // print your WiFi shield's IP address
  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);

  // print the received signal strength
  long rssi = WiFi.RSSI();
  Serial.print("Signal strength (RSSI):");
  Serial.print(rssi);
  Serial.println(" dBm");
}
