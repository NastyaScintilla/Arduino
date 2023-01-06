#include <SPI.h>
#include <Ethernet.h>
#include <ESP8266WiFi.h>
#include <PubSubClient.h>

const char* ssid = "Iot"; //  SSID (имя роутера)
const char* pass = "Iot12345";    //  пароль роутера 
const char* mqtt_server ="192.168.200.17";//  сервер


 WiFiClient espClient;
 PubSubClient client(espClient); \
 long lastMsg = 0;
 char msg[50];
 int value = 0;
 void setup_wifi() {
 
  delay(10);
  
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, pass);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  randomSeed(micros());

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}


void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println();

  if ((char)payload[0] == '1') {
    digitalWrite(BUILTIN_LED, LOW);   
  } else {
    digitalWrite(BUILTIN_LED, HIGH);  
  }

}

void reconnect() {
 
  while (!client.connected())
 {
    Serial.print("Attempting MQTT connection...");
    // Create a random client ID
   String clientId = "ESP8266Client-";
    clientId += String(random(0xffff), HEX);
    // Attempt to connect
    if (client.connect("ESP8266Client-"))//clientId.c_str()))
    {
      Serial.println("connected");
      // Once connected, publish an announcement...
      //client.publish("outTopic", "hello world");
      // ... and resubscribe
      client.subscribe("rocs/#");
      //client.subscribe("rocs/10/#");
      //client.subscribe("rocs/14/#");
      //client.subscribe("rocs/16/#");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

void setup() {
  
  pinMode(BUILTIN_LED, OUTPUT);     // Initialize the BUILTIN_LED pin as an output
  Serial.begin(115200);
  setup_wifi();
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
  
}

void loop() {
 
  if (!client.connected()) {
    reconnect();
  }
  client.loop();
  if (Serial.available()>0)
  {
  char knop = Serial.read();
   switch (knop) {
  
  case 'w' :(client.publish("rocrail/service/client","<lc id=\"3\" v=\"50\" cmd=\"velocity\"/>" )); Serial.println ("V50") ;break;
  case 's' :(client.publish("rocrail/service/client","<lc id=\"3\" v=\"0\" cmd=\"velocity\"/>" )); Serial.println ("V0") ;break;
  case 'l' :(client.publish("rocrail/service/client","<lc id=\"3\" cmd=\"fn\"/>" )); Serial.println ("LN") ;break;
  case 'o' :(client.publish("rocrail/service/client","<sys cmd=\"stop\"/>" )); Serial.println ("PowerOff") ;break;
  case 'g' :(client.publish("rocrail/service/client","<sys cmd=\"go\"/>" )); Serial.println ("PowerOn") ;break;
}
  }
// long now = millis();
//if (now - lastMsg > 2000) {
    //lastMsg = now;
  // ++value;
// snprintf (msg, 75, "ld", value);
   //Serial.print("Publish message:sys cmd=go");
   //Serial.println(msg);
   // client.publish("rocrail/service/client","<lc id=\"4\" v=\"50\" cmd=\"velocity\"/>" );
    //delay (10000);
   // client.publish("rocrail/service/client","<lc id=\"4\" v=\"0\" cmd=\"velocity\"/>" );
  //  delay (10000);
  }
//}
