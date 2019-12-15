#include <stdio.h>
int data = 1;
int led = 13;
bool DEBUG = false;

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

  if (DEBUG){
    Serial.begin(9600);
  }
  // initialize the digital/LED pin as an output.
  pinMode(data, OUTPUT);
  pinMode(led, OUTPUT);
  // Configures dataline to be high
  digitalWrite(data, LOW);
}


//training sequence needed before any signal
void training() {
  // training sequence = :
  digitalWrite(data, HIGH);
  delayMicroseconds(980);
  for(int c =0; c<16; c++){
    digitalWrite(data, LOW);
    delayMicroseconds(293);
    digitalWrite(data, HIGH);
    delayMicroseconds(293);
    }
  digitalWrite(data, LOW);
  // delayMicroseconds(3200);
}

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
  digitalWrite(data, write_var);
  delayMicroseconds(85);
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
  training();
  delayMicroseconds(3700);

  // Actual Pattern generation
  for (int index=0; index < strlen(pattern); index++){
    out_write(pattern[index]);
    }
  // Makes sure data always returns to LOW
  digitalWrite(data, LOW);
  }
/* ******************************************************************* */

// Toggles lights on_off
void on_off_sequence() {
  // NEW
  //100011101110111010001000100011101110100010001000111011101110100011
  char sequence[] = "100011101110111010001000100011101110100010001000111011101110100011";
  send_pattern(sequence);
}

// Sets lights to fade sequence
void fade_sequence() {

}
 
// the loop routine runs over and over again forever:
void loop() {
  if (DEBUG) {
    Serial.println("Looping");
  }
  for (int counter=0;counter<10;counter++){
    on_off_sequence();
    delayMicroseconds(15000);
  }
  delay(1000);
  
}
