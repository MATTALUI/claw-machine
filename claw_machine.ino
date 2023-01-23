// Controller pins
const int  UP_PIN = 9;
const int  DOWN_PIN = 10;
const int  LEFT_PIN = 8;
const int  RIGHT_PIN = 11;
const int  PLUNGER_PIN = 12;
// Motor pins
const int HOR_MOTOR_PIN = 4;
const int HOR_MOTOR_DIRECTION_PIN = 3;
const int VERT_MOTOR_PIN = 5;
const int VERT_MOTOR_DIRECTION_PIN = 6;
// Other Pins
const int COIN_OP_PIN = 2;
const int MACHINE_LIGHT_PIN = 7;
// Coin Data
// TODO: Reprogram these to be something more reliable
const int QUARTER_PULSE_COUNT = 1;
const int QUARTER_CREDIT_VALUE = 25;
const int DIME_PULSE_COUNT = 2;
const int DIME_CREDIT_VALUE = 10;
const int NICKEL_PULSE_COUNT = 3;
const int NICKEL_CREDIT_VALUE = 5;
// Limits
const int MAX_X_MOVEMENT = 150;
const int MAX_Y_MOVEMENT = 150;
const int REQUIRED_CREDITS = 50;
const int COIN_TIME_COUNTER_TIMEOUT = 10;
// Global values
int xCoord;
int yCoord;
int creditValue;
volatile int coinTimeCounter;
volatile int coinPulseCounter;
bool controlsActive;

void setup() {
	// Initialize pins
	pinMode(UP_PIN, INPUT);
	pinMode(DOWN_PIN, INPUT);
	pinMode(LEFT_PIN, INPUT);
	pinMode(RIGHT_PIN, INPUT);
	pinMode(PLUNGER_PIN, INPUT);
	pinMode(COIN_OP_PIN, INPUT);
  pinMode(COIN_OP_PIN, INPUT);
	pinMode(MACHINE_LIGHT_PIN, OUTPUT);
  pinMode(HOR_MOTOR_PIN, OUTPUT);
  pinMode(VERT_MOTOR_PIN, OUTPUT);
  pinMode(HOR_MOTOR_DIRECTION_PIN, OUTPUT);
  pinMode(VERT_MOTOR_DIRECTION_PIN, OUTPUT);
	// Initialize Serial
	Serial.begin(9600);
	// Initialize global values
	xCoord = 0;
	yCoord = 0;
	creditValue = 0;
	controlsActive = false;
  coinTimeCounter = 0;
  coinPulseCounter = 0;

  attachInterrupt(digitalPinToInterrupt(COIN_OP_PIN), insertCoin, RISING);
}

void loop() {
  if (controlsActive && digitalRead(PLUNGER_PIN) == HIGH) {
    digitalWrite(MACHINE_LIGHT_PIN, HIGH);
		plunge();
	} else if (controlsActive) {
    digitalWrite(MACHINE_LIGHT_PIN, HIGH);
		if (digitalRead(LEFT_PIN) == HIGH) goLeft();
		if (digitalRead(UP_PIN) == HIGH) goUp();
		if (digitalRead(DOWN_PIN) == HIGH) goDown();
		if (digitalRead(RIGHT_PIN) == HIGH) goRight();
	} else {
    digitalWrite(MACHINE_LIGHT_PIN, LOW);
		checkCoins();
	}
}

void goLeft() {
	if (xCoord <= 0) return;
	Serial.println("GO LEFT");
	xCoord--;
  digitalWrite(HOR_MOTOR_DIRECTION_PIN, HIGH);
  moveHorMotor();
	if (xCoord == 0) Serial.println("You have hit the X min.");
}

void goRight() {
	if (xCoord >= MAX_X_MOVEMENT) return;
	Serial.println("GO RIGHT");
	xCoord++;// TODO: Move motor
  digitalWrite(HOR_MOTOR_DIRECTION_PIN, LOW);
  moveHorMotor();
	if (xCoord == MAX_X_MOVEMENT) Serial.println("You have hit the X max.");
}

void goDown() {
	if (yCoord <= 0) return;
	Serial.println("GO Down");
	yCoord--;
  digitalWrite(VERT_MOTOR_DIRECTION_PIN, HIGH);
  moveVertMotor();
	if (yCoord == 0) Serial.println("You have hit the Y min.");
}

void goUp() {
	if (yCoord >= MAX_Y_MOVEMENT) return;
	Serial.println("GO Up");
	yCoord++;
  digitalWrite(VERT_MOTOR_DIRECTION_PIN, LOW);
  moveVertMotor();
	if (yCoord == MAX_Y_MOVEMENT) Serial.println("You have hit the Y max.");
}

void plunge() {
  controlsActive = false;
	Serial.println("Plunge!");
	// TODO: Run plunging routine
	delay(2000);
	Serial.println("Done Plunging");
  Serial.println("Return to 0!");
	// TODO: Run return to 0 routine
	delay(2000);
	Serial.println("Done Returning");
  // TODO: Drop Prize
  Serial.println("Dropping Prize");
	// TODO: See what it would take to play a little chiptune
}

void checkCoins() {
  coinTimeCounter++;
  if (coinTimeCounter >= COIN_TIME_COUNTER_TIMEOUT) {
    if (coinPulseCounter > 0){
      creditValue += getPulseCreditValue(coinPulseCounter);
      Serial.println(creditValue);
	    if (creditValue >= REQUIRED_CREDITS){
        creditValue -= REQUIRED_CREDITS;
        digitalWrite(MACHINE_LIGHT_PIN, HIGH);
        controlsActive = true;
      }
    }
    coinTimeCounter = 0;
    coinPulseCounter = 0;
  }
  delay(30);
}

void insertCoin() {
  coinTimeCounter = 0;
  coinPulseCounter++;
}

int getPulseCreditValue(int pulseCount) {
  switch(pulseCount) {
    case QUARTER_PULSE_COUNT:
      return QUARTER_CREDIT_VALUE;
    case DIME_PULSE_COUNT:
      return DIME_CREDIT_VALUE;
    case NICKEL_PULSE_COUNT:
      return NICKEL_CREDIT_VALUE;
    default:
      return 0;
  }
}

void moveHorMotor(){
  moveMotor(HOR_MOTOR_PIN);
}

void moveVertMotor(){
  moveMotor(VERT_MOTOR_PIN);
}

void moveMotor(int pin) {
  digitalWrite(pin, HIGH);
  delay(3);
  digitalWrite(pin, LOW);
  delay(3);
}