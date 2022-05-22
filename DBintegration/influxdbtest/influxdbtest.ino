#include <WiFiMulti.h>
WiFiMulti wifiMulti;
#define DEVICE "ESP32_1"

#include <InfluxDbClient.h>
#include <InfluxDbCloud.h>

#define WIFI_SSID "nobel"
#define WIFI_PASSWORD "sarobert"

//InfluxDB
#define INFLUXDB_URL "https://europe-west1-1.gcp.cloud2.influxdata.com"
#define INFLUXDB_TOKEN "KlmcztU1dEWZ04JzoiK3eg2eBtZxH-LbbYauBEH0UGpdmfjqxGvwIpaAIXv8EqTwIiXD-Hs0O3Konu3NjyrQfA=="
#define INFLUXDB_ORG "nobel2002@gmail.com"
#define INFLUXDB_BUCKET "nobel2002's Bucket"
#define TZ_INFO "IST"


//Definitions
InfluxDBClient client(INFLUXDB_URL, INFLUXDB_ORG, INFLUXDB_BUCKET, INFLUXDB_TOKEN, InfluxDbCloud2CACert);

Point sensor1("pollutionReading");
Point sensor2("pollutionReading");
Point sensor3("pollutionReading");

void influxInit(){
    WiFi.mode(WIFI_STA);
  wifiMulti.addAP(WIFI_SSID, WIFI_PASSWORD);

  Serial.print("Connecting to wifi");
  while (wifiMulti.run() != WL_CONNECTED) {
    Serial.print(".");
    delay(100);
  }
  Serial.println();

  sensor1.addTag("sensor", DEVICE);
  sensor1.addTag("pollutant", "CO2");

  sensor2.addTag("sensor", DEVICE);
  sensor2.addTag("pollutant", "CO");

  sensor3.addTag("sensor", DEVICE);
  sensor3.addTag("pollutant", "NH4");

  timeSync(TZ_INFO, "pool.ntp.org", "time.nis.gov");

  if (client.validateConnection()) {
    Serial.print("Connected to InfluxDB: ");
    Serial.println(client.getServerUrl());
  } else {
    Serial.print("InfluxDB connection failed: ");
    Serial.println(client.getLastErrorMessage());
  }
}

void setup() {
  //Init the serial port communication - to debug the library
  Serial.begin(9600); //Init serial port

  influxInit();
}

void loop() {
  sensor1.clearFields();
  sensor2.clearFields();
  sensor3.clearFields();

sensor1.addField("reading",123);
  Serial.print("Writing: ");
  Serial.println(sensor1.toLineProtocol());
  if (!client.writePoint(sensor1)) {
    Serial.print("InfluxDB write failed: ");
    Serial.println(client.getLastErrorMessage());
  }

  sensor2.addField("reading",300);
  Serial.print("Writing: ");
  Serial.println(sensor2.toLineProtocol());
  if (!client.writePoint(sensor2)) {
    Serial.print("InfluxDB write failed: ");
    Serial.println(client.getLastErrorMessage());
  }

  sensor3.addField("reading",400);
  Serial.print("Writing: ");
  Serial.println(sensor3.toLineProtocol());
  if (!client.writePoint(sensor3)) {
    Serial.print("InfluxDB write failed: ");
    Serial.println(client.getLastErrorMessage());
  }

  Serial.println("Wait 1s");

    
  /*
  Serial.print("|   "); Serial.print(CO); 
  Serial.print("   |   "); Serial.print(Alcohol); 
  Serial.print("   |   "); Serial.print(CO2+415.08); 
  Serial.print("   |   "); Serial.print(Tolueno); 
  Serial.print("   |   "); Serial.print(NH4); 
  Serial.print("   |   "); Serial.print(Acetona);
  Serial.println("   |"); 
  */
  /*
    Exponential regression:
  GAS      | a      | b
  CO       | 605.18 | -3.937  
  Alcohol  | 77.255 | -3.18 
  CO2      | 110.47 | -2.862
  Tolueno  | 44.947 | -3.445
  NH4      | 102.2  | -2.473
  Acetona  | 34.668 | -3.369
  */

  //delay(1); //Sampling frequency
}
