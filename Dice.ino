#include <Wire.h>
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x3F, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);

int b, c, t, w;                       // b - count down,      c - cycle time,           t - time left,       w - watchdog
byte a, k, p, mn, mx, r, s;           // a - current player,  k - auxiliary variable,   p - number of players,    mn - minimum random number,   mx - maximum random number,    s - startup
int voltage[] = {1018, 994, 973, 952, 924, 904, 887, 870, 846, 830, 815, 801, 780, 766, 755, 741};
String values[] = {"1", "2", "3", "A", "4", "5", "6", "B", "7", "8", "9", "C", "*", "0", "#", "D"};
String texts[] = {"Player numbers:", "Cycle time: SEC", "Minimum number:", "Maximum number:", "Player"};

void setup() {
  Serial.begin(9600);
  pinMode(A0, INPUT);           //Resistor for random seed
  pinMode(A1, INPUT);           //Matrix input
  pinMode(A2, INPUT);           //ENGAGE BUTTON
  pinMode(1, OUTPUT);           //Buzzer
  digitalWrite(1, HIGH);        
  randomSeed(analogRead(0));    //RandomSeed
  lcd.begin(16,2);

  k = 1;                        //Number of players. Default: 1
  p = 0;
  while (k < 15) {
    if (k = 14) {               //Reset p value
      p = 1;
    } else {
      if (p <= 30)  {           //Limit p value. Max: 30
        p = 10*p + k;
      } else {
        p = 30;
      }
    }
    k = Matrix();
    Display(texts[1], p, 0);
    delay(500);
  }

  k = 1;
  c = 0;
  while (k < 15) {              //Cycle time
    if (k = 14) {               //Reset c value
      c = 1;
    } else {
      if (c <= 600) {           //Limit c value. Max: 600 sec = 10 min.
        c = 10*c + k;
      } else {
        c = 600;
      }
    }
    k = Matrix();
    Display(texts[2], c, 0);
    delay(500);
  }

  k = 1;
  mn = 0;
  while (k < 15) {              //Minimum number
    if (k = 14) {               //Reset mn value
      mn = 1;
    } else {
      if (mn <= 254) {           //Limit c value. Max: 254 (255-1).
        mn = 10*mn + k;
      } else {
        mn = 254;
      }
    }
    k = Matrix();
    Display(texts[3], mn, 0);
    delay(500);
  }

  k = 1;
  mx = 0;
  while (k < 15) {              //Maximum number
    if (k = 14) {               //Reset mn value
      mx = 1;
    } else {
      if (mx <= 255) {          //Limit c value. Max: 255.
        mx = 10*mx + k;
      } else {
        mx = 255;
      }
    }
    if (mx <= mn) {             //If max is less min, then reset value to max = min +1
    mx = mn + 1;
    }
    k = Matrix();
    Display(texts[4], mn, 0);
    delay(500);
  }                  

  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Push engage");
  lcd.setCursor(0,1);
  lcd.print("to start");
}

void loop() {
  a = 0;
  s = 0;
  w = analogRead(2);
  if (w > 500) {
    digitalWrite(1, HIGH);
    a++;
    s = 1;
    if (a > p) {
      a = 1;
    }
    b = millis();
    r = random(mn, mx);                     //Randomizing
    Display(texts[5], c, r);
  } else {                                  //Count down
    t = (((1000*c + b) - millis())/1000);
    if (s = 1) {
      if (t >= 0) {
      //String(a)
      Display(texts[5], t, r);
      } else {
      digitalWrite(1, LOW);
      Display("Time is up!", 0, 0);
      }  
    }
  }
}



void Display(String x, int y, int z) {     //Displaying  on screen
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print(x);
  lcd.setCursor(0,1);
  lcd.print(y);
  lcd.setCursor(4,1);
  lcd.print(z);
}

int Matrix() {                          //Matrix button input
  int d, i, j, tol;                     //d - data,         i - voltage variable,         j - voltage, 
  tol = 1;
  while (j < 700) {
    j = analogRead(1);
  }
  for(i = 0; i <= 15; i++) {
    if ((voltage[i]-tol) <= j) {  
    }
  }
  switch (i)  {
    case 0:
      d = 1;
      break;
    case 1:
      d = 2;
      break;
    case 2:
      d = 3;
      break;
    case 4:
      d = 4;
      break;
    case 5:
      d = 5;
      break;        
    case 6:
      d = 6;
      break;
    case 8:
      d = 7;
      break;
    case 9:
      d = 8;
      break;
    case 10:
      d = 9;
      break;
    case 11:
      d = 14;     //Clear
      break;
    case 13:
      d = 0;
      break;
    case 15:
      d = 15;     //Confirm
    /*default:
      break;*/
  }
  return d;
  //delay(200);
}



