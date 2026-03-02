
#include <LiquidCrystal_I2C.h>


LiquidCrystal_I2C lcd(0x27, 16, 2);

const int LDR_PIN = A0;       
const int BTN_LEFT = 8;      
const int BTN_RIGHT = 9;      
const int BTN_PAUSE = 10;     

const int LDR_THRESHOLD = 500;  

const unsigned long MOVE_INTERVAL = 400;    
const unsigned long PAUSE_DURATION = 60000; 
const unsigned long DEBOUNCE_DELAY = 50;    

bool isAutoMode = false;    
bool isPaused = false;      

int charPosition = 0;       
int charDirection = 1;      

unsigned long lastMoveTime = 0;       
unsigned long pauseStartTime = 0;     
unsigned long lastDebounceTime = 0;   

bool btnLeftPressed = false;
bool btnRightPressed = false;
bool btnPausePressed = false;

byte customChar[8] = {
  B00100,  
  B01110,  
  B11111,  
  B10101,  
  B10101,  
  B01110,  
  B00100,  
  B00000   
};


void setup() {
  Serial.begin(9600);  
  
  lcd.init();
  lcd.backlight();
  lcd.createChar(0, customChar);  
  
 
  pinMode(LDR_PIN, INPUT);
  pinMode(BTN_LEFT, INPUT);
  pinMode(BTN_RIGHT, INPUT);
  pinMode(BTN_PAUSE, INPUT);
  
  lcd.clear();
  lcd.print("System Ready");
  delay(1000);
  lcd.clear();
  
  updateCharacterOnLCD();
  
  Serial.println("=== SYSTEM STARTED ===");
}


void loop() {
  int ldrValue = analogRead(LDR_PIN);
  
  if (ldrValue >= LDR_THRESHOLD) {
    isAutoMode = true;   
  } else {
    isAutoMode = false;  
    isPaused = false;    
  }
  
  handleButtons();
  
  if (isAutoMode) {
    handleAutomaticMode(ldrValue);  
  } else {
    handleManualMode();
  }
  
  updateDisplay(ldrValue);
  
  delay(10);
}


void handleButtons() {
  unsigned long currentTime = millis();
  
  if (currentTime - lastDebounceTime > DEBOUNCE_DELAY) {
    
    if (digitalRead(BTN_LEFT) == HIGH) {
      btnLeftPressed = true;
      lastDebounceTime = currentTime;
    } else {
      btnLeftPressed = false;
    }
    
    if (digitalRead(BTN_RIGHT) == HIGH) {
      btnRightPressed = true;
      lastDebounceTime = currentTime;
    } else {
      btnRightPressed = false;
    }
    
    if (digitalRead(BTN_PAUSE) == HIGH) {
      btnPausePressed = true;
      lastDebounceTime = currentTime;
    } else {
      btnPausePressed = false;
    }
  }
}


void handleManualMode() {
  if (btnLeftPressed) {
    if (charPosition > 0) {
      charPosition--;
      updateCharacterOnLCD();
      delay(200);  
    }
  }
  
  if (btnRightPressed) {
    if (charPosition < 15) {
      charPosition++;
      updateCharacterOnLCD();
      delay(200);  
    }
  }
}


void handleAutomaticMode(int ldrValue) {
  
  
  if (btnPausePressed && !isPaused) {
    isPaused = true;
    pauseStartTime = millis(); 
    Serial.print(">>> PAUSED | t=");
    Serial.print(millis());
    Serial.print("ms | LDR=");
    Serial.println(ldrValue);
    delay(200);  
  }
  
  if (isPaused) {
    unsigned long elapsed = millis() - pauseStartTime;
    
    if (elapsed > 0 && elapsed % 10000 < 20) {
      Serial.print("Pause: ");
      Serial.print(elapsed / 1000);
      Serial.print("LDR=");
      Serial.println(ldrValue);
    }
    
    if (elapsed >= PAUSE_DURATION) {
      
      if (ldrValue >= LDR_THRESHOLD) {
        isPaused = false;
        charDirection = 1; 
        Serial.print(">>> RESUMED | t=");
        Serial.print(millis());
        Serial.print("ms | LDR=");
        Serial.println(ldrValue);
      } else {
        Serial.print("Waiting for light | LDR=");
        Serial.println(ldrValue);
      }
    }
  }
  
 
  if (!isPaused && ldrValue >= LDR_THRESHOLD) {
    if (millis() - lastMoveTime >= MOVE_INTERVAL) {
      
      charPosition += charDirection;
      
      if (charPosition >= 15) {
        charDirection = -1;
        charPosition = 15;  
      } else if (charPosition <= 0) {
        charDirection = 1;
        charPosition = 0;   
      }
      
      updateCharacterOnLCD();
      
      lastMoveTime = millis();
    }
  }
}

void updateCharacterOnLCD() {
  lcd.setCursor(0, 0);  
  
  for (int i = 0; i < 16; i++) {
    if (i == charPosition) {
      lcd.write(byte(0));  
    } else {
      lcd.print(" ");       
    }
  }
}

void updateDisplay(int ldrValue) {
  lcd.setCursor(0, 1);
  
  if (isAutoMode) {
    if (isPaused) {
      lcd.print("Mode:Auto P");  
    } else {
      lcd.print("Mode:Auto   ");  
    }
  } else {
    lcd.print("Mode:Manual ");    
  }
  
  
  lcd.setCursor(11, 1);            
  lcd.print("L:");                 
  lcd.print(ldrValue);             
  lcd.print("   ");                
  
  
}