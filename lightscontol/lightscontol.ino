/*
To allow you to turn on and off an LED by entering different urls using an Ethernet Shield.
 
 To turn it on:
 http://your-IP-address/$1
 
 To turn it off:
 http://your-IP-address/$2
 
 */

#include <SPI.h>
#include <Ethernet.h>

boolean incoming = 0;

// Local Network Settings
byte mac[] = { 0x00, 0xAA, 0xBB, 0xCC, 0xDE, 0x02 };
IPAddress ip(192,168,1,106);   //IP address dependent on the local network

EthernetServer server(80);  // Initialize the Ethernet server library
// with the IP address and port you want to use 
// (port 80 is default for HTTP)

///////////////////////////
//SETUP
void setup()
{
  pinMode(2, OUTPUT);

  Ethernet.begin(mac, ip);   // start the Ethernet connection and the server
  server.begin();
  Serial.begin(9600);
}
//////////////////////
//LOOP
void loop()
{
  EthernetClient client = server.available();
  if (client) {
    boolean currentLineIsBlank = true;
    while (client.connected()) {
      if (client.available()) {
        char c = client.read();
        // if you've gotten to the end of the line (received a newline
        // character) and the line is blank, the http request has ended,
        // so you can send a reply
        
         if(incoming && c == ' '){    //reads URL string from $ to first blank space
          incoming = 0;
        }
        if(c == '$'){ 
          incoming = 1; 
        }
        
          if(incoming == 1){  //Checks for the URL string $1 or $2
          Serial.println(c);
          
          if(c == '1'){
            Serial.println("ON");
            digitalWrite(2, HIGH);
          }
          if(c == '2'){
            Serial.println("OFF");
            digitalWrite(2, LOW);
          }
        
        }

        if (c == '\n') {
             currentLineIsBlank = true; // you're starting a new line
        } 
        else if (c != '\r') {
          currentLineIsBlank = false;  // you've gotten a character on the current line
        }
      }
    }
    // give the web browser time to receive the data
    delay(1);
    client.stop();    // close the connection
  }
}
