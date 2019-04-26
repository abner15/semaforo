#include <GoogleMapsApi.h>
#include <ArduinoJson.h>
#include <WiFi.h>
#include <WiFiClientSecure.h>

const char* ssid = "prueba1";                                                       // your network SSID (name)
const char* password = "aA7365775";                                               // your network key
#define API_KEY "AIzaSyDxX2d8Is6eri6LN15dwCBDeH-utr_wTW0"                       // google apps API Token

WiFiClientSecure client;
GoogleMapsApi api(API_KEY, client);

//Free Google Maps Api only allows for 2500 "elements" a day, so careful you dont go over
long mapDueTime;
const int mapDueDelay = 60000;
const int mapDueDelayIfFailed = 5000;

//Entrada 1
String origin = "Galway,+Ireland";
String destination = "Dublin,+Ireland";
String displayText = "Galway -> Dublin";
/*
//Entrada 2
String origin = "Galway,+Ireland";
String destination = "Dublin,+Ireland";
String displayText = "Galway -> Dublin";
//Entrada 3
String origin = "Galway,+Ireland";
String destination = "Dublin,+Ireland";
String displayText = "Galway -> Dublin";
//Entrada 4
String origin = "Galway,+Ireland";
String destination = "Dublin,+Ireland";
String displayText = "Galway -> Dublin";
*/
//Optional
String departureTime = "now"; //This can also be a timestamp, needs to be in the future for traffic info
String trafficModel = "best_guess"; //defaults to this anyways. see https://developers.google.com/maps/documentation/distance-matrix/intro#DistanceMatrixRequests for more info

String durationInTraffic;
int differenceInMinutes;
float percentageDifference;

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

  delay(1000);
}
bool checkGoogleMaps() {
  Serial.println("Getting traffic for " + origin + " to " + destination);
  /*Serial.println("Getting traffic for " + origin2 + " to " + destination2);
  Serial.println("Getting traffic for " + origin3 + " to " + destination3);
  Serial.println("Getting traffic for " + origin4 + " to " + destination4);
  */  
    String responseString = api.distanceMatrix(origin, destination, departureTime, trafficModel);
    /*
    String responseString2 = api.distanceMatrix(origin2, destination2, departureTime, trafficModel);
    String responseString3 = api.distanceMatrix(origin3, destination3, departureTime, trafficModel);
    String responseString4 = api.distanceMatrix(origin4, destination4, departureTime, trafficModel);
    */
    DynamicJsonBuffer jsonBuffer;
    JsonObject& response = jsonBuffer.parseObject(responseString);
    /*
    JsonObject& response2 = jsonBuffer.parseObject(responseString2);
    JsonObject& response3 = jsonBuffer.parseObject(responseString3);
    JsonObject& response4 = jsonBuffer.parseObject(responseString4);
    */
    if (response.success()){
    //if (response.success() and response2.success() and response3.success() and response4.success()) {
      if (response.containsKey("rows")){
      //if (response.containsKey("rows") and response2.containsKey("rows") and response3.containsKey("rows") and response4.containsKey("rows") ) {
        JsonObject& element = response["rows"][0]["elements"][0];
      /*
        JsonObject& element2 = response2["rows"][0]["elements"][0];
        JsonObject& element3 = response2["rows"][0]["elements"][0];
        JsonObject& element4 = response2["rows"][0]["elements"][0];
       */
        String status = element["status"];
      /*
        String status2 = element2["status"];
        String status3 = element3["status"];
        String status4 = element4["status"];
        */
        //Debug
        if ((status) == "OK");{
        //if((status) and (status2) and (status3) and (status4) == "OK") {

          durationInTraffic = element["duration_in_traffic"]["text"].as<String>();
          /* //consulta 2
          durationInTraffic2 = element2["duration_in_traffic"]["text"].as<String>();
          //consulta 3
          durationInTraffic3 = element3["duration_in_traffic"]["text"].as<String>();
          //consulta 4
          durationInTraffic4 = element4["duration_in_traffic"]["text"].as<String>();
          */
          //consulta 1 - Tiempo Duro
          int durationInSeconds = element["duration"]["value"];
          int durationInTrafficInSeconds = element["duration_in_traffic"]["value"];
          int difference = durationInSeconds - durationInTrafficInSeconds;
          
          /* //consulta 2 - Tiempo Duro
          int durationInSeconds2 = element2["duration"]["value"];
          int durationInTrafficInSeconds2 = element2["duration_in_traffic"]["value"];
          int difference2 = durationInSeconds2 - durationInTrafficInSeconds2;
          
          //consulta 3 - Tiempo Duro
          int durationInSeconds3 = element3["duration"]["value"];
          int durationInTrafficInSeconds3 = element3["duration_in_traffic"]["value"];
          int difference3 = durationInSeconds3 - durationInTrafficInSeconds3;

          //consulta 4 - Tiempo Duro
          int durationInSeconds4 = element4["duration"]["value"];
          int durationInTrafficInSeconds4 = element4["duration_in_traffic"]["value"];
          int difference4 = durationInSeconds4 - durationInTrafficInSeconds4;
          */
          //consulta 1 - Tiempo modificado
          differenceInMinutes = difference/60;
          percentageDifference = (difference * 100.0) / durationInSeconds;
          Serial.println("Duration In Traffic: " + durationInTraffic + "("+ percentageDifference +")"+"(" + durationInTrafficInSeconds + ")");
        
          /*consulta 2 - Tiempo modificado
          differenceInMinutes2 = difference2/60;
          percentageDifference2 = (difference2 * 100.0) / durationInSeconds2;
          Serial.println("Duration In Traffic: " + durationInTraffic2 + "(" + durationInTrafficInSeconds2 + ")");
         
          //consulta 3 - Tiempo modificado
          differenceInMinutes2 = difference3/60;
          percentageDifference2 = (difference3 * 100.0) / durationInSeconds3;
          Serial.println("Duration In Traffic: " + durationInTraffic3 + "(" + durationInTrafficInSeconds3 + ")");
          
          //consulta 4 - Tiempo modificado
          differenceInMinutes4 = difference4/60;
          percentageDifference4 = (difference4 * 100.0) / durationInSeconds4;
          Serial.println("Duration In Traffic: " + durationInTraffic4 + "(" + durationInTrafficInSeconds4 + ")");
            */
          return true;

        }
        else {
          Serial.println("Got an error status: " + status);
          return false;
        }
      } else {
        Serial.println("Reponse did not contain rows");
        return false;
      }
    } else {
      if(responseString == ""){
        Serial.println("No response, probably timed out");
      } else {
        Serial.println("Failed to parse Json. Response:");
        Serial.println(responseString);
      }
      if(responseString2 == ""){
        Serial.println("No response, probably timed out");
      } else {
        Serial.println("Failed to parse Json. Response:");
        Serial.println(responseString2);
      }
      if(responseString3 == ""){
        Serial.println("No response, probably timed out");
      } else {
        Serial.println("Failed to parse Json. Response:");
        Serial.println(responseString3);
      }
      if(responseString3 == ""){
        Serial.println("No response, probably timed out");
      } else {
        Serial.println("Failed to parse Json. Response:");
        Serial.println(responseString3);
      }

      return false;
    }

    return false;
}


void loop() {

  long now = millis();
  if (now > mapDueTime)  {
    Serial.println("Checking google maps");
    if (checkGoogleMaps()) {
      mapDueTime = now + mapDueDelay;
    } else {
      mapDueTime = now + mapDueDelayIfFailed;
    }
  }
  delay(500);
}
