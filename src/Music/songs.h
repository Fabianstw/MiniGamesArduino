#include "pitches.h"

int starWarsMelody[] = {
    NOTE_G4,  4, NOTE_G4,  4, NOTE_G4,  4, NOTE_DS4, 8, NOTE_AS4, 8, NOTE_G4, 4,
    NOTE_DS4, 8, NOTE_AS4, 8, NOTE_G4,  2, NOTE_D5,  4, NOTE_D5,  4, NOTE_D5, 4,
    NOTE_DS5, 8, NOTE_AS4, 8, NOTE_FS4, 4, NOTE_DS4, 8, NOTE_AS4, 8};

int marioMelody[] = {NOTE_E5,  8, NOTE_E5, 8, NOTE_E5, 4, NOTE_C5, 8,
                     NOTE_E5,  4, NOTE_G5, 2, NOTE_G4, 4, NOTE_C5, 8,
                     NOTE_G4,  8, NOTE_E4, 8, NOTE_A4, 8, NOTE_B4, 8,
                     NOTE_AS4, 8, NOTE_A4, 8, NOTE_G4, 4};

int terisLongMelody[] = {
    NOTE_E5, 4, NOTE_B4, 8, NOTE_C5, 8, NOTE_D5, 4, NOTE_C5, 8, NOTE_B4, 8,
    NOTE_A4, 4, NOTE_A4, 8, NOTE_C5, 8, NOTE_E5, 4, NOTE_D5, 8, NOTE_C5, 8,
    NOTE_B4, 4, NOTE_C5, 8, NOTE_D5, 4, NOTE_E5, 4, NOTE_C5, 4, NOTE_A4, 4,
    NOTE_A4, 8, NOTE_A4, 4, NOTE_B4, 8, NOTE_C5, 8,

    NOTE_D5, 4, NOTE_F5, 8, NOTE_A5, 4, NOTE_G5, 8, NOTE_F5, 8, NOTE_E5, 4,
    NOTE_C5, 8, NOTE_E5, 4, NOTE_D5, 8, NOTE_C5, 8, NOTE_B4, 4, NOTE_B4, 8,
    NOTE_C5, 8, NOTE_D5, 4, NOTE_E5, 4, NOTE_C5, 4, NOTE_A4, 4, NOTE_A4, 4,
    REST,    4,

    NOTE_E5, 4, NOTE_B4, 8, NOTE_C5, 8, NOTE_D5, 4, NOTE_C5, 8, NOTE_B4, 8,
    NOTE_A4, 4, NOTE_A4, 8, NOTE_C5, 8, NOTE_E5, 4, NOTE_D5, 8, NOTE_C5, 8,
    NOTE_B4, 4, NOTE_C5, 8, NOTE_D5, 4, NOTE_E5, 4, NOTE_C5, 4, NOTE_A4, 4,
    NOTE_A4, 8, NOTE_A4, 4, NOTE_B4, 8, NOTE_C5, 8};

int entchenMelody[] = {
    NOTE_C4, 4, NOTE_D4, 4, NOTE_E4, 4, NOTE_F4, 4, NOTE_G4, 2, NOTE_G4, 2,
    NOTE_A4, 4, NOTE_A4, 4, NOTE_G4, 4, NOTE_G4, 4, NOTE_F4, 4, NOTE_F4, 4,
    NOTE_E4, 4, NOTE_E4, 4, NOTE_D4, 4, NOTE_D4, 4, NOTE_C4, 2, NOTE_C4, 2};

// collection of melodies, stored with their length
int *melodies[] = {starWarsMelody, marioMelody, terisLongMelody, entchenMelody};
int melodyLengths[] = {
    sizeof(starWarsMelody) / sizeof(int), sizeof(marioMelody) / sizeof(int),
    sizeof(terisLongMelody) / sizeof(int), sizeof(entchenMelody) / sizeof(int)};
