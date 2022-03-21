const int PIN_INPUT = 2;
const int PIN_MOTOR = 14;

const int INPUT_RANGE[] = {1161, 4095};
const int OUTPUT_RANGE[] = {128, 25};

int minValue=4096;
int maxValue=0;

void setup() {
  // put your setup code here, to run once:
  pinMode(PIN_INPUT, INPUT);

  ledcSetup(0, 50, 10);
  ledcAttachPin(PIN_MOTOR,0);
  
  Serial.begin(115200);
}

void loop() {
  // put your main code here, to run repeatedly:
  int r = analogRead(PIN_INPUT);
  int m = map(r, INPUT_RANGE[0], INPUT_RANGE[1], OUTPUT_RANGE[0], OUTPUT_RANGE[1]);

  ledcWrite(0, m);
  
  Serial.print(r);Serial.print(";");Serial.print(m);Serial.println();

  delay(100);
}
