/*
SimonSays.ino (MiniGames Project)

This is the SimonSays game.
It lights leds and plays a sound.
The user has to enter those via the Keypad (repeat the sequence).
Each round the sequence gets longer by 1, starting with 1.

@author: Fabian Stiewe
@data: 10.11.2024

Used libraries:
- U8g2lib
- Keypad
Those are listed in the libaries folder, but should be installed via the Arduino
IDE.

*/

#include <Keypad.h>
#include <U8g2lib.h>

#include "Arduino.h"
#include "pitches.h"
using namespace std;

U8G2_SH1106_128X64_NONAME_F_HW_I2C u8g(U8G2_R0, /* reset=*/U8X8_PIN_NONE);

const uint8_t KEYPAD_ROWS = 4;
const uint8_t KEYPAD_COLS = 4;
char keys[KEYPAD_ROWS][KEYPAD_COLS] = {{'1', '2', '3', 'A'},
                                       {'4', '5', '6', 'B'},
                                       {'7', '8', '9', 'C'},
                                       {'*', '0', '#', 'D'}};
byte rowPins[KEYPAD_ROWS] = {5, 4, 3, 2};
byte colPins[KEYPAD_COLS] = {A3, A2, A1, A0};

Keypad keypad =
    Keypad(makeKeymap(keys), rowPins, colPins, KEYPAD_ROWS, KEYPAD_COLS);

int buzzerPin = 13;
int redLightPin = 7;
int blueLightPin = 8;
int yellowLightPin = 9;
int greenLightPin = 12;

/**
 * Setups the pinModes for each light
 */
void setupPinModes() {
  pinMode(redLightPin, OUTPUT);
  pinMode(yellowLightPin, OUTPUT);
  pinMode(greenLightPin, OUTPUT);
  pinMode(blueLightPin, OUTPUT);
}

void setup(void) {
  u8g.begin();
  randomSeed(analogRead(11));
  setupPinModes();
}

/**
 * #####################################################################
 *
 * Paste all game code below here
 *
 * #####################################################################
 */

int level = 1;
char lightSequence[15] = {};

int counterShowLightShow = 0;
int counterAskForUserInput = 0;
int counterShowCorrect = 0;

enum phaseOfGame {
  INSTRUCTIONS,
  SHOWLIGHTSHOW,
  USERINPUT,
  WRONGINPUT,
  CORRECT
};
phaseOfGame phase = INSTRUCTIONS;

/**
 * Draws a short introduction to the game.
 */
void drawInstructions() {
  u8g.drawStr(0, 10, "Simon Says");
  u8g.drawStr(0, 20, "Repeat");
  u8g.drawStr(0, 40, "Press any to start");
}

void drawCorrectAnswer() {
  u8g.drawStr(0, 30, "Correct:");
  for (int i = 0; i < level; i++) {
    u8g.drawStr(0 + i * 10, 40, String(lightSequence[i]).c_str());
  }
}

/**
 * Draws, that is was wrong and the correct answer
 */
void drawWrongAnswer() {
  u8g.drawStr(0, 10, "Simon Says:");
  u8g.drawStr(0, 20, "Bad!");
  drawCorrectAnswer();
  u8g.drawStr(0, 50, "Any to restart");
}

/**
 * Draws the instructions for the user to repeat the sequence.
 */
void drawInstructionsForInput() { u8g.drawStr(20, 30, "Repeat"); }

/**
 * Gets the key input from the user to start the game.
 */
void getInstructionsKey() {
  char key = keypad.getKey();
  if (key != NO_KEY) {
    phase = SHOWLIGHTSHOW;
  }
}

/**
 * Shows the corresponding light and plays a sound for a given light ID.
 * @param lightID The ID of the light to show.
 */
void showCorrespondingLightAndPlaySound(int lightID) {
  switch (lightID) {
    case 0:
      digitalWrite(greenLightPin, HIGH);
      tone(buzzerPin, NOTE_G4, 200);
      break;
    case 1:
      digitalWrite(yellowLightPin, HIGH);
      tone(buzzerPin, NOTE_A4, 200);
      break;
    case 2:
      digitalWrite(redLightPin, HIGH);
      tone(buzzerPin, NOTE_B4, 200);
      break;
    default:
      digitalWrite(blueLightPin, HIGH);
      tone(buzzerPin, NOTE_C5, 200);
      break;
  }
  delay(200);
  noTone(buzzerPin);
  delay(200);
  resetLights();
  delay(200);
}

/**
 * Resets all lights to LOW.
 */
void resetLights() {
  digitalWrite(redLightPin, LOW);
  digitalWrite(yellowLightPin, LOW);
  digitalWrite(greenLightPin, LOW);
  digitalWrite(blueLightPin, LOW);
}

/**
 * Displays the light show for the user to repeat.
 */
void displayLightShow() {
  for (int i = 0; i < level; i++) {
    int randomLight = random(0, 4);
    lightSequence[i] = randomLight;
    showCorrespondingLightAndPlaySound(randomLight);
  }
  phase = USERINPUT;
}

/**
 * Shows a short text on the screen to watch the lights.
 */
void screenLightShow() { u8g.drawStr(20, 30, "Watch!"); }

/**
 * Gets the user input for the light IDs.
 * Compares each input with the corresponding light in the sequence.
 */
void getUserInputLightIDs() {
  int keyInputCounter = 0;
  while (keyInputCounter < level && phase == USERINPUT) {
    char key = keypad.getKey();
    if (key != NO_KEY) {
      comparesUserinputWithSequence(key, keyInputCounter);
    }
  }
  goesToNextLevel(keyInputCounter);
}

/**
 * Compares the user input with the light sequence.
 * @param key The key input from the user.
 * @param keyInputCounter the index (which current light to compare)
 */
void comparesUserinputWithSequence(char key, int &keyInputCounter) {
  if (key >= '0' && key <= '3') {
    if (lightSequence[keyInputCounter] == key - '0') {
      showCorrespondingLightAndPlaySound(key - '0');
      keyInputCounter++;
    } else {
      phase = WRONGINPUT;
    }
  }
}

/**
 * Checks if the user input was correct and goes to the next level.
 * Otherwise sets phase to WRONGINPUT.
 * @param keyInputCounter The counter for the user input.
 */
void goesToNextLevel(int keyInputCounter) {
  if (keyInputCounter == level) {
    level++;
    phase = CORRECT;
  } else {
    phase = WRONGINPUT;
  }
}

/**
 * Gets the key input from the user to go to the menu or play again.
 */
void getWrongMenuKey() {
  char key = keypad.getKey();
  if (key != NO_KEY) {
    phase = SHOWLIGHTSHOW;
    level = 1;
  }
}

/**
 * Shows the instructions and asks for the user input.
 */
void loopPhaseInstructions() {
  drawInstructions();
  getInstructionsKey();
}

/**
 * Shows the light show and displays to watch on the screen.
 */
void loopPhaseShowLightShow() {
  screenLightShow();
  if (counterShowLightShow >= 100) {
    displayLightShow();
    counterShowLightShow = 0;
  }
  counterShowLightShow++;
}

/**
 * Shows the instructions for the user input.
 * Should repeat the sequence of lights.
 */
void loopPhaseUserinput() {
  drawInstructionsForInput();
  if (counterAskForUserInput >= 40) {
    getUserInputLightIDs();
    counterAskForUserInput = 0;
  }
  counterAskForUserInput++;
}

/**
 * Shows the input was wrong, the correct answer and aks
 * for the user to go to the menu or play again.
 */
void loopPhaseWronginput() {
  drawWrongAnswer();
  getWrongMenuKey();
}

/**
 * Shortly shows the input by the user has been correct.
 */
void loopPhaseCorrect() {
  u8g.drawStr(0, 30, "Correct!");
  if (counterShowCorrect > 35) {
    phase = SHOWLIGHTSHOW;
    counterShowCorrect = 0;
  }
  counterShowCorrect++;
}

/**
 * The main loop for the Simon Says game.
 * Displays the instructions, light show, user input, and wrong input.
 */
void mainLoopSimon() {
  u8g.firstPage();
  do {
    u8g.setFont(u8g2_font_6x13_tf);
    if (phase == INSTRUCTIONS) {
      loopPhaseInstructions();
    } else if (phase == SHOWLIGHTSHOW) {
      loopPhaseShowLightShow();
    } else if (phase == USERINPUT) {
      loopPhaseUserinput();
    } else if (phase == WRONGINPUT) {
      loopPhaseWronginput();
    } else if (phase == CORRECT) {
      loopPhaseCorrect();
    }
  } while (u8g.nextPage());
}

/**
 * #####################################################################
 *
 * Call the game loop here
 *
 * #####################################################################
 */

void loop(void) { mainLoopSimon(); }