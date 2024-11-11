#include "SSmain.h"

#include <Keypad.h>

#include "../Menu/menu.h"
#include "../utils/utils.h"
#include "Arduino.h"
#include "U8glib.h"

int level = 1;
char squences[100] = {};

SimonSaysGame::phaseOfGame SimonSaysGame::phase = INSTRUCTIONS;

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

void SimonSaysGame::drawInstructions() {
  u8g.setFont(u8g_font_tpss);
  u8g.drawStr(0, 10, "Simon Says");
  u8g.drawStr(0, 20, "Repeat the sequence");
  u8g.drawStr(0, 30, "of lights and sounds");
  u8g.drawStr(0, 40, "Press any key to start");
}

void SimonSaysGame::drawWrongAnswer() {
  u8g.setFont(u8g_font_tpss);
  u8g.drawStr(0, 10, "Simon Says:");
  u8g.drawStr(0, 20, "U suck!");

  // draw correct answer
  u8g.drawStr(0, 30, "Correct answer:");
  for (int i = 0; i < level; i++) {
    u8g.drawStr(0 + i * 10, 40, String(squences[i]).c_str());
  }

  u8g.drawStr(0, 50, "1 Menu | 2 Again");
}

void SimonSaysGame::drawInstructionsForInput() {
  u8g.setFont(u8g_font_tpss);
  u8g.drawStr(20, 30, "Please repeat");
}

void SimonSaysGame::getInstructionsKey() {
  char key = keypad.getKey();
  if (key != NO_KEY) {
    phase = SHOWLIGHTSHOW;
  }
}

void SimonSaysGame::showCorrespondingLight(int lightID) {
  switch (lightID) {
    case 0:
      digitalWrite(greenLightPin, HIGH);
      delay(500);
      digitalWrite(greenLightPin, LOW);
      break;
    case 1:
      digitalWrite(yellowLightPin, HIGH);
      delay(500);
      digitalWrite(yellowLightPin, LOW);
      break;
    case 2:
      digitalWrite(redLightPin, HIGH);
      delay(500);
      digitalWrite(redLightPin, LOW);
      break;
    case 3:
      digitalWrite(blueLightPin, HIGH);
      delay(500);
      digitalWrite(blueLightPin, LOW);
      break;
    default:
      break;
  }
}

void SimonSaysGame::displayLightShow() {
  for (int i = 0; i < level; i++) {
    int randomLight = randInt(0, 3);
    squences[i] = randomLight;
    showCorrespondingLight(randomLight);
    delay(700);
  }
  phase = USERINPUT;
}

void SimonSaysGame::screenLightShow() {
  u8g.setFont(u8g_font_tpss);
  u8g.drawStr(20, 30, "Watch the lights!!!");
}

void SimonSaysGame::getUserInputLightIDs() {
  int keyInputCounter = 0;
  while (keyInputCounter < level && phase == USERINPUT) {
    char key = keypad.getKey();
    if (key != NO_KEY) {
      if (key >= '0' && key <= '3') {
        if (squences[keyInputCounter] == key - '0') {
          showCorrespondingLight(key - '0');
          keyInputCounter++;
        } else {
          phase = WRONGINPUT;
        }
      }
    }
  }
  if (keyInputCounter == level) {
    delay(2000);
    level++;
    phase = SHOWLIGHTSHOW;
  }
}

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

int counterShowLightShow = 0;
int counterAskForUserInput = 0;
void SimonSaysGame::mainLoopSimon() {
  u8g.firstPage();
  do {
    if (phase == INSTRUCTIONS) {
      drawInstructions();
      getInstructionsKey();
    } else if (phase == SHOWLIGHTSHOW) {
      screenLightShow();
      if (counterShowLightShow >= 350) {
        displayLightShow();
        counterShowLightShow = 0;
      }
      counterShowLightShow++;
    } else if (phase == USERINPUT) {
      drawInstructionsForInput();
      if (counterAskForUserInput >= 50) {
        getUserInputLightIDs();
        counterAskForUserInput = 0;
      }
      counterAskForUserInput++;
    } else if (phase == WRONGINPUT) {
      drawWrongAnswer();
      getWrongMenuKey();
    }
  } while (u8g.nextPage());
}