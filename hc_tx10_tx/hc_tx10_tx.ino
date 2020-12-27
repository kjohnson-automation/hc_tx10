
//Tx Libs
#include <stdio.h>
#include <digitalWriteFast.h>
//Screen Drivers
#include <Wire.h>
#include <Adafruit_RGBLCDShield.h>
#include <utility/Adafruit_MCP23017.h>
//Tx Lib:
//#include <RadioHead.h>
#include <RH_ASK.h>
RH_ASK driver;
//SPI Lib
#include <SPI.h>

//Initializes the Screen:
// The shield uses the I2C SCL and SDA pins. On classic Arduinos
// this is Analog 4 and 5 so you can't use those for analogRead() anymore
// However, you can connect other I2C sensors to the I2C bus and share
// the I2C bus.
Adafruit_RGBLCDShield lcd = Adafruit_RGBLCDShield();

//Sets up Data for driving Tx
int data = 1;
int led = 13;
bool DEBUG = false;

//Color Defines:
#define WHITE 0x7

String ROUTINES[9] = {"On/Off", "Fade", "Twinkle", "Flash", "Dual", "Chase", "Steady", "Combo", "Auto"};
int count_index = 0;

/* Sequences for Light Control 
 *  On Off  Sequence = 100011101110111010001000100011101110100010001000111011101110100011
 *  Fade    Sequence = 100011101110111010001110111011101110100010001000111010001000100011
 *  Twinkle Sequence = 100011101110111011101000100010001110100010001000100011101110111011
 *  Flash   Sequence = 100011101110111011101000100011101110100010001000100011101110100011
 *  Dual    Sequence = 100011101110111011101000111010001110100010001000100011101000111011
 *  Chase   Sequence = 100011101110111011101000111011101110100010001000100011101000100011
 *  Steady  Sequence = 100011101110111011101110111010001110100010001000100010001000111011
 *  Combo   Sequence = 100011101110111011101110100011101110100010001000100010001110100011
 *  Auto    Sequence = 100011101110111011101110111010001110100010001000100010001000111011
 */

// the setup routine runs once when you press reset:
void setup() {
  // Setting up debug serial:
  if (DEBUG) {
    Serial.begin(115200);
  }
  // initialize the digital/LED pin as an output.
  pinMode(data, OUTPUT);
  pinMode(led, OUTPUT);
  // Configures dataline to be high
  digitalWriteFast(data, LOW);
  digitalWrite(led, LOW);

  // set up the LCD's number of columns and rows: 
  lcd.begin(16, 2);
  lcd.setBacklight(WHITE);
  lcd.print("MERRY CHRISTMAS!");
  lcd.setCursor(0,1);
  if (!driver.init()) {
    lcd.print("Tx Failed");
  }
  else {
    lcd.print("Ready to Tx");
  }
//  int (*Function_Ptr)(9);
}


//training sequence needed before any signal
//void training() {
  // training sequence = 111101010101010101010101010101010101
//  char training sequence [] = "111101010101010101010101010101010101";
//  digitalWrite(data, HIGH);
//  delayMicroseconds(980);
//  for(int c=0; c<16; c++){
//    digitalWriteFast(data, LOW);
//    delayMicroseconds(293);
//    digitalWriteFast(data, HIGH);
//    delayMicroseconds(293);
//    }
//  digitalWriteFast(data, LOW);
//  // delayMicroseconds(3200);
//}

int out_write(char write) {
  int write_var;
  if (DEBUG){
    Serial.print("Char to compare: ");
    Serial.println(write);
  }
  if (write == '0') {
    if (DEBUG){
      Serial.println("LOW");
    }
    write_var = LOW;
  }
  else if (write == '1') {
    if (DEBUG){
      Serial.println("HIGH");
    }
    write_var = HIGH;
  }
  else {
    if (DEBUG) {
      Serial.println("Bad Compare");
    }
    return -1;
  }
  digitalWriteFast(data, write_var);
  delayMicroseconds(75);
  }

// Pattern Generator - send array and writes pattern out to data
void send_pattern(char *pattern) {
  if (DEBUG) {
    Serial.print("Size of pattern: ");
    Serial.println(strlen(pattern));
    Serial.print("Pattern: ");
    Serial.println(pattern);
  }
  // Training sequence before pattern sending
  char training_sequence [] = "111101010101010101010101010101010101";
  char zeroes [] = "00000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000";
//  digitalWriteFast(led, HIGH);
//  training();
//  delayMicroseconds(3700);
  // Actual Pattern generation
  char full_sequence [] = training_sequence + zeroes + pattern;
//  for (int index=0; index < strlen(pattern); index++){
//    out_write(pattern[index]);
//    }
  driver.send((unit8_t *)full_sequence, strlen(full_sequence));
  driver.waitPacketSent();
  // Makes sure data always returns to LOW
  digitalWriteFast(data, LOW);
  digitalWriteFast(led, LOW);
  }
/* ******************************************************************* */

// Toggles lights on_off
void on_off_sequence() {
  //100011101110111010001000100011101110100010001000111011101110100011
  if (DEBUG) {
    Serial.println("Sending On Off");
  }
  char sequence[] = "100011101110111010001000100011101110100010001000111011101110100011";
  send_pattern(sequence);
}

// Sets lights to fade sequence
void fade_sequence() {
  //100011101110111010001110111011101110100010001000111010001000100011
  char sequence[] = "100011101110111010001110111011101110100010001000111010001000100011";
  send_pattern(sequence);
}

void twinkle_sequence() {
  //100011101110111011101000100010001110100010001000100011101110111011
  char sequence [] = "100011101110111011101000100010001110100010001000100011101110111011";
  send_pattern(sequence);
}

void flash_sequence() {
  char sequence [] = "100011101110111011101000100011101110100010001000100011101110100011";
  send_pattern(sequence);
}

void dual_color_sequence() {
  //100011101110111011101000100011101110100010001000100011101110100011
  char sequence [] = "100011101110111011101000111010001110100010001000100011101000111011";
  send_pattern(sequence);
}

void chasing_sequence() {
  //100011101110111011101000111011101110100010001000100011101000100011
  char sequence [] = "100011101110111011101000111011101110100010001000100011101000100011";
  send_pattern(sequence);
}

void steady_sequence() {
  //100011101110111011101110111010001110100010001000100010001000111011
  char sequence [] = "100011101110111011101110111010001110100010001000100010001000111011";
  send_pattern(sequence);
}

void combo_sequence() {
  //100011101110111011101110100011101110100010001000100010001110100011
  char sequence [] = "100011101110111011101110100011101110100010001000100010001110100011";
  send_pattern(sequence);
}

void auto_sequence() {
  //100011101110111011101110111010001110100010001000100010001000111011
  char sequence [] = "100011101110111011101110111010001110100010001000100010001000111011";
  send_pattern(sequence);
}

int up_list_num(int current_val) {
  // Moves up the scroll list
  if (DEBUG) {
    Serial.println(current_val);
  }
  if (current_val == 0) {
    if (DEBUG) {
        Serial.println(8);
    }
     return 8;
  }
  else {
    if (DEBUG) {
      Serial.println(--current_val);
    }
    return --current_val;
  }
}

int down_list_num(int current_val) {
  // Moves down the scroll list
  if (DEBUG) {
    Serial.println(current_val);
  }
  if (current_val == 8) {
      if (DEBUG) {
        Serial.println(0);
      }
    return 0;
  }
  else {
    if (DEBUG) {
      Serial.println(++current_val);
    }
    return ++current_val;
  }
}

// the loop routine runs over and over again forever:
void loop() {
  void (*Function_Ptr[9])() = {on_off_sequence, fade_sequence, twinkle_sequence, flash_sequence, dual_color_sequence, chasing_sequence, steady_sequence, combo_sequence, auto_sequence};

  uint8_t buttons = lcd.readButtons();

  if (buttons) {
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("MERRY CHRISTMAS!");
    //Sends on/off 100 times
    if (buttons & BUTTON_LEFT){
      lcd.setCursor(0,1);
      lcd.print("On/Off");
      for (int counter=0;counter<20;counter++) {
        on_off_sequence();
      }
    }
    if (buttons & BUTTON_RIGHT) {
      lcd.setCursor(0,1);
      lcd.print("Auto");
      for (int counter=0;counter<20;counter++) {
        auto_sequence();
      }
    }
    //Scroll up through the list
    if (buttons & BUTTON_UP) {
      count_index = up_list_num(count_index);
      lcd.setCursor(0,1);
      lcd.print(ROUTINES[count_index]);
    }

    //Scroll down through the list
    if (buttons & BUTTON_DOWN) {
      count_index = down_list_num(count_index);
      lcd.setCursor(0,1);
      lcd.print(ROUTINES[count_index]);
    }

    if (buttons & BUTTON_SELECT) {
      if (DEBUG) {
        Serial.println("sending: " + ROUTINES[count_index]);
      }
      lcd.setCursor(0,1);
      lcd.print("Sending " + ROUTINES[count_index]);
      for (int counter=0;counter<50;counter++) {
        Function_Ptr[count_index]();
      }
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("MERRY CHRISTMAS!");
      lcd.setCursor(0,1);
      lcd.print("HO HO HO!");
    }
  }
  //Adding button repetion
  delay(50);
}
