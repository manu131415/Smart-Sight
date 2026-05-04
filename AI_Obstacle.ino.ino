// -------- PIN DEFINITIONS --------
const int trigPin = 6;
const int echoPin = 7;
const int buzzerPin = 8;
const int waterSensorPin = A0;

// -------- SETTINGS --------
int threshold = 20;        // distance (cm)
int waterThreshold = 400; // adjust later

int buzzerDelay;
int buzzerDelayMin = 10;
int buzzerDelayMax = 250;

long duration;
long cm;

void setup() {
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(buzzerPin, OUTPUT);
  pinMode(waterSensorPin, INPUT);

  Serial.begin(9600);
}

void loop() {

  // -------- WATER SENSOR --------
  int waterValue = analogRead(waterSensorPin);

  Serial.print("Water: ");
  Serial.print(waterValue);
  Serial.print("  |  ");

  if (waterValue > waterThreshold) {
    Serial.println("Water Detected!");

    digitalWrite(buzzerPin, HIGH);
    delay(150);
    digitalWrite(buzzerPin, LOW);
    delay(150);

    return;
  }

  // -------- ULTRASONIC --------
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  duration = pulseIn(echoPin, HIGH, 30000);
  cm = duration / 29 / 2;

  Serial.print("Distance: ");
  Serial.print(cm);
  Serial.println(" cm");

  if (cm > 0 && cm < threshold) {
    buzzerDelay = map(cm, 0, threshold, buzzerDelayMin, buzzerDelayMax);

    digitalWrite(buzzerPin, HIGH);
    delay(buzzerDelay);
    digitalWrite(buzzerPin, LOW);
    delay(buzzerDelay);
  } else {
    digitalWrite(buzzerPin, LOW);
  }
}