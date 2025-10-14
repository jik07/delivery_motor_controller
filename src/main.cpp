#include <Arduino.h>

#include <commands.h>


/* Command parsing variables */

// A pair of varibles to help parse serial commands (thanks Fergs)
int arg = 0;
int index = 0;

// Variable to hold an input character
char chr;

// Variable to hold the current single-character command
char cmd;

// Character arrays to hold the first and second arguments
char argv1[ARG_MAX_LEN];
char argv2[ARG_MAX_LEN];

// The arguments converted to integers
long arg1;
long arg2;


// Run command based on currently parsed command:
void runCommand();

void resetCommand();

// Error parsing command, send back an error over serial
void errorCommand();

// Go through buffer, parse and run commands as needed
void processBuffer();

void setup() {
  
}

void loop() {
  processBuffer();
}

void runCommand() {
  switch (cmd) {
  case SET_MOTOR_PWM:
    //TODO
  case GET_ENCODER:
    //TODO
  default:
    errorCommand();
  }
}

void resetCommand() {
  arg = 0;
  index = 0;
  cmd = NULL;
  memset(argv1, 0, sizeof(argv1));
  memset(argv2, 0, sizeof(argv2));
  arg1 = 0;
  arg2 = 0;
}

void errorCommand() {
  Serial.print(ERROR_RES);
  Serial.print(NEW_COMMAND);
  resetCommand();
}

void processBuffer() {
  while (Serial.available() > 0) {
    chr = Serial.read();

    if (chr == NEW_COMMAND) {
      if (arg == 1) argv1[index] = NULL;
      else if (arg == 2) argv2[index] = NULL;
      if (cmd != NULL) runCommand();
      resetCommand();
    }

    else if (chr == NEW_ARG) {
      if (arg == 0) arg++;
      else if (arg == 1) {
        argv1[index] = NULL;
        arg++;
        index = 0;
      }
    }

    else {
      if (arg == 0) {
        if (cmd != NULL) errorCommand();
        else cmd = chr;
      } else if (index == ARG_MAX_LEN) {
        errorCommand();
      } else if (arg == 1) {
        argv1[index] = chr;
        index++;
      } else if (arg == 2) {
        argv2[index] = chr;
        index++;
      }
    }
  }
}