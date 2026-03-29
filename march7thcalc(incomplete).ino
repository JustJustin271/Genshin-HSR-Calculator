#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <avr/pgmspace.h>

//Defines the size of the screen
LiquidCrystal_I2C lcd(0x27, 16, 2); 

const int powerPin = 12;
const int calcPin = 11;

const int pressPin = 10;
const int optPin = 9;

const int signPin = 8;

const int onePin = 7;
const int fivePin = 6;
const int tenPin = 5;

bool power = false;

int mode = 0;

int pulls = 0;

float odds = 0;

float table[] = {
  0.39806,
  0.47390,
  0.57176,
  0.67711,
  0.77591,
  0.85793,
  0.91845,
  0.95808,
  0.98097,
  0.99250,
  0.99750,
  0.99931,
  0.99985,
  0.99998,
  0.99999,
  0.99999
};

void setup() {
  lcd.init();
  lcd.noBacklight();

  pinMode(powerPin, INPUT_PULLUP);
  pinMode(calcPin, INPUT_PULLUP);

  pinMode(pressPin, INPUT_PULLUP);
  pinMode(optPin, INPUT_PULLUP);

  pinMode(signPin, INPUT_PULLUP);

  pinMode(onePin, INPUT_PULLUP);
  pinMode(fivePin, INPUT_PULLUP);
  pinMode(tenPin, INPUT_PULLUP);

  // put your setup code here, to run once:

}

void loop() {
  // put your main code here, to run repeatedly:

  if (digitalRead(powerPin) == LOW) {
    power = !power;
    lcd.clear();
    
    if(power) { 
      lcd.init(); 
      lcd.backlight();
    } 
    else { 
      lcd.noBacklight(); 
      mode = 0;
    }
    delay(150);
  }


}

float cumulative (int pulls) {
  if (pulls == 0){
    return 0.00000;
  } else if (pulls == 90) {
    return 1.00000;
  } else if ((pulls >= 1) && (pulls <= 73)) {
    return 1 - pow(0.996, pulls);
  } else if ((pulls >= 74) && (pulls <= 89)) {
    return table[pulls - 74];
  } else {
    return 0.40404; //Error
  }
}

float pessimistic (int pulls) {
  if (pulls)
}

/*
On/Off ~ 1 button
(Turning off, then back on will always take you to the welcome screen)

Calculate button ~ 1 button

Welcome Screen:
Genshin/HSR Calc
Calc to start!

~ Press Calculate to begin

Asks for pessimistic/optimistic ~ 2 buttons

~ Press Calculate to confirm

Calculate Screen:
  Pulls: N ~ M: O/P
  Calc when done!

Incrementation buttons:
+1, +5, +10 buttons ~ 3 buttons
Negative/Positive Toggle ~ 1 Button

~ Press Calculate when done adding odds

Calculating Loading Screen:
  Calculating Odds
  ......

(The 7 dots would appear on screen 1 by 1)
~ Wait e seconds

Calculation Screen:
  Your odds are:
  xxx.xxx%

~ Press Calculate when done to take you back to the Welcome Screen

*/



/*
Formula for Optimisitic:
c(N)
Normal look up/semi calculate

Pessimistic:
Sum of P ( (exactly X) * C(Pulls - X) )

Pulls is an input

X is defined as 
1 <= X <= Pulls

If pulls = 0 or 90, there are already predetermined cases such as 0.0 and 1.0
*/

//March 29th, 2026

