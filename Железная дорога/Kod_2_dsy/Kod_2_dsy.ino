
#include <SPI.h>
#include <Ethernet.h>
#include <ESP8266WiFi.h>
#include <PubSubClient.h>

const char* ssid     = "Iot";
const char* password = "Iot12345";

// Установка сервера на 80 порт
WiFiServer server(80);

// Заголовок строки
String header;

// Вспомогательные переменные для кнтроля выходов
String output5State = "off";
String output4State = "off";

// GPIO для выходов
const int output5 = 5;
const int output4 = 4;

void setup() {
  Serial.begin(115200);
  // Инициализации выходных переменных как выходов
  pinMode(output5, OUTPUT);
  pinMode(output4, OUTPUT);
  //выход на LOW
  digitalWrite(output5, LOW);
  digitalWrite(output4, LOW);

  // Wi-Fi соединение
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  // Печать IP адресса и запус сервера
  Serial.println("");
  Serial.println("WiFi connected.");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  server.begin();
}

void loop(){
  WiFiClient client = server.available();   // Прослушивание входящий клиентов

  if (client) {                             
    Serial.println("New Client.");           
    String currentLine = "";                
    while (client.connected()) {            
      if (client.available()) {             
        char c = client.read();             
        Serial.write(c);                    
        header += c;
        if (c == '\n') {                    
          
          
          if (currentLine.length() == 0) {
          
          
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println("Connection: close");
            client.println();
            
           // ВКЛ и ВЫКЛ
            if (header.indexOf("GET /5/on") >= 0) {
              Serial.println("GPIO 5 on");
              output5State = "on";
              digitalWrite(output5, HIGH);
            } else if (header.indexOf("GET /5/off") >= 0) {
              Serial.println("GPIO 5 off");
              output5State = "off";
              digitalWrite(output5, LOW);
            } else if (header.indexOf("GET /4/on") >= 0) {
              Serial.println("GPIO 4 on");
              output4State = "on";
              digitalWrite(output4, HIGH);
            } else if (header.indexOf("GET /4/off") >= 0) {
              Serial.println("GPIO 4 off");
              output4State = "off";
              digitalWrite(output4, LOW);
            }
            
            // использование во всех веб-браузерлв
            client.println("<!DOCTYPE html><html>");
            client.println("<head><meta name=\"viewport\" content=\"width=device-width; initial-scale=1; charset=cp1261\" >");
            client.println("<link rel=\"icon\" href=\"data:,\">");
            //кноаки внешнег вида
            //выбираем шрифт и контент который будет выравниваться по центру
            client.println("<style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}");
            //цвет,размер..
            client.println(".button { background-color: #195B6A; border: none; color: white; padding: 16px 40px;");
            client.println("text-decoration: none; font-size: 30px; margin: 2px; cursor: pointer;}");
            //цвет и размер второй кнопки
            client.println(".button2 {background-color: #77878A;}</style></head>");
            
            // назвение веб страницы
            client.println("<body><h1>The management of the railway</h1>");
            
            // Display current state, and ON/OFF buttons for GPIO 5  
            client.println("<p>Forward " + output5State + "</p>");
            // If the output5State is off, it displays the ON button       
            if (output5State=="off") {
              client.println("<p><a href=\"/5/on\"><button class=\"button\">Forward</button></a></p>");
            } else {
              client.println("<p><a href=\"/5/off\"><button class=\"button button2\">Forward</button></a></p>");
            } 
               
            // Display current state, and ON/OFF buttons for GPIO 4  
            client.println("<p>Back " + output4State + "</p>");
            // If the output4State is off, it displays the ON button       
            if (output4State=="off") {
              client.println("<p><a href=\"/4/on\"><button class=\"button\">Back</button></a></p>");
            } else {
              client.println("<p><a href=\"/4/off\"><button class=\"button button2\">Back</button></a></p>");
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
