int STRICTRED = 4;
int RED = 10;
int YELLOW = 11;
int GREEN = 12;
int BLUE = 13;
int WHITE = 9;

int buttonSpin = 2;
int buttonApin = 3;
int buttonBpin = 5;
int buttonCpin = 6;
int buttonDpin = 7;

byte S = digitalRead(buttonSpin);
byte A = digitalRead(buttonApin);
byte B = digitalRead(buttonBpin);
byte C = digitalRead(buttonCpin);
byte D = digitalRead(buttonDpin);

byte strict = 0;
int buzzer = 8;

int tones[] = { 523, 587, 659, 698 };
int pattern[20] = {};
int threshold = 0;
int count = 0;
int duration = 1000;

int introMelody[] = { 523, 587, 659, 698, 659, 587, 523 };
int introLights[] = {RED,YELLOW,GREEN,BLUE,WHITE,RED,YELLOW,RED};

int winMelody[] = { 523, 587, 659, 698, 784, 880, 988, 1047 };
int loseMelody[] = {  };

//play(loseLights, loseMelody, 1000, 3);
//play(winLights, winMelody, 1000, 3);

void setup() {
  Serial.begin(9600);
  randomSeed(analogRead(0));
  pinMode(buttonSpin, INPUT_PULLUP);
  pinMode(buttonApin, INPUT_PULLUP);
  pinMode(buttonBpin, INPUT_PULLUP);
  pinMode(buttonCpin, INPUT_PULLUP);
  pinMode(buttonDpin, INPUT_PULLUP);
  pinMode(RED, OUTPUT);
  pinMode(GREEN, OUTPUT);
  pinMode(BLUE, OUTPUT);
  pinMode(YELLOW, OUTPUT);
  pinMode(WHITE, OUTPUT);
  pinMode(STRICTRED, OUTPUT);
  play(introLights, introMelody, 50, 8);
}

void play(int lightArr[], int toneArr[], int ms, int limit) {
  for (int i = 0; i < limit; i++) {
    tone(buzzer, toneArr[i], ms);
    digitalWrite(lightArr[i], HIGH);
    delay(ms);
    digitalWrite(lightArr[i], LOW);
    random(0,6);
  }
}

void loop() {
  if (count == threshold) {
    increment();
    iterateExisting();
    checkExisting();
    checkDuration();
  }
  if (threshold == 20) {
    // win melody
    play(introLights, introMelody, 50, 8);
    restart();
  } else {
    checkExisting();
  }
}

void increment() {
  int rand = random(0,4);
  pattern[threshold] = rand;
  count = 0;
  Serial.print(String("Incrementing: "));
  Serial.println(rand);
  delay(duration);
  threshold++;
  for(int i = 0; i < threshold; i++) {  
    Serial.println(pattern[i]);
  }
}

void iterateExisting() {
  Serial.println(String("Iterating"));
  for (int index = 0; index < threshold; index++) {
    if (pattern[index] == 0) { //523, 587, 659, 698 
      digitalWrite(RED, HIGH);
      tone(buzzer, tones[pattern[index]], duration);
      delay(duration);
      digitalWrite(RED, LOW);
    } else if (pattern[index] == 1) {
      digitalWrite(YELLOW, HIGH);
      tone(buzzer, tones[pattern[index]], duration);
      delay(duration);
      digitalWrite(YELLOW, LOW);
    } else if (pattern[index] == 2) {
      digitalWrite(GREEN, HIGH);
      tone(buzzer, tones[pattern[index]], duration);
      delay(duration);
      digitalWrite(GREEN, LOW);
    } else if (pattern[index] == 3) {
      digitalWrite(BLUE, HIGH);
      tone(buzzer, tones[pattern[index]], duration);
      delay(duration);
      digitalWrite(BLUE, LOW);
    }
    delay(duration/10);
  }
}

void restart() {
  threshold = 0;
  count = 0;
  increment();
  iterateExisting();
}

void wrong() {
  count = 0;
  tone(buzzer, 185, 1000);
  delay(1000);
}

void checkDuration() {
  (threshold == 5) ? duration = 750 : (threshold == 10) ? duration = 500 : (threshold == 15) ? duration = 250 : duration = duration;
}

void checkExisting() {
  while (count < threshold) { // A == 1 && B == 1 && C == 1 && D == 1 && 
    digitalWrite(WHITE, HIGH);
    A = digitalRead(buttonApin);
    B = digitalRead(buttonBpin);
    C = digitalRead(buttonCpin);
    D = digitalRead(buttonDpin);
    S = digitalRead(buttonSpin);
    if (S == LOW) {
      (strict == 0) ? strict = 1 : strict = 0;
      tone(buzzer, 777, 100);
      (strict == 1) ? digitalWrite(STRICTRED, HIGH) : digitalWrite(STRICTRED, LOW);
      delay(250);
    }
    if (A == LOW) {
      if (pattern[count] == 0) {
        digitalWrite(WHITE, LOW);
        while (A == LOW) {
          A = digitalRead(buttonApin);
          digitalWrite(RED, HIGH);
          tone(buzzer, tones[0], 100);
          delay(50);
        }
        digitalWrite(RED, LOW);
        count++;
        if (count < threshold - 1) {
          return checkExisting();
          break;
        }
      } else {
        checkStrict();
      }
    } else if (B == LOW) {
      if (pattern[count] == 1) {
        digitalWrite(WHITE, LOW);
        while (B == LOW) {
          B = digitalRead(buttonBpin);
          digitalWrite(YELLOW, HIGH);
          tone(buzzer, tones[1], 100);
          delay(50);
        }
        digitalWrite(YELLOW, LOW);
        count++;
        if (count < threshold - 1) {
          return checkExisting();
          break;
        }
      } else {
        checkStrict();
      }
    } else if (C == LOW) {
      if (pattern[count] == 2) {
        digitalWrite(WHITE, LOW);
        while (C == LOW) {
          C = digitalRead(buttonCpin);
          digitalWrite(GREEN, HIGH);
          tone(buzzer, tones[2], 100);
          delay(50);
        }
        digitalWrite(GREEN, LOW);
        count++;
        if (count < threshold - 1) {
          return checkExisting();
          break;
        }
      } else {
        checkStrict();
      }
    } else if (D == LOW) {
      if (pattern[count] == 3) {
        digitalWrite(WHITE, LOW);
        while (D == LOW) {
          D = digitalRead(buttonDpin);
          digitalWrite(BLUE, HIGH);
          tone(buzzer, tones[3], 100);
          delay(50);
        }
        digitalWrite(BLUE, LOW);
        count++;
        if (count < threshold - 1) {
          return checkExisting();
          break;
        }
      } else {
        checkStrict();
      }
    }
    delay(10);
  }
}

void checkStrict() {
  wrong();
  Serial.print(String("Strict: "));
  Serial.println(strict);
  if (strict == 1) {
    //play(loseLights, loseMelody, 1000, 3);
    restart();
  } else {
    iterateExisting();
  }
}

