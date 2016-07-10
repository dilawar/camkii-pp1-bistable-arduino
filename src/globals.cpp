#include "globals.h"
#include <avr/wdt.h>

bool reboot_ = false;
bool paused_ = false;
bool flipped_ = false;

// This must be default to 0. It is set to 1 with some probablity (most likely
// to be 0.5).
bool CS_plus = 0;
int trialNum = 0;
int prevTwoTrials[2] = { 0, 0 };
int nextProbeIn = 0;
unsigned int CS_fraction = 1;

String status = "000";

unsigned int CS_TONE_1 = 4500;
unsigned int CS_TONE_2 = 11000;

long int trialTime = 0;
char status_[5] = "PRE_";

