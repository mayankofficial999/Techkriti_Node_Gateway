#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <WiFiServer.h>
#include <PubSubClient.h>

const char *gateway_id="2111";
const char *ap_ssid = "ESP8266";
const char *ap_password = "12345678";

const char *sta_ssid = "Motera"; 
const char *sta_password = "aaaaayyyeeee";
const char* mqtt_server = "65.1.164.121";
const char* userName = "teamrookie";
const char* passWord = "benrowonahack";
String currentLine,Input="";
char output[20],sample_out[20];

WiFiClient espClient;
PubSubClient client_mqtt(espClient);

WiFiServer server(80);

typedef struct struct_message {
  String nodeID;
  int value;
  String action;
  String extras;
} struct_message;

struct_message Node_Data;
struct_message Server_Data;
String temporary;

void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
    Input+=(char)payload[i];
  }
    //Extracting Data from the server.
    String c="";int c1=0;
  for(int i=0;i<Input.length();i++)
  {
    if(Input.charAt(i)!='/')
    c+=Input.charAt(i);
    else
    {
      c1++;
      if(c1==1)
      {
      Server_Data.nodeID=c;
      c="";
      }
      else if(c1==2){
      Server_Data.action=c;
      c="";}
      else if(c1==3){
      Server_Data.value=c.toInt();
      c="";}
      else{}
    }
  }
  Server_Data.extras=c.toInt();
  temporary=(String)Server_Data.nodeID+"/"+(String)Server_Data.action+"/"+(String)Server_Data.value+"/"+(String)Server_Data.extras;// Extracting Node's Data
}

void reconnect() {
  // Loop until we're reconnected
  while (!client_mqtt.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Create a random client ID
    String clientId = "ESP8266Client-";
    clientId += String(random(0xffff), HEX);
    // Attempt to connect
    if (client_mqtt.connect(clientId.c_str(),userName,passWord)) {
      Serial.println("connected");
        String sample="12g/OFF/0/0";
        char n1[100]="";
        sample.toCharArray(n1,sizeof(n1));
        snprintf (sample_out, 25, n1, 50);
      // Once connected, publish an announcement...
      client_mqtt.publish(gateway_id,sample_out);// Gateway Id needed
      // ... and resubscribe
      client_mqtt.subscribe(gateway_id);// Gateway Id needed
    } else {
      Serial.print("failed, rc=");
      Serial.print(client_mqtt.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

void WebHandling()
 {
  // listen for incoming clients
   WiFiClient client = server.available();
  if (client) {
    Serial.println("new client");
    currentLine= "";
    // an http request ends with a blank line
   while (client.connected()) {
      if (client.available()) {
        char c = client.read();
        Serial.write(c);
        // if you've gotten to the end of the line (received a newline
        // character) and the line is blank, the http request has ended,
        // so you can send a reply
        if (c == '\n') {
          // send a standard http response header
          client.println(temporary);
          break;
        }
          else if (c != '\r') {  // if you got anything else but a carriage return character,// add it to the end of the currentLine
          currentLine +=c;
          }
          }
        }
        // give the web browser time to receive the data
        delay(100);

        // close the connection:
        client.stop();
        Serial.println("Client disconnected");
    }
    String c="";int c1=0;
  for(int i=0;i<currentLine.length();i++)
  {
    if(currentLine.charAt(i)!='/')
    c+=currentLine.charAt(i);
    else
    {
      c1++;
      if(c1==2)
      {
      Node_Data.nodeID=c;
      c="";
      }
      else if(c1==3){
      Node_Data.action=c;
      c="";}
      else if(c1==4){
      Node_Data.value=c.toInt();
      c="";}
      else{}
    }
  }
  Node_Data.extras=c;
  String temp=(String)Node_Data.nodeID+"/"+(String)Node_Data.action+"/"+(String)Node_Data.value+"/"+(String)Node_Data.extras;// Extracting Node's Data
  char n[100]="";
  temp.toCharArray(n,sizeof(n));
  snprintf (output, 25, n, 50);  
 }

void setup() {
  Serial.begin(115200);
  Serial.println();
  WiFi.mode(WIFI_AP_STA);
  
  Serial.print("Configuring access point...");
  WiFi.softAP(ap_ssid, ap_password);
  delay(1000);
  server.begin();

  IPAddress myIP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(myIP);

  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(sta_ssid);
  WiFi.begin(sta_ssid, sta_password);
  delay(1000);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

  client_mqtt.setServer(mqtt_server, 1883); //Start up mqqt server client
  client_mqtt.setCallback(callback);
}

void loop() {
  WebHandling();
  client_mqtt.publish(gateway_id,output); // Publish the Node data when we get one.
    delay(2000);
  if (!client_mqtt.connected()) {
    reconnect();
  }
  client_mqtt.loop();
}
