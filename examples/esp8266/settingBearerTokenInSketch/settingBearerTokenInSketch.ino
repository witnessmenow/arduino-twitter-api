#include <TwitterApi.h>

/*******************************************************************
 *  An example of usisng the distance-matrix api to get            *
 *  travel time (with traffic) between two locations               *
 *                                                                 *
 *  Written by Brian Lough                                         *
 *******************************************************************/


#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>

//------- Replace the following! ------
char ssid[] = "SSID";       // your network SSID (name)
char password[] = "password";  // your network key

// Normally we would use these to generate the bearer token but its not working yet :/
#define CONSUMER_KEY "consumerkey" //not needed for this sketch as we are setting bearer token directly
#define CONSUMER_SECRET "consumerSecret" // as above

#define BEARER_TOKEN "this is long jumble of characters"

//Using curl to get bearer token
// curl -u "$CONSUMER_KEY:$CONSUMER_SECRET" \
//    --data 'grant_type=client_credentials' \
//    'https://api.twitter.com/oauth2/token'

WiFiClientSecure client;
TwitterApi api(CONSUMER_KEY, CONSUMER_SECRET, client);

unsigned long api_mtbs = 60000; //mean time between api requests
unsigned long api_lasttime = 0;   //last time api request has been done
bool firstTime = true;

//Inputs

String screenName = "witnessmenow";
bool haveBearerToken = false;

void setup() {

  Serial.begin(115200);

  // Set WiFi to station mode and disconnect from an AP if it was Previously
  // connected
  WiFi.mode(WIFI_STA);
  WiFi.disconnect();
  delay(100);

  // Attempt to connect to Wifi network:
  Serial.print("Connecting Wifi: ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  IPAddress ip = WiFi.localIP();
  Serial.println(ip);

//  if(api.encodeBearerCredientials()) {
//    Serial.println("Encoded bearer creds");
//  } else {
//    Serial.println("Failed to encode bearer creds");
//    return;
//  }
//
//  if(api.updateBearerToken()) {
//    Serial.println("Got bearer token");
//    haveBearerToken = true;
//  } else {
//    Serial.println("Did not get bearer token");
//  }

  api.setBearerToken(BEARER_TOKEN);
  haveBearerToken = true;
  getTwitterStats(screenName);

}

void getTwitterStats(String name) {
  Serial.println("Getting twitter stats for " + name);
    String responseString = api.getUserStatistics(name);
    Serial.println(responseString);
    DynamicJsonBuffer jsonBuffer;
    JsonObject& response = jsonBuffer.parseObject(responseString);
    if (response.success()) {
      Serial.println("parsed Json");
    } else {
      Serial.println("Failed to parse Json");
    }
}

void loop() {
  if(haveBearerToken){
    if ((millis() > api_lasttime + api_mtbs))  {
      getTwitterStats(screenName);
      api_lasttime = millis();
    }
  }
}
