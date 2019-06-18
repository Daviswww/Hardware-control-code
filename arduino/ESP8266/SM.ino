#include <ESP8266WiFi.h> 
#include <PubSubClient.h>

const char* ssid =  "E102";
const char* password = "000000000";
const char* mqttServer = "140.126.20.95";  // MQTT伺服器位址
const char* mqttUserName =  "DDA";  // 使用者名稱，隨意設定。
const char* mqttPwd ="12312311";  // MQTT密碼
const char* clientID = "f76e";      // 用戶端ID，隨意設定。
const char* topic = "12345";
char* sss = "";
//-----------setup---------
//daviswww
String url = "/DB/script/ins.php?";
const char* host = "140.126.21.185";  //傳送IP
const uint16_t port = 80;
//-----------setup---------
WiFiClient client;

 
void callback(char*topic, byte* payload, unsigned int length) {
  for (int i = 0; i< length; i++) {
    sss[i] = payload[i];
  }
  Serial.write(sss);
}

void reconnect() {
  while (!client.connected()) {
    if (client.connect(clientID, mqttUserName, mqttPwd)) {
    } else {
      delay(5000);  // 等5秒之後再重試
    }
  }
}


void setup_wifi() {
  delay(10);
  WiFi.begin(ssi, psw);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
  }
}

void setup() {
  Serial.begin(57600);
  setup_wifi();
  client.setServer(mqttServer,1883);
  if (!client.connected()) {
    reconnect();
  }
  client.setCallback(callback);
  client.subscribe(topic);
  client.publish("qq","OK!");
} 
 
void loop() 
{   
  char  topic_temperature[5]={};
  char  topic_air_humidity[5]={};
  char  topic_soil_humidity [5]={};
  char i=0,s_value=0,a_value=0,t_value=0;
  char a[10]={};
  //--------sql--------
  String data = "";
  WiFiClient client;
  if (!client.connect(host, port)) {
    Serial.println("connection failed");
    return;
  }
  //--------sql--------
  client.loop();
  while(Serial.available()) 
  {  
    char c = Serial.read();   
    a[i]=c;
    i++;
    if(a[4]==0Xff)
    { 
      if(a[0]==0x40)
      {                                       
        s_value=a[1];
        a_value=a[2];
        t_value=a[3];
        sprintf(topic_soil_humidity,"%d",s_value);
        sprintf(topic_air_humidity,"%d",a_value);
        sprintf(topic_temperature,"%d",t_value);
        client.publish("m1/sh",topic_soil_humidity);
        client.publish("m1/ah",topic_air_humidity);
        client.publish("m1/tp",topic_temperature);
        data += "a=";
        data += a[1];
        data += "&b=";
        data += a[2];
        data += "&c=";
        data += a[3];  
        client.print(String("GET ") + url + data + " HTTP/1.1\r\n" + "Host: " + host + "\r\n"+"Connection: close\r\n\r\n"); 
      }
      else if(a[0]==0x41)
      {                                       
         s_value=a[1];
         a_value=a[2];
         t_value=a[3];
         sprintf(topic_soil_humidity,"%d",s_value);
         sprintf(topic_air_humidity,"%d",a_value);
         sprintf(topic_temperature,"%d",t_value);
         client.publish("m2/sh",topic_soil_humidity);
         client.publish("m2/ah",topic_air_humidity);
         client.publish("m2/tp",topic_temperature);
      }  
    }
    delay(2); 
  }   
}
