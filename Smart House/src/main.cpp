#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <PubSubClient.h>
//WIFI UserID/Password
const char *ssid = "AndroidAP";
const char *password = "9313150219";

//MQTT Credentials
const char *mqttServer = "m15.cloudmqtt.com";
const int mqttPort = 13388;
const char *mqttUser = "nffzqxes";
const char *mqttPassword = "6vvaEm4-G6pL";

WiFiClient espClient;
PubSubClient client(espClient);

//#define LED_BUILTIN 0

void ledON()
{
    Serial.print("Calling....Light ON");
    digitalWrite(LED_BUILTIN, LOW);
}

void ledOFF()
{
    Serial.print("Calling....Light OFF");
    digitalWrite(LED_BUILTIN, HIGH);
}

void callback(char *topic, byte *payload, unsigned int length)
{

    Serial.print("Message arrived in topic: ");
    Serial.println(topic);
    payload[length] = '\0';
    String s = String((char *)payload);
    Serial.print(s);
    String stringTopicValue = String("LightOn");

    if (s.equals(stringTopicValue))
    {
        ledON();
    }
    else
    {
        ledOFF();
    }

    // Serial.print("Message:");
    // for (int i = 0; i < length; i++)
    // {
    //     Serial.print((char)payload[i]);
    // }

    Serial.println();
    Serial.println("-----------------------");
}

void reconnect()
{
    // Loop until we're reconnected
    while (!client.connected())
    {
        Serial.print("Attempting MQTT connection...");
        // Attempt to connect
        if (client.connect("ESP8266Client", mqttUser, mqttPassword))
        {
            Serial.println("connected");
            // ... and subscribe to topic
            client.subscribe("Smart House");
        }
        else
        {
            Serial.print("failed, rc=");
            Serial.print(client.state());
            Serial.println(" try again in 5 seconds");
            // Wait 5 seconds before retrying
            delay(5000);
        }
    }
}

void setup()
{
    Serial.begin(115200);
    pinMode(LED_BUILTIN, OUTPUT);
    digitalWrite(LED_BUILTIN, HIGH);
    Serial.println("Connecting to ");
    Serial.println(ssid);
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED)
    {
        delay(500);
        Serial.print(".");
    }
    Serial.println("");
    Serial.println("WiFi connected");
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());

    client.setServer(mqttServer, mqttPort);
    client.setCallback(callback);

    //Connect to MQTT
    while (!client.connected())
    {
        Serial.println("Connecting to MQTT...");

        if (client.connect("ESP8266Client", mqttUser, mqttPassword))
        {

            Serial.println("connected");
        }
        else
        {

            Serial.print("failed with state ");
            Serial.print(client.state());
            delay(2000);
        }
    }

    client.publish("Smart House", "Hello from ESP8266");
    client.subscribe("Smart House");

    // put your setup code here, to run once:
    //pinMode(LED_BUILTIN, OUTPUT);
}

void loop()
{
    if (!client.connected())
    {
        reconnect();
    }
    client.loop();
}