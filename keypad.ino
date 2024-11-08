#include <LiquidCrystal_I2C.h>
#include <Keypad.h>
#include <Servo.h>

LiquidCrystal_I2C lcd(0x27, 20, 2);
Servo myservo;

#define Password_Length 7 // Space for six chars + NULL char

#define PIEZO   8
#define NOTE_G4  392
#define NOTE_C5  523
#define NOTE_G5  784
#define NOTE_C6  1047

int pos = 0;    // Variable to store the servo position

char Data[Password_Length]; // Holds the input password
char Master[Password_Length] = "123456"; // Master password

byte data_count = 0;
int door = 1; // 1 = closed, 0 = open
unsigned long accessGrantedTime = 0; // Time when access was granted
const unsigned long accessDuration = 60000; // 1 minute in milliseconds

const byte ROWS = 4;
const byte COLS = 4;

char keys[ROWS][COLS] = {
  {'1', '2', '3', 'A'},
  {'4', '5', '6', 'B'},
  {'7', '8', '9', 'C'},
  {'*', '0', '#', 'D'}
};

byte rowPins[ROWS] = {10, 9, 8, 7};
byte colPins[COLS] = {6, 5, 4, 3};

Keypad myKeypad = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);

int OpenMelody[] = {NOTE_G5, NOTE_C6};
int OpenNoteDurations[] = {12, 8};
int CloseMelody[] = {NOTE_C6, NOTE_G5};
int CloseNoteDurations[] = {12, 8};

#define playOpenMelody() playMelody(OpenMelody, OpenNoteDurations, 2)
#define playCloseMelody() playMelody(CloseMelody, CloseNoteDurations, 2)

void setup() {
  myservo.attach(11);
  pinMode(PIEZO, OUTPUT);
  ServoClose(); // Ensure the servo starts in the closed position
  lcd.init();
  lcd.backlight();
  lcd.print("   Travis Peach  ");
  lcd.setCursor(0, 1);
  lcd.print("    Safe Box   ");
  delay(3000);
  lcd.clear();

  Serial.begin(9600); // Initialize serial for debugging
}

void loop() {
  if (door == 0) {
    // Door is open, check for auto-close
    checkForAutoClose();
  } else {
    // Door is closed, wait for password input
    Open();
  }
}

void clearData() {
  for (byte i = 0; i < Password_Length; i++) {
    Data[i] = 0;
  }
  data_count = 0;
}

void ServoOpen() {
  for (pos = 90; pos >= 0; pos -= 5) {
    myservo.write(pos);
    delay(15);
  }
}

void ServoClose() {
  for (pos = 0; pos <= 90; pos += 5) {
    myservo.write(pos);
    delay(15);
  }
}

void Open() {
  lcd.setCursor(0, 0);
  lcd.print(" Enter Password ");
  
  char customKey = myKeypad.getKey();
  if (customKey) {
    if (customKey == 'D') {
      // Backspace functionality
      if (data_count > 0) {
        data_count--;
        Data[data_count] = '\0'; // Remove last character
        // Move cursor back and overwrite with space
        lcd.setCursor(data_count, 1);
        lcd.print(' ');
        lcd.setCursor(data_count, 1);
      }
    } else {
      // Regular key input
      if (data_count < Password_Length - 1) {
        Data[data_count] = customKey;
        lcd.setCursor(data_count, 1);
        lcd.print('*'); // Mask input for security
        data_count++;
      }
    }
  }

  if (data_count == Password_Length - 1) {
    Data[data_count] = '\0'; // Null-terminate the string
    if (!strcmp(Data, Master)) {
      lcd.clear();
      ServoOpen();
      lcd.print(" Access Granted");
      playOpenMelody();
      door = 0; // Set door to open state
      accessGrantedTime = millis(); // Record the time access was granted
    } else {
      lcd.clear();
      lcd.print(" Access Denied ");
      playCloseMelody();
      delay(1000);
      door = 1; // Keep door closed
    }
    clearData(); // Clear the input data after checking
  }
}

void playMelody(int *melody, int *noteDurations, int notesLength) {
  for (int thisNote = 0; thisNote < notesLength; thisNote++) {
    int noteDuration = 1000 / noteDurations[thisNote];
    tone(PIEZO, melody[thisNote], noteDuration);
    int pauseBetweenNotes = noteDuration * 1.30;
    delay(pauseBetweenNotes);
    noTone(PIEZO);
  }
}

// Function to check for auto-close after 1 minute
void checkForAutoClose() {
  if (millis() - accessGrantedTime >= accessDuration) {
    lcd.clear();
    lcd.print(" Closing...");
    ServoClose();
    playCloseMelody();
    delay(2000);
    door = 1; // Set door back to closed state
    lcd.clear();
  }
}
