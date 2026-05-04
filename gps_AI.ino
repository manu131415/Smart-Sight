#include <SoftwareSerial.h>
#include <TinyGPS++.h>

SoftwareSerial gpsSerial(4, 3);
SoftwareSerial gsmSerial(7, 8);

TinyGPSPlus gps;

String phoneNumber = "+9176000XXXXX"; // Add your phone number
bool smsSent = false;

void setup() {
  Serial.begin(9600);
  gpsSerial.begin(9600);
  gsmSerial.begin(9600);

  Serial.println("System Starting...");
  delay(3000);

  gsmSerial.listen();
  gsmSerial.println("AT");
  delay(1000);

  while (gsmSerial.available()) {
    Serial.write(gsmSerial.read());
  }

  Serial.println("Waiting for GPS...");
}

void loop() {
  gpsSerial.listen();

  while (gpsSerial.available() > 0) {
    gps.encode(gpsSerial.read());
  }

  if (gps.location.isUpdated()) {
    float lat = gps.location.lat();
    float lng = gps.location.lng();

    Serial.print("Latitude: ");
    Serial.println(lat, 6);

    Serial.print("Longitude: ");
    Serial.println(lng, 6);

    if (!smsSent) {
      sendSMS(lat, lng);
      smsSent = true;
    }
  } else {
    Serial.println("No GPS fix yet...");
    delay(2000);
  }
}

void sendSMS(float lat, float lng) {
  gsmSerial.listen();

  gsmSerial.println("AT");
  delay(1000);

  gsmSerial.println("AT+CMGF=1");
  delay(1000);

  gsmSerial.print("AT+CMGS=\"");
  gsmSerial.print(phoneNumber);
  gsmSerial.println("\"");
  delay(3000);

  gsmSerial.print("Help! My location: https://maps.google.com/?q=");
  gsmSerial.print(lat, 6);
  gsmSerial.print(",");
  gsmSerial.print(lng, 6);

  gsmSerial.write(26);
  delay(5000);

  while (gsmSerial.available()) {
    Serial.write(gsmSerial.read());
  }

  Serial.println("SMS command sent.");
}
