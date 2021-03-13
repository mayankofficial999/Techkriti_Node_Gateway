#include <WiFi.h>
#include <HTTPClient.h>
#include <Adafruit_Sensor.h>
#include <DHT.h>
#define DHTPIN 5 
#define DHTTYPE    DHT11

DHT dht(DHTPIN, DHTTYPE);

/* Set these to your desired credentials. */
const char *ssid = "ESP8266";  //ENTER YOUR WIFI SETTINGS
const char *password = "12345678";
String temporary;
//Web/Server address to read/write from 
const char *host = "192.168.4.1";   //https://circuits4you.com website or IP address of server
String action_update;
float temp;
int relayPin_2 = 8;
int relayPin=10; //relayPin 

typedef struct struct_message {
  String nodeID;
  int value;
  String action;
  int extras;
} struct_message;

struct_message Server_Data;
struct_message Sensor_Data;

//=======================================================================
//                    Power on setup
//=======================================================================
void Extract(String Data)
{
  //Extracting Data from the server.
    String c="";int c1=0;
  for(int i=0;i<Data.length();i++)
  {
    if(Data.charAt(i)!='/')
    c+=Data.charAt(i);
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
  
  action_update = Server_Data.action; //Updating action value only (Others are not nessessary)
  RelayChangeState();
  temporary=(String)Server_Data.nodeID+"/"+(String)Server_Data.action+"/"+(String)Server_Data.value+"/"+(String)Server_Data.extras;// Extracting Node's Data
  Serial.println(temporary);
}

void RelayChangeState()
{
  if((Server_Data.action).equals(action_update)==false)
  {
  if((Server_Data.action).equals("ON")||(Server_Data.action).equals("AUTO")){
  digitalWrite(relayPin,HIGH);
  delay(100);}
  else if((Server_Data.action).equals("OFF")){
  digitalWrite(relayPin,LOW);
  delay(100);}
  else
  Serial.println("Invalid Relay State !");
  Server_Data.action=action_update;
  }
}
void getReadings(){ //Sensor Readings
  // Read Temperature
   temp= random(100);
   Sensor_Data.value=temp;
  // Read temperature as Fahrenheit (isFahrenheit = true)
  //float t = dht.readTemperature(true);
  if (isnan(temp)){
    Serial.println("Failed to read from DHT");
    temp = 0.0;
  }
}

void activate_fan(){//for activating fan
  if (Sensor_Data.action=="ON"){
    digitalWrite(relayPin_2,HIGH);
    else{
      digitalWrite(relayPin_2,LOW);
    }
  }
}
void activate_led(){
  ldrval = analogRead(ldr);
ledval = map(ldrval,0,1023,0,255);
analogWrite(led,ledval);
}
void setup() {
  if (sensor_Data.nodeID == "12g")
  {
    Sensor_Data.nodeID="12g";
    Sensor_Data.action="OFF";
    Sensor_Data.value=30;
    Sensor_Data.extras=100;
  }
  else if (sensor_Data.nodeID=="12u"
  {
    Sensor_Data.nodeID="12u";
    Sensor_Data.action="OFF";
    Sensor_Data.value=30;
    Sensor_Data.extras=100; 
  }
  delay(1000);
  Serial.begin(115200);
  dht.begin();
  WiFi.mode(WIFI_OFF);        //Prevents reconnection issue (taking too long to connect)
  delay(1000);
  WiFi.mode(WIFI_STA);        //This line hides the viewing of ESP as wifi hotspot
  
  WiFi.begin(ssid, password);     //Connect to your WiFi router
  Serial.println("");

  Serial.print("Connecting");
  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  //If connection successful show IP address in serial monitor
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());  //IP address assigned to your ESP
}

//=======================================================================
//                    Main Program Loop
//=======================================================================
void loop() {
  getReadings();
  if (sensor_Data.nodeID == "12g")
  {
    activate_led();
  }
  else if (sensor_Data.nodeID == "12u")
  {
    activate_fan();
  }
  
  HTTPClient http;    //Declare object of class HTTPClient

  String getData="192.168.4.1/"+(String)Sensor_Data.nodeID+"/"+(String)Sensor_Data.action+"/"+(String)Sensor_Data.value+"/"+(String)Sensor_Data.extras;
  
  http.begin(getData);     //Specify request destination
  
  int httpCode = http.GET();            //Send the request
  String payload = http.getString();    //Get the response payload

  Serial.println(httpCode);   //Print HTTP return code
  Serial.println(payload);    //Print request response payload
  Extract(payload);

  http.end();  //Close connection
  
  delay(100);  //GET Data at every 100 mili seconds
}
