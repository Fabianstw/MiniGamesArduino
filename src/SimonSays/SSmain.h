#ifndef SIMONSAYSGAME_H
#define SIMONSAYSGAME_H

#include <Keypad.h>
#include <U8glib.h>

#include "../Menu/menu.h"
#include "Arduino.h"

class SimonSaysGame {
 public:
  SimonSaysGame(U8GLIB_SSD1306_128X64 &u8g, Keypad &keypad, int buzzerPin,
                int redLightPin, int yellowLightPin, int greenLightPin,
                int blueLightPin, GameMenu gameMenu);
  void mainLoopSimon();

  enum phaseOfGame {
    INSTRUCTIONS,
    SHOWLIGHTSHOW,
    USERINPUT,
    WRONGINPUT,
    CORRECT
  };

 private:
  void drawCorrectAnswer();
  void drawInstructions();
  void drawWrongAnswer();
  void getInstructionsKey();
  void getWrongMenuKey();
  void displayLightShow();
  void screenLightShow();
  void showCorrespondingLightAndPlaySound(int);
  void getUserInputLightIDs();
  void comparesUserinputWithSequence(char key, int &keyInputCounter);
  void goesToNextLevel(int keyInputCounter);
  void drawInstructionsForInput();

  void resetLights();

  void loopPhaseCorrect();
  void loopPhaseWronginput();
  void loopPhaseUserinput();
  void loopPhaseShowLightShow();
  void loopPhaseInstructions();

  static phaseOfGame phase;

  U8GLIB_SSD1306_128X64 &u8g;
  Keypad &keypad;
  int buzzerPin;
  int redLightPin;
  int yellowLightPin;
  int greenLightPin;
  int blueLightPin;
  GameMenu gameMenu;
};

#endif