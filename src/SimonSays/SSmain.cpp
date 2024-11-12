#include "SSmain.h"

#include <Keypad.h>

#include "../Menu/menu.h"
#include "../utils/utils.h"
#include "Arduino.h"
#include "U8glib.h"

int level = 1;
char lightSequence[100] = {};

int counterShowLightShow = 0;
int counterAskForUserInput = 0;
int counterShowCorrect = 0;

SimonSaysGame::phaseOfGame SimonSaysGame::phase = INSTRUCTIONS;

/**
 * Constructor for the SimonSaysGame class.
 * @param u8g The U8GLIB object for the OLED display.
 * @param keypad The Keypad object for the input.
 * @param buzzerPin The pin number for the buzzer.
 * @param redLightPin The pin number for the red light.
 * @param yellowLightPin The pin number for the yellow light.
 * @param greenLightPin The pin number for the green light.
 * @param blueLightPin The pin number for the blue light.
 * @param gameMenu The GameMenu object for the game menu,
 *                  to reset the current gamechoice.
 */
SimonSaysGame::SimonSaysGame(U8GLIB_SSD1306_128X64 &u8g, Keypad &keypad,
                             int buzzerPin, int redLightPin, int yellowLightPin,
                             int greenLightPin, int blueLightPin,
                             GameMenu gameMenu)
    : u8g(u8g),
      keypad(keypad),
      buzzerPin(buzzerPin),
      redLightPin(redLightPin),
      yellowLightPin(yellowLightPin),
      greenLightPin(greenLightPin),
      blueLightPin(blueLightPin),
      gameMenu(gameMenu) {}

/**
 * Draws a short introduction to the game.
 */
void SimonSaysGame::drawInstructions() {
  u8g.setFont(u8g_font_tpss);
  u8g.drawStr(0, 10, "Simon Says");
  u8g.drawStr(0, 20, "Repeat the sequence");
  u8g.drawStr(0, 30, "of lights and sounds");
  u8g.drawStr(0, 40, "Press any key to start");
}

void SimonSaysGame::drawCorrectAnswer() {
  u8g.drawStr(0, 30, "Correct answer:");
  for (int i = 0; i < level; i++) {
    u8g.drawStr(0 + i * 10, 40, String(lightSequence[i]).c_str());
  }
}

/**
 * Draws, that is was wrong and the correct answer
 */
void SimonSaysGame::drawWrongAnswer() {
  u8g.setFont(u8g_font_tpss);
  u8g.drawStr(0, 10, "Simon Says:");
  u8g.drawStr(0, 20, "that was not good!");

  drawCorrectAnswer();

  u8g.drawStr(0, 50, "1 Menu | 2 Again");
}

/**
 * Draws the instructions for the user to repeat the sequence.
 */
void SimonSaysGame::drawInstructionsForInput() {
  u8g.setFont(u8g_font_tpss);
  u8g.drawStr(20, 30, "Please repeat");
}

/**
 * Gets the key input from the user to start the game.
 */
void SimonSaysGame::getInstructionsKey() {
  char key = keypad.getKey();
  if (key != NO_KEY) {
    phase = SHOWLIGHTSHOW;
  }
}

/**
 * Shows the corresponding light for a given light ID.
 * @param lightID The ID of the light to show.
 */
void SimonSaysGame::showCorrespondingLight(int lightID) {
  switch (lightID) {
    case 0:
      lightLEDForTime(greenLightPin, 500);
      break;
    case 1:
      lightLEDForTime(yellowLightPin, 500);
      break;
    case 2:
      lightLEDForTime(redLightPin, 500);
      break;
    default:
      lightLEDForTime(blueLightPin, 500);
      break;
  }
}

/**
 * Displays the light show for the user to repeat.
 */
void SimonSaysGame::displayLightShow() {
  for (int i = 0; i < level; i++) {
    int randomLight = randInt(0, 3);
    lightSequence[i] = randomLight;
    showCorrespondingLight(randomLight);
    delay(700);
  }
  phase = USERINPUT;
}

/**
 * Shows a short text on the screen to watch the lights.
 */
void SimonSaysGame::screenLightShow() {
  u8g.setFont(u8g_font_tpss);
  u8g.drawStr(20, 30, "Watch the lights!!!");
}

/**
 * Gets the user input for the light IDs.
 * Compares each input with the corresponding light in the sequence.
 */
void SimonSaysGame::getUserInputLightIDs() {
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
void SimonSaysGame::comparesUserinputWithSequence(char key,
                                                  int &keyInputCounter) {
  if (key >= '0' && key <= '3') {
    if (lightSequence[keyInputCounter] == key - '0') {
      showCorrespondingLight(key - '0');
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
void SimonSaysGame::goesToNextLevel(int keyInputCounter) {
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
void SimonSaysGame::getWrongMenuKey() {
  char key = keypad.getKey();
  if (key != NO_KEY) {
    if (key == '1') {
      phase = INSTRUCTIONS;
      level = 1;
      gameMenu.setGameChoice(gameMenu.MENUGAMECHOICE);
    } else if (key == '2') {
      phase = SHOWLIGHTSHOW;
      level = 1;
    }
  }
}

/**
 * Shows the instructions and asks for the user input.
 */
void SimonSaysGame::loopPhaseInstructions() {
  drawInstructions();
  getInstructionsKey();
}

/**
 * Shows the light show and displays to watch on the screen.
 */
void SimonSaysGame::loopPhaseShowLightShow() {
  screenLightShow();
  if (counterShowLightShow >= 350) {
    displayLightShow();
    counterShowLightShow = 0;
  }
  counterShowLightShow++;
}

/**
 * Shows the instructions for the user input.
 * Should repeat the sequence of lights.
 */
void SimonSaysGame::loopPhaseUserinput() {
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
void SimonSaysGame::loopPhaseWronginput() {
  drawWrongAnswer();
  getWrongMenuKey();
}

/**
 * Shortly shows the input by the user has been correct.
 */
void SimonSaysGame::loopPhaseCorrect() {
  u8g.drawStr(0, 30, "Correct!");
  if (counterShowCorrect > 350) {
    phase = SHOWLIGHTSHOW;
    counterShowCorrect = 0;
  }
  counterShowCorrect++;
}

/**
 * The main loop for the Simon Says game.
 * Displays the instructions, light show, user input, and wrong input.
 */
void SimonSaysGame::mainLoopSimon() {
  u8g.firstPage();
  do {
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