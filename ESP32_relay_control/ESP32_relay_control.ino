#include <WiFi.h>                                                // esp32 library
#include <FirebaseESP32.h>                                             // firebase library
#define FIREBASE_HOST "xxxxxxx.firebaseio.com"						    //FIREBASE HOST name
#define FIREBASE_AUTH "U4R7E0Z5pRfy*************slUQLZUAWf"         // the secret key generated from firebase
#define WIFI_SSID  "wifi_name"
#define WIFI_PASSWORD "wifi_password"

String fireStatus = "";                                                     // led status received from firebase
int led = 13;                                                               
void setup() {
  Serial.begin(9600);
  delay(1000);
  pinMode(2, OUTPUT);                
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);                                      //try to connect with wifi
  Serial.print("Connecting to ");
  Serial.print(WIFI_SSID);
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  Serial.println();
  Serial.print("Connected to ");
  Serial.println(WIFI_SSID);
  Serial.print("IP Address is : ");
  Serial.println(WiFi.localIP());                                                      //print local IP address
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);                                       // connect to firebase
  Firebase.setString("LED_STATUS", "OFF");                                          //send initial string of led status

}

void loop() {

  fireStatus = Firebase.getString("LED_STATUS");                     // get led status input from firebase
  if (fireStatus == "ON") {                         // compare the input of led status received from firebase
    Serial.println("Led Turned ON");                 
    digitalWrite(2, HIGH);                                                         // make output led ON


  else if (fireStatus == "OFF") {              // compare the input of led status received from firebase
    Serial.println("Led Turned OFF");
    digitalWrite(2, LOW);                                                         // make output led OFF

  }

  else {
    Serial.println("Wrong Credential! Please send ON/OFF");
  }

}
