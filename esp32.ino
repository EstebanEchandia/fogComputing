#include <WiFi.h>
#include <PubSubClient.h>


//Nombre y contraseña del wifi.
const char* ssid = "NAME";
const char* password = "PASSWORD";

//Ip del Brokker mqtt, en nuestor caso el servidor de gimosic.
const char* mqtt_server = "IP";
 
WiFiClient espClient;
PubSubClient client(espClient);
long lastMsg = 0;
char msg[50];
int value = 0;
const char* message = "P";
long sendTime;
long recibeTime;
int LED_BUILTIN = 2;
bool messageSended = false;
int counter = 1;
void setup() {
  Serial.begin(115200); //Seteamos el serial para ver los print
  delay(500);
  setup_wifi();   
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
  bool messageSended = false;
}

void setup_wifi() {
  delay(10);
  //Empezamos conectando el wifi
  Serial.println();
  Serial.print("Conectando a ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi conectado");
  Serial.println("IP local: ");
  Serial.println(WiFi.localIP());
}


void reconnect() {
  // Loop hasta que se reconecte
  while (!client.connected()) {
    Serial.print("Intentando conexion MQTT...");
    // Attempt to connect
    if (client.connect("ESP8266Client")) {
      Serial.println("conectado");
      // Subscribe
      client.subscribe("esp32/output");
    } else {
      Serial.print("fallo, rc=");
      Serial.print(client.state());
      Serial.println(" intentando devuelta en 5 segundos");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();

  long now = millis();
  //Mandar mensaje cada 5 segundos
  
  if(now - lastMsg > 5000){
    Serial.print("Se envia el mensaje numero: ");
    Serial.println(counter);
    counter += 1;
    lastMsg = now;
    Serial.print("Mensaje enviado: ");
    Serial.println(message);
    sendTime = millis();
    client.publish("esp32/pruebas", message);
  }
}

void callback(char* topic, byte* message, unsigned int length) {
  Serial.print("Mensaje recibido: ");
  String messageTemp;
  
  for (int i = 0; i < length; i++) {
    Serial.print((char)message[i]);
    messageTemp += (char)message[i];
  }
  Serial.println();

  if (String(topic) == "esp32/output") {
    recibeTime = millis();
    Serial.print("Se tardo: ");
    long latency = recibeTime - sendTime;
    Serial.print(latency);
    Serial.println("ms");

   // Serial.println("Changing output to ");
    //if(messageTemp == "on"){
      //Serial.println("on");
      //digitalWrite(ledPin, HIGH);
    //}
    //else if(messageTemp == "off"){
      //Serial.println("off");
      //digitalWrite(ledPin, LOW);
//    }
  }
}