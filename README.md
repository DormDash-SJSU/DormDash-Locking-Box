# DormDash Safe Box

## Description

This Arduino-based project is a secure digital lock system for a safe box. It integrates the following components:
- **4x4 Membrane Keypad** for password input.
- **Two Servo Motors** to control the locking and unlocking mechanism.
- **Passive Buzzer** to play melodies for access granted/denied events.
- **I2C LCD Screen** to display prompts and status messages.

The system includes features such as password input with a backspace option, auto-close functionality after a set time, and visual/audio feedback for user actions.

---

## Features

- **Secure Access**: Enter a predefined 6-digit password to unlock.
- **Visual Feedback**: LCD displays password prompts, success, and failure messages.
- **Audio Feedback**: A passive buzzer plays melodies for granted or denied access.
- **Auto-Close**: The safe automatically locks after 1 minute of being open.
- **Dual Lock Mechanism**: Two servos operate simultaneously for enhanced security.
- **Backspace Support**: Use the `D` key to erase the last entered character.

---

## Components

### Hardware
1. **Arduino Uno**
2. **4x4 Membrane Keypad**
3. **I2C 20x2 LCD Display**
4. **Two Servo Motors**
5. **Passive Buzzer**
6. **Power Supply** 
7. **Wires and Breadboard**

---

## Pin Connections

### Keypad
- **Rows**: Pins 3, 4, 5, 6
- **Columns**: Pins 7, 8, 9, 10

### LCD
- **SDA (Data)**: Pin A4
- **SCL (Clock)**: Pin A5

### Servos
- **Servo 1 (Lock 1)**: Pin 11
- **Servo 2 (Lock 2)**: Pin 12

### Passive Buzzer
- **Signal**: Pin 13
- **Ground**: GND

---

## How It Works

1. **Startup**:
   - The LCD displays a welcome message for 3 seconds.
   - The servos are initialized to the locked (closed) position.

2. **Password Input**:
   - Enter the 6-digit password using the keypad.
   - Use the `D` key to backspace and correct input.

3. **Access Granted**:
   - If the password matches, the servos unlock, and the buzzer plays an "open" melody.
   - The LCD displays "Access Granted."

4. **Access Denied**:
   - If the password is incorrect, the buzzer plays a "close" melody, and the LCD displays "Access Denied."

5. **Auto-Close**:
   - The safe locks automatically 1 minute after being unlocked.

---

## Code Overview

### Libraries Used
1. **LiquidCrystal_I2C**: For controlling the LCD display.
2. **Keypad**: For handling keypad input.
3. **Servo**: For controlling the servo motors.

### Key Functions
- `setup()`: Initializes all components and sets the safe to the locked state.
- `loop()`: Monitors the system for password input and triggers appropriate actions.
- `ServoOpen()`: Unlocks the servos.
- `ServoClose()`: Locks the servos.
- `playMelody()`: Plays a melody using the passive buzzer.
- `checkForAutoClose()`: Locks the safe automatically after 1 minute of inactivity.

---

## Usage Instructions

1. **Power the System**:
   - Connect the Arduino Uno to a power source.
   - Ensure the servos and buzzer are properly wired.

2. **Unlock the Safe**:
   - Enter the 6-digit password on the keypad (default: `123456`).
   - If entered correctly, the servos unlock, and the LCD displays "Access Granted."

3. **Lock the Safe**:
   - The safe locks automatically after 1 minute.

---

## Customization

1. **Change Password**:
   - Modify the `Master` variable in the code:
     ```cpp
     char Master[Password_Length] = "YOUR_PASSWORD";
     ```

2. **Auto-Close Duration**:
   - Adjust the `accessDuration` variable (in milliseconds):
     ```cpp
     const unsigned long accessDuration = 60000; // 1 minute
     ```

3. **Melodies**:
   - Customize the tones in `OpenMelody` and `CloseMelody` arrays.

---

## Future Improvements

- Add a numeric display for remaining auto-close time.
- Implement multi-user password support.
- Add an external button for manual locking.

---

## License

This project is free to use and modify. Attribution is appreciated.

