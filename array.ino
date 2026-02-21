// -------- IR Sensors (ESP32 ADC pins) --------
#define S1 13
#define S2 12
#define S3 14
#define S4 27
#define S5 26
#define S6 25
#define S7 33
#define S8 32

// -------- Motor Direction Pins --------
#define IN1 5
#define IN2 18
#define IN3 2
#define IN4 4

// -------- Enable Pins --------
#define ENA 19   // Left motor
#define ENB 15   // Right motor

int TH = 300;

void setup() {
  Serial.begin(115200);

  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);

  pinMode(ENA, OUTPUT);
  pinMode(ENB, OUTPUT);

  digitalWrite(ENA, HIGH);
  digitalWrite(ENB, HIGH);
}

void loop() {

  int raw[8];
  int dig[8];

  raw[0] = analogRead(S1);
  raw[1] = analogRead(S2);
  raw[2] = analogRead(S3);
  raw[3] = analogRead(S4);
  raw[4] = analogRead(S5);
  raw[5] = analogRead(S6);
  raw[6] = analogRead(S7);
  raw[7] = analogRead(S8);

  // Convert to digital
  for (int i = 0; i < 8; i++) {
    dig[i] = (raw[i] < TH) ? 0 : 1;
  }

  // ---- WEIGHTED POSITION ----
  int weights[8] = {0, 1000, 2000, 3000, 4000, 5000, 6000, 7000};

  long sum = 0;
  long count = 0;

  for (int i = 0; i < 8; i++) {
    if (dig[i] == 0) { // black line
      sum += weights[i];
      count++;
    }
  }

  int pos;

  // If no sensor detects line → stop or search
  if (count == 0) {
    stopMotor();
    Serial.println("NO LINE");
    return;
  }

  pos = sum / count;  // Average position

  Serial.print("POS = ");
  Serial.println(pos);

  // ---- DECISION ----

  if (pos > 2800 && pos < 4200) {
    // Center range
    forward();
  }
  else if (pos <= 2800) {
    left();
  }
  else if (pos >= 4200) {
    right();
  }
}


// -------- MOTOR FUNCTIONS --------
void forward() {
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
}

void left() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
}

void right() {
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
}

void stopMotor() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);
}
