
#include <WiFi.h>
#include <HTTPClient.h>

const char* ssid     = "*rivercheese*";
const char* password = "riverc2015";
unsigned long previousMillis = 0;
const long interval = 500;
long ran;
WiFiServer server(80);
void setup()
{
  Serial.begin(115200);
  pinMode(5, OUTPUT);      // set the LED pin mode
  pinMode(4, OUTPUT);      // set the LED pin mode
  pinMode(12, INPUT_PULLUP);
  delay(10);
  // We start by connecting to a W|iFi network
  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected.");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

  server.begin();

}

int value = 0;

void loop() {
  unsigned long currentMillis = millis();
  Serial.println(currentMillis);
  if ((WiFi.status() == WL_CONNECTED)) {
    WiFiClient client = server.available();   // listen for incoming clients
    if (client) {                             // if you get a client,
      Serial.println("New Client.");           // print a message out the serial port
      String currentLine = "";                // make a String to hold incoming data from the client
      while (client.connected()) {            // loop while the client's connected
        if (client.available()) {             // if there's bytes to read from the client,
          char c = client.read();             // read a byte, then
          Serial.write(c);                    // print it out the serial monitor
          if (c == '\n') {                    // if the byte is a newline character0
            // if the current line is blank, you got two newline characters in a row.
            // that's the end of the client HTTP request, so send a response:
            if (currentLine.length() == 0) {
              // HTTP headers always start with a response code (e.g. HTTP/1.1 200 OK)
              // and a content-type so the client knows what's coming, then a blank line:
              client.println("HTTP/1.1 200 OK");
              client.println("Content-type:text/html");
              client.println();
              // the content of the HTTP response follows the header:
              client.print("Click <a href=\"/H\">here</a> to turn the LED on pin 5 on.<br>");
              client.print("Click <a href=\"/L\">here</a> to turn the LED on pin 5 off.<br>");
              // The HTTP response ends with another blank line:
              client.println();
              // break out of the while loop:
              break;
            } else {    // if you got a newline, then clear currentLine:
              currentLine = "";
            }
          } else if (c != '\r') {  // if you got anything else but a carriage return character,
            currentLine += c;      // add it to the end of the currentLine
          }
          // Check to see if the client request was "GET /H" or "GET /L":
          if (currentLine.endsWith("GET /H")) {
            digitalWrite(5, HIGH);               // GET /H turns the LED on
          }
          if (currentLine.endsWith("GET /L")) {
            digitalWrite(5, LOW);                // GET /L turns the LED off
          }
        }
      }
      // close the connection:
      client.stop();
      Serial.println("incomming Disconnected.");
    }
    String req = "";
    if (digitalRead(12)) {
      req = "http://www.paisemprendedor.com/iot/test.php?de=1&vr=inte&vl=1&ut=state";
      digitalWrite(5, HIGH);
    } else {
      req = "http://www.paisemprendedor.com/iot/test.php?de=1&vr=inte&vl=0&ut=state";
      digitalWrite(5, LOW);
    }
    ran = random(100);
    float r=ran/100;
    String rs=String(ran);
    String req2 = "http://www.paisemprendedor.com/iot/test.php?de=2&vr=inte&ut=state&vl=";
    req2+=rs;
    if (currentMillis - previousMillis >= interval) {
      previousMillis = currentMillis;
      HTTPClient http;
      http.begin(req);
      int httpCode = http.GET();
      if (httpCode > 0) { //Check for the returning code

        String payload = http.getString();
        Serial.println(httpCode);
        Serial.println(payload);
      }

      else {
        Serial.println("Error on HTTP request");
      }
      http.end();
      flag(4);
      http.begin(req2);
      httpCode = http.GET();
      if (httpCode > 0) { //Check for the returning code

        String payload = http.getString();
        Serial.println(httpCode);
        Serial.println(payload);
      }

      else {
        Serial.println("Error on HTTP request");
      }
      http.end();
      flag(4);
    }

  }
}
void flag(int pin) {
  digitalWrite(pin, HIGH);
  delay(100);
  digitalWrite(pin, LOW);
  delay(100);
  digitalWrite(pin, HIGH);
  delay(100);
  digitalWrite(pin, LOW);
}
