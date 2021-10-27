// Load Wi-Fi library
#include <ESP8266WiFi.h>
#include <FirebaseArduino.h>                                              // firebase library

#define FIREBASE_HOST "xxxxxx.firebaseio.com"                         // the project name address from firebase id
#define FIREBASE_AUTH "xxxxxxxxxxxxxxxxxxxxxxxxxxx" 					//token

String fireStatus = ""; 
// Replace with your network credentials

const char* ssid = "Wifi_name";   // Your network details 
const char* password = "wifi_password";  // Your network details


// Set web server port number to 80
WiFiServer server(80);


// Variable to store the HTTP request
String header;
// Auxiliar variables to store the current output state
String relay1State = "off";


// Assign output variables to GPIO pins
const int relay1 = 2; // GPIO5 D1


void setup() {
  Serial.begin(115200);
  // Initialize the output variables as outputs
  pinMode(relay1, OUTPUT);

  // Set outputs to HIGH. relay active LOW
  digitalWrite(relay1, HIGH);


  // Connect to Wi-Fi network with SSID and password
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  // Print local IP address and start web server
  Serial.println("");
  Serial.println("WiFi connected.");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  server.begin();
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);                                       // connect to firebase
  Firebase.setString("LED_STATUS", "OFF");  
}

void loop() {
 fireStatus = Firebase.getString("LED_STATUS");
 if (fireStatus == "1") {                                                          // compare the input of led status received from firebase             
  delay(450);
   Serial.write(0xA0);
   Serial.write(0x01);
   Serial.write(0x01);
   Serial.write(0xA2);                                                  // make bultin led ON
   Serial.println("firbase");
   delay(450);
   Serial.write(0xA0);
   Serial.write(0x01);
   Serial.write(0x00);
   Serial.write(0xA1);
   Firebase.setString("LED_STATUS", "OFF"); 
                                                      // make external led ON
  } 
 /* else  {                                                  // compare the input of led status received from firebase
   Serial.write(0xA0);
   Serial.write(0x01);
   Serial.write(0x00);
   Serial.write(0xA1);                                              // make bultin led OFF
                                                      // make external led OFF
  }*/

  
  WiFiClient client = server.available();   // Listen for incoming clients

  if (client) {                             // If a new client connects,
    Serial.println("New Client.");          // print a message out in the serial port
    String currentLine = "";                // make a String to hold incoming data from the client
    while (client.connected()) {            // loop while the client's connected
      if (client.available()) {             // if there's bytes to read from the client,
        char c = client.read();             // read a byte, then
        Serial.write(c);                    // print it out the serial monitor
        header += c;
        if (c == '\n') {                    // if the byte is a newline character
          // if the current line is blank, you got two newline characters in a row.
          // that's the end of the client HTTP request, so send a response:
          if (currentLine.length() == 0) {
            // HTTP headers always start with a response code (e.g. HTTP/1.1 200 OK)
            // and a content-type so the client knows what's coming, then a blank line:
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println("Connection: close");
            client.println();

            // turns the GPIOs on and off
            if ((header.indexOf("GET /5/on") >= 0) || (fireStatus == "1"))
            {
              Serial.println("GPIO 5 on");
              relay1State = "on";
                    Serial.write(0xA0);
                    Serial.write(0x01);
                    Serial.write(0x01);
                    Serial.write(0xA2);
              delay(200);
              relay1State = "off";
              Serial.write(0xA0);
              Serial.write(0x01);
              Serial.write(0x00);
              Serial.write(0xA1);
              Firebase.setString("LED_STATUS", "OFF"); 
            }
            else if (header.indexOf("GET /5/off") >= 0)
            {
              Serial.println("GPIO 5 off");
              relay1State = "off";
              Serial.write(0xA0);
              Serial.write(0x01);
              Serial.write(0x00);
              Serial.write(0xA1);
            }
           

            // Display the HTML web page
            client.println("<!DOCTYPE html><html>");
            client.println("<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">");
            client.println("<link rel=\"icon\" href=\"data:,\">");
            // CSS to style the on/off buttons
            // Feel free to change the background-color and font-size attributes to fit your preferences
            client.println("<style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}");
            client.println(".button { background-color: #195B6A; border: none; color: white; padding: 12px 24px;");
            client.println("text-decoration: none; font-size: 20px; margin: 2px; cursor: pointer;}");
            client.println(".button2 {background-color: #77878A;}</style></head>");

            // Web Page Heading
            

            // Display current state, and ON/OFF buttons for GPIO 5
            client.println("<p>Relay 1 - State " + relay1State + "</p>");
            // If the relay1State is off, it displays the ON button
            if (relay1State == "off") {
              client.println("<p><a href=\"/5/on\"><button class=\"button\">ON</button></a></p>");
            } else {
              client.println("<p><a href=\"/5/off\"><button class=\"button button2\">OFF</button></a></p>");
            }


            client.println("</body></html>");

            // The HTTP response ends with another blank line
            client.println();
            // Break out of the while loop
            break;
          } else { // if you got a newline, then clear currentLine
            currentLine = "";
          }
        } else if (c != '\r') {  // if you got anything else but a carriage return character,
          currentLine += c;      // add it to the end of the currentLine
        }
      }
    }
    // Clear the header variable
    header = "";
    // Close the connection
    client.stop();
    Serial.println("Client disconnected.");
    Serial.println("");
  }
}
