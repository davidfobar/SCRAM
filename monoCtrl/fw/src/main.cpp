#include <Arduino.h>
#include <BasicStepperDriver.h>

#define GRATING_STEPS_PER_REVOLUTION 400
#define GRATING_MICROSTEPS_PER_STEP 16
#define GRATING_GEAR_RATIO 180

#define GRATING_HOME_PIN 3
#define GRATING_INDEX_PIN 9

#define GRATING_SLEEP_PIN A0
#define GRATING_STEP_PIN A1
#define GRATING_DIR_PIN 15

#define GRATING_MAX_RPM 50
#define GRATING_HOMEING_RPM 50

#define SLIT_HOMEING_STEPS 800
#define SLIT_DEFAULT_OPENING_STEPS 500
#define SLIT_CLOSE_DIR 1
#define SLIT_OPEN_DIR 0

#define SLIT_SLEEP_PIN 16
#define SLIT_STEP_PIN 14
#define SLIT_DIR_PIN 11

#define SPECTROMETER_PIN 2

int currentGratingStep = 0;
bool automationMode = false;

//The grating is controlled by a stepper motor driven by a driver set to 8 microsteps per step, 
// the motor itself is a 400 step per revolution motor. The motor turns a worm gear which turns a 200 tooth gear.
BasicStepperDriver gratingStepper(400, GRATING_DIR_PIN, GRATING_STEP_PIN);

//The slit is controlled by a stepper motor driven by a driver set to 1 microsteps per step, it is a linear actuator with 200 steps per revolution, each revolution 
// moves a wedge that opens or closes the slit by 1/1000 of an inch per step
BasicStepperDriver slitStepper(200, SLIT_DIR_PIN, SLIT_STEP_PIN);

long currentGratingPosition = 0;

long degreesToSteps(float degrees) {
  return degrees * GRATING_STEPS_PER_REVOLUTION * GRATING_MICROSTEPS_PER_STEP * GRATING_GEAR_RATIO / 360;
}

long wavelengthToSteps(float wavelength) {
  return (long)(2.239e-04 * pow(wavelength, 3) -3.209e-01 * pow(wavelength, 2) + 3.449e+02 * wavelength -2.984e+04);
}

void sleepGrating() {
  digitalWrite(GRATING_SLEEP_PIN, LOW);
}

void wakeGrating() {
  digitalWrite(GRATING_SLEEP_PIN, HIGH);
}

void sleepSlit() {
  digitalWrite(SLIT_SLEEP_PIN, LOW);
}

void wakeSlit() {
  digitalWrite(SLIT_SLEEP_PIN, HIGH);
}

void printCommandList(){
  Serial.println("Waiting for command");
  Serial.println("  0: home the grating table");
  Serial.println("  1 X: move  X degrees");
  Serial.println("  2 X: switch to grating X [A, B, C]");
  Serial.println("  3: print the grating index switch status");
  Serial.println("  4 X: Home the slit");
  Serial.println("  5 X: move the slit X [10e-3 inches]");
  Serial.println("  6 X: move the grating X steps");
  Serial.println("  7 X: Move to wavelength X [nm]");
  Serial.println("  8: enter automation mode");
}

void home(){
  wakeGrating();

  //based off the current GRAITNG_HOME_PIN status, move the grating in the appropriate direction until the switch is cleared
  int initialGratingHomeSwitchStatus = digitalRead(GRATING_HOME_PIN);
  int gratingHomeDir = 0;

  // determine the initial direction to move the grating
  if (initialGratingHomeSwitchStatus == 0) {
    gratingHomeDir = -1;
  }
  else {
    gratingHomeDir = 1;
  }

  //move the grating until the switch is cleared
  gratingStepper.startMove(gratingHomeDir*degreesToSteps(90));

  //wait until the switch is cleared
  while (digitalRead(GRATING_HOME_PIN) == initialGratingHomeSwitchStatus) {
    gratingStepper.nextAction();
  }
  //continue for 5000 more steps to ensure the switch is cleared
  gratingStepper.stop();
  gratingStepper.move(gratingHomeDir*5000);

  //reduce the gratings speed to the homeing speed
  gratingStepper.setRPM(GRATING_HOMEING_RPM);

  //reverse the direction of the grating and move until the switch is back to the initial state
  gratingStepper.startMove(-gratingHomeDir*degreesToSteps(90));
  while (digitalRead(GRATING_HOME_PIN) != initialGratingHomeSwitchStatus) {
    gratingStepper.nextAction();
  }
  gratingStepper.stop();
  delay(100);

  //reset the grating speed to the max speed
  gratingStepper.setRPM(GRATING_MAX_RPM);

  currentGratingStep = 0;

  //print 'OK'
  Serial.println("OK");

}

void enterAutomationMode(){
  //expect a command of either "6 X" to move a specific number of steps, or "7 X" to move to a specific wavelength
  //once movement is complete, toggle the spectrometer pin high for 500 mS to trigger the spectrometer
  //then return to waiting for a command

  //if an '0' is provided, home the grating table
  //if an '8' is provided, exit automation mode
  while (true){
    if (Serial.available() > 0) {
      //wait in a loop for a command to be provided
      while (Serial.available() <= 0) {
        delay(100);
      }

      //read the command
      String command = Serial.readStringUntil('\n');

      //check for home command by looking at the first character
      if (command.charAt(0) == '0') {
        home();
        //home sends an OK message, so no need to send another
      }
      
      //check for grating move command by looking at the first character
      if (command.charAt(0) == '6') {
        //get the number of steps to move
        String stepsString = command.substring(2);
        long steps = stepsString.toInt();

        gratingStepper.move(steps);
        currentGratingPosition += steps;

        //delay 250 ms to allow the stepper to stop moving
        delay(250);

        //toggle Spectrometer pin high for 50 mS to trigger the spectrometer
        digitalWrite(SPECTROMETER_PIN, HIGH);
        delay(50);
        digitalWrite(SPECTROMETER_PIN, LOW);

        //delay 100 ms to allow the spectrometer to complete the measurement
        delay(100);
        
        Serial.println("OK");
      }

      //check for grating move command by looking at the first character
      if (command.charAt(0) == '7') {
        //get the number of steps to move
        String wavelengthString = command.substring(2);
        float wavelength = wavelengthString.toFloat();

        //calculate the step position of that wavelength
        long steps_pos = wavelengthToSteps(wavelength);

        //determine the steps to move from the current position
        long steps = steps_pos - currentGratingPosition;

        gratingStepper.move(steps);
        currentGratingPosition += steps;

        //delay 100 ms to allow the stepper to stop moving
        delay(100);

        //toggle Spectrometer pin high for 50 mS to trigger the spectrometer
        digitalWrite(SPECTROMETER_PIN, HIGH);
        delay(50);
        digitalWrite(SPECTROMETER_PIN, LOW);

        //delay 100 ms to allow the spectrometer to complete the measurement
        delay(100);
        
        Serial.println("OK");
      }

      //check for automation mode command by looking at the first character
      if (command.charAt(0) == '8') {

        //exit automation mode
        return;
      }
    }
  }
}

void indexGrating(){
  //to index the grating, first move the grating to the home position, then move -80 degrees, then 100 degrees, then home again
  home();

  delay(200);
  gratingStepper.move(-degreesToSteps(80));
  delay(200);
  gratingStepper.move(degreesToSteps(105));
  delay(200);
  home();
  delay(200);
}

// recieve a command from the serial port to determine which action to complete
//available commands:
// 0: home the grating table
// 1 X: move  X degrees
// 2 X: switch to grating X [A, B, C]
// 3: print the grating index switch status
int processCommand() {
  if (Serial.available() > 0) {
    //wait in a loop for a command to be provided
    while (Serial.available() <= 0) {
      delay(100);
    }

    //read the command
    String command = Serial.readStringUntil('\n');

    //check for home command by looking at the first character
    if (command.charAt(0) == '0') {
      Serial.println("Homing grating table");
      home();
      return 0;
    }

    //check for move command by looking at the first character
    if (command.charAt(0) == '1') {
      //get the number of degrees to move
      String degreesString = command.substring(2);
      float degrees = degreesString.toFloat();
      long steps = degreesToSteps(degrees);
      Serial.print("Moving grating table ");
      Serial.print(degrees);
      Serial.print(" degrees, ");
      Serial.print(steps);
      Serial.println(" steps");
      //wakeGrating();
      gratingStepper.move(steps);
      currentGratingPosition += steps;
      //sleepGrating();
      return 1;
    }

    //check for grating switch command by looking at the first character
    if (command.charAt(0) == '2') {
      //get the grating to switch to
      String gratingString = command.substring(2);

      //if they provided lower case, convert to upper case
      if (gratingString.charAt(0) >= 'a' && gratingString.charAt(0) <= 'z') {
        gratingString.setCharAt(0, gratingString.charAt(0) - 32);
      }

      //check for valid grating
      if (gratingString.charAt(0) != 'A' && gratingString.charAt(0) != 'B' && gratingString.charAt(0) != 'C') {
        Serial.println("Invalid grating");
        return -1;
      }

      Serial.print("Switching to grating ");
      Serial.println(gratingString);
      home();
      delay(200);
      gratingStepper.move(degreesToSteps(5));
      delay(200);
      //check for grating A, status is 0
      int gratingIndexSwitchStatus = digitalRead(GRATING_INDEX_PIN);
      Serial.print("Grating index switch status: ");
      Serial.println(gratingIndexSwitchStatus);

      //until we are at A, index the grating
      while(gratingIndexSwitchStatus != 0){
          Serial.println("Indexing grating");
          indexGrating();
          delay(200);
          gratingStepper.move(degreesToSteps(5));
          delay(200);
          gratingIndexSwitchStatus = digitalRead(GRATING_INDEX_PIN);
        }

      Serial.println("Grating A reached");

      // we are now at grating A, so index until we get to the desired grating
      if(gratingIndexSwitchStatus == 0){ //currenly at grating A
        if(gratingString.charAt(0) == 'A'){
          //just move back home
          gratingStepper.move(degreesToSteps(-5));
          delay(200);
        }
        if(gratingString.charAt(0) == 'B' || gratingString.charAt(0) == 'C'){
          indexGrating();
          delay(200);
        }
        if(gratingString.charAt(0) == 'C'){
          indexGrating();
          delay(200);
        }
      }

      return 2;
    }

    //check for grating index switch status command by looking at the first character
    if (command.charAt(0) == '3') {
      Serial.print("Grating index switch status: ");
      Serial.println(digitalRead(GRATING_INDEX_PIN));
      return 3;
    }

    //check for slit home command by looking at the first character
    if (command.charAt(0) == '4') {
      Serial.println("Homing slit");
      wakeSlit();
      delay(200);
      slitStepper.move(-SLIT_HOMEING_STEPS);
      delay(200);
      slitStepper.stop();
      sleepSlit();
      return 4;
    }

    //check for slit move command by looking at the first character
    if (command.charAt(0) == '5') {
      //get the number of inches to move
      String inchesString = command.substring(2);
      float inches = inchesString.toFloat();
      long steps = inches; //assume that 1 step is 1/1000 of an inch
      Serial.print("Moving slit ");
      Serial.print(inches);
      Serial.print(" inches, ");
      Serial.print(steps);
      Serial.println(" steps");

      //wake the slit
      wakeSlit();
      delay(200);

      //move the slit
      slitStepper.move(steps);
      delay(200);

      //sleep the slit
      sleepSlit();

      return 5;
    }

    //check for grating move command by looking at the first character
    if (command.charAt(0) == '6') {
      //get the number of steps to move
      String stepsString = command.substring(2);
      long steps = stepsString.toInt();

      Serial.print("Moving steps ");
      Serial.println(steps);

      gratingStepper.move(steps);
      currentGratingPosition += steps;

      Serial.println("OK");

      return 6;
    }

    //check for grating move command by looking at the first character
    if (command.charAt(0) == '7') {
      //get the number of steps to move
      String wavelengthString = command.substring(2);
      float wavelength = wavelengthString.toFloat();
      Serial.print("Moving to wavelength ");
      Serial.print(wavelength);
      Serial.println(" nm");


      //calculate the step position of that wavelength
      long steps_pos = wavelengthToSteps(wavelength);

      //determine the steps to move from the current position
      long steps = steps_pos - currentGratingPosition;

      Serial.print("Moving ");
      Serial.print(steps);
      Serial.println(" steps");

      //wakeGrating();
      gratingStepper.move(steps);
      currentGratingPosition += steps;
      Serial.print("----Current grating position: ");
      Serial.print(currentGratingPosition);
      Serial.println(" steps ----");

      //sleepGrating();

      return 7;
    }

    //check for automation mode command by looking at the first character
    if (command.charAt(0) == '8') {
      //toggle Spectrometer pin high for 500 mS to trigger the spectrometer
      enterAutomationMode();
    }

  }
  return -1;
}

void setup() {
  // initialize digital pins
  pinMode(GRATING_HOME_PIN, INPUT_PULLUP);
  pinMode(GRATING_INDEX_PIN, INPUT);

  pinMode(GRATING_SLEEP_PIN, OUTPUT);
  pinMode(GRATING_STEP_PIN, OUTPUT);
  pinMode(GRATING_DIR_PIN, OUTPUT);

  pinMode(SLIT_SLEEP_PIN, OUTPUT);
  pinMode(SLIT_STEP_PIN, OUTPUT);
  pinMode(SLIT_DIR_PIN, OUTPUT);

  pinMode(SPECTROMETER_PIN, OUTPUT);

  // initialize serial communication at 9600 bits per second:
  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }

  // initialize the stepper library:
  gratingStepper.begin(GRATING_STEPS_PER_REVOLUTION, GRATING_MICROSTEPS_PER_STEP);
  gratingStepper.setRPM(GRATING_MAX_RPM);
  gratingStepper.setSpeedProfile(gratingStepper.CONSTANT_SPEED, 2000, 2000);
  gratingStepper.enable();

  slitStepper.begin(200, 1);
  slitStepper.setRPM(200);
  slitStepper.setSpeedProfile(slitStepper.CONSTANT_SPEED, 2000, 2000);
  slitStepper.enable();

  printCommandList();
}

void loop() {
  //test to see if a command has been provided
  int command = processCommand();
  delay(100);

  //if a command was provided, reprint the command list
  if (command >= 0) {
    printCommandList();
    command = -1;
  }
}
