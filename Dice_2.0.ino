#include <Wire.h>
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x3F, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);

int b, c, t, w;                       // b - count down,      c - cycle time,           t - time left,            w - watchdog
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
  digitalWrite(1, HIGH);        //Buzzer: OFF
  randomSeed(analogRead(0));    //RandomSeed
  lcd.begin(16,2);

  p = Setup(30, 1, 0);          //Number of players. Max: 30 players.
  c = Setup(600, 2, 0);         //Cycle time. Max: 600 sec = 10 min.
  mn = Setup(254, 3, 0);        //Minimum number. Max: 254 (255-1).
  mx = Setup(255, 4, mn);       //Maximum number. Max: 255.             

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
    digitalWrite(1, HIGH);                  //Buzzer: OFF
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
      digitalWrite(1, LOW);                 //Buzzer: ON
      Display("Time is up!", 0, 0);
      }  
    }
  }
}

int Setup(int f, byte g, byte h) {          //Setup of parameters
  k = 1;                                    //e - setup parameter (e.g.: player number, cycletime, min, max),     f - limit value,    g - displayed text,    h - mn value,
  int e = 1;
  while (k < 15) {
    if (k = 14) {                           //Reset e value to 1
      e = 1;
    } else {
      if (e <= f)  {                        //Limit e value. Max: f
        e = 10*e + k;
      } else {
        p = f;
      }
    }
    if (h > 0 && e < h) {                   //If max (e) is less min (h), then reset value to max = min + 1
      e = h + 1;
    }
  }
  k = Matrix();
  Display(texts[g], e, 0);
  delay(500);
}

void Display(String x, int y, int z) {      //Displaying  on screen
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



