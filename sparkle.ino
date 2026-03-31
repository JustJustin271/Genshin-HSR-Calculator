#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <avr/pgmspace.h>

//Defines the size of the screen
LiquidCrystal_I2C lcd(0x27, 16, 2); 

const int powerPin = 12;
const int calcPin = 11;

const int pessPin = 10;
const int optPin = 9;

const int signPin = 8;

const int onePin = 7;
const int fivePin = 6;
const int tenPin = 5;

bool power = false;

int mode = 0;

int pulls = 0;

bool lastCalcState = HIGH;

bool lastOneState = HIGH;
bool lastFiveState = HIGH;
bool lastTenState = HIGH;

bool lastOperationState = HIGH;
bool sign = true;

bool isOptimistic = false;

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

  pinMode(pessPin, INPUT_PULLUP);
  pinMode(optPin, INPUT_PULLUP);

  pinMode(signPin, INPUT_PULLUP);

  pinMode(onePin, INPUT_PULLUP);
  pinMode(fivePin, INPUT_PULLUP);
  pinMode(tenPin, INPUT_PULLUP);

  // put your setup code here, to run once:

}

void loop() {
  // put your main code here, to run repeatedly:

  powerCheck();

  while (power) {
    
  powerCheck();

    bool calcState = digitalRead(calcPin);

    if ((calcState == LOW && lastCalcState == HIGH) && (mode == 0 || mode == 2 || mode == 3)) {
      mode += 1;
      delay(100);
    }

     if ((calcState == LOW && lastCalcState == HIGH) && (mode == 1)) {
      mode += 2;
      delay(100);
    }

    lastCalcState = calcState;
      

    if (mode == 0) {
      lcd.setCursor(0, 0);
      lcd.print(F("Genshin/HSR Calc"));
      lcd.setCursor(0, 1);
      lcd.print(F("Calc to start!"));

    } else if (mode == 1) {

      lcd.setCursor(0, 0);
      lcd.print(F("Feel pessimistic"));
      lcd.setCursor(0, 1);
      lcd.print(F("Or optimistic?"));

      if(digitalRead(pessPin) == LOW) {
        mode = 2;
        clearScreen();
      } else if (digitalRead(optPin) == LOW) {
        mode = 3;
        clearScreen();
        }
      } else if (mode == 2) {
        lcd.setCursor(0, 0);
        lcd.print(F("Pulls: "));
        lcd.print(pulls);
        lcd.print(F(" ~ P"));
        lcd.setCursor(0, 1);
        lcd.print(F("Calc when done!"));

        increment();

        isOptimisitc = false;

      } else if (mode == 3) {
        lcd.setCursor(0, 0);
        lcd.print(F("Pulls: "));
        lcd.print(pulls);
        lcd.print(F(" ~ O"));
        lcd.setCursor(0, 1);
        lcd.print(F("Calc when done!"));

        increment();

        isOptimisitc = true;

      } else if (mode == 4) {
        clearScreen();

        lcd.setCursor(0, 0);
        lcd.print(F("Calculating..."));
        lcd.setCursor(0, 1);
        lcd.print(F("."));
        delay(200);
        lcd.print(F("."));
        delay(200);
        lcd.print(F("."));
        delay(200);
        lcd.print(F("."));
        delay(200);
        lcd.print(F("."));
        delay(200);
        lcd.print(F("."));
        delay(200);
        lcd.print(F("."));
        delay(200);

        mode = 5;
      } else if (mode == 5) {
        clearScreen();

        lcd.setCursor(0, 0);
        lcd.print(F("Your odds are:"));
        lcd.setCursor(0, 1);

        //fix logic to make it so that it doesn't round to 100.000% unless at the valid cases :)

        lcd.print(odds * 100, 3);
        lcd.print(F("%"));

        if(isOptimisitic) {
          odds = cumulative(pulls);
        } else {
          odds = pessimisitic(pulls);
        }
      }
    }
  }


float cumulative (int pulls) {
  if (pulls == 0){
    return 0.00000;
  } else if (pulls >= 90) {
    return 1.00000;
  } else if ((pulls >= 1) && (pulls <= 73)) {
    return 1 - pow(0.996, pulls);
  } else if ((pulls >= 74) && (pulls <= 89)) {
    return table[pulls - 74];
  } else {
    return 0.40404; //Error
  }
}

float pExactX (int pulls) {
  if (pulls == 0) {
    return 0.00000;
  } else if (pulls == 90){
    return  1.0 - cumulative(89);
  } else if ((pulls >= 1) && (pulls <= 73)) {
    return 0.00600;
  } else if ((pulls >= 74) && (pulls <= 89)) {
    return ((6*(pulls - 73)) + 0.6)/100;
  } else {
    return 0.40404;
  }
}

float pessimistic(int N) {
  float total = 0;

  for (int x = 1; x <= N; x++) {
    total = total + (pExactX(x) * cumulative(N - x));
  }

  return total;
}

void powerCheck() {
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

void increment() {
  
  bool oneState = digitalRead(onePin);
  bool fiveState = digitalRead(fivePin);
  bool tenState = digitalRead(tenPin);

  bool operationState = digitalRead(signPin);

  if(operationState == LOW && lastOperationState == HIGH){
    sign = !sign;

    delay(100);
  }

  lastOperationState = operationState;

  if(oneState == LOW && lastOneState == HIGH){
    if(sign) {
      pulls++;
    } else if(!sign){
      pulls--;
    } 
    delay(75);

  } else if (fiveState == LOW && lastFiveState == HIGH){
    if(sign) {
      pulls += 5;
    } else if(!sign){
      pulls -= 5;
    } 
    delay(75);

  } else if (tenState == LOW && lastTenState == HIGH) {
    if(sign) {
      pulls += 10;
    } else if(!sign){
      pulls -= 10;
    } 
    delay(75);

  }

  if(pulls > 180) {
    pulls = 180;
  }

  if(pulls < 0) {
    pulls = 0;
  }

  lastOneState = oneState;
  lastFiveState = fiveState;
  lastTenState = tenState;
}

void clearScreen () {
  setCursor(0, 0);
  lcd.print(F("                "));
  setCursor(0, 1);
  lcd.print(F("                "));
  delay(50);
}

float capped (float percentage) {
  if(percentage >= 1.0) {
    return 0.999;
  }
}

// P (odds of eaxctly X) * C(N - X)

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

//March 30th, 2026

