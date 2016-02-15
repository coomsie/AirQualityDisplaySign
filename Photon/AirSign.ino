///SYSTEM_MODE(SEMI_AUTOMATIC);

// This #include statement was automatically added by the Particle IDE.

#include "ChainableLED.h"

#include "HttpClient.h"
/*
 * Example of using the ChainableRGB library for controlling a Grove RGB.
 * This code cycles through all the colors in an uniform way. This is accomplished using a HSB color space.
 */

#define NUM_LEDS  1

#define CLK D2
#define DIO D3

ChainableLED leds(2, 3, NUM_LEDS);

HttpClient http;
// Headers currently need to be set at init, useful for API keys etc.
http_header_t headers[] = {
    //  { "Content-Type", "application/json" },
    //  { "Accept" , "application/json" },
    { "Content-Type", "application/json" },
    // { "Accept" , "*/*"},
     ///{ "Accept" , "application/json" },
     { "Accept" , "application/csv" },
    { NULL, NULL } // NOTE: Always terminate headers with NULL
};

http_request_t request;
http_response_t response;


void setup()
{
    Serial.begin(9600);
//    Serial.printlnf("System version: %s", System.version().c_str());
    leds.init();
  ///  Particle.connect();

  //  Particle.subscribe("hook-response/get_PM10", gotPM10Data, MY_DEVICES);

}


void loop()
{

 int x = 0;
 while (x < 4){
  setRGBColorfromPM(0);
  delay(500);
  setRGBColorfromPM(25);
  delay(500);
  setRGBColorfromPM(55);
  delay(500);
  x++;
};

  // publish the event that will trigger our Webhook
  //    Particle.publish("get_PM10");

getPM10Data();


      Serial.print(" Data: \n" );
      Serial.println(response.body);

      //get rid of first line;
      int indexOfPM10_0 = response.body.indexOf("\n");
      String line1 = response.body.substring(0,indexOfPM10_0);
      response.body = response.body.substring(indexOfPM10_0+1);
      int indexOfPM10_1 = response.body.indexOf("\n");
      String line2 = response.body.substring(0,indexOfPM10_1);
      response.body = response.body.substring(indexOfPM10_1+1);

      int startIndex = line2.indexOf("PM10 FDMS");
      String pm10 = line2.substring(startIndex+10);

      Serial.print(" PM10 Data: \n" );
      Serial.println(startIndex);
      Serial.println(pm10);
      Serial.println(indexOfPM10_0);

      int pm10_int = 0;
      pm10_int = atoi(pm10.c_str()); //convert to int from String;

      setRGBColorfromPM(pm10_int);

      // and wait at least 60 seconds before doing it again
      delay(60000);

      }

      void getPM10Data()
      {
        //send via http client to ubidots
            // Request path and body can be set at runtime or at setup.
            request.hostname = "data.ecan.govt.nz";
            request.port = 80;
            //JSON request.path = "/data/100/Air/Latest%20Air%20Quality%20Data/JSON?AirMonType=50";
            request.path = "/data/100/Air/Latest%20Air%20Quality%20Data/CSV?AirMonType=50";

            // Get request
            http.get(request, response, headers);
      }

      void setRGBColorfromPM(int value){
        if( value <= 20){
        //GREEN
        int g = 230 + value;
        leds.setColorRGB(0,0, g, 0);
        }
        if( value >= 40){
        //RED
        int r = 200+value;
        leds.setColorRGB(0, r, 0, 0);
        }
        if (value >20 && value < 40){
        //ORANGE
        int o = 125 + o;
        leds.setColorRGB(0, 255,165,0);
        }
      }
