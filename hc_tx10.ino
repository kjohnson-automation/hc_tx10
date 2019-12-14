int data = 1;
int led = 13;

int b0 = 9;
int b1 = 10;
int b2 = 11;
// the setup routine runs once when you press reset:
void setup() {
  // DEBUGGING WITH SERIAL
  Serial.begin(9600);
  // initialize the digital/LED pin as an output.
  pinMode(data, OUTPUT);
  pinMode(led, OUTPUT);

  // Configures Pins 9/10/11 as input to select mode:
  pinMode(b0, INPUT);
  pinMode(b1, INPUT);
  pinMode(b2, INPUT);
  // Configures dataline to be high
  digitalWrite(data, HIGH);
}

// Start to training sequence
void start_indicator(){
  digitalWrite(data, LOW);
  delay(1);
  digitalWrite(data, HIGH);
  delayMicroseconds(300);
}

//training sequence needed before any signal
void training() {
  start_indicator();
  for(int c =0; c<16; c++){
    digitalWrite(data, LOW);
    delayMicroseconds(293);
    digitalWrite(data, HIGH);
    delayMicroseconds(293);
  }
  delayMicroseconds(3200);
}

// Each Bit sequence starts with a LOW for 100uS
void start_bit() {
  digitalWrite(data, LOW);
  delayMicroseconds(98);
}

// Ends data sequence and sets bit to high
void end_bit() {
  digitalWrite(data, HIGH);
}

// Keeps data high for n Bits
void high_sequence(int length_bits) {
  digitalWrite(data, HIGH);
  delayMicroseconds(98*length_bits);
}

// Keeps data low for n Bits
void low_sequence(int length_bits) {
  digitalWrite(data, LOW);
  delayMicroseconds(98*length_bits);
}

/* The following are common patterns to make replication easier to code */

// toggles 3 high bits, followed by 3 low bits
void high3_low3() {
  high_sequence(3);
  low_sequence(3);
}

// toggles 3 high bits, followed by 1 low bit
void high3_low1() {
  high_sequence(3);
  low_sequence(1);
}

// toggles high than low, each 1 bit long
void high_low() {
  high_sequence(1);
  low_sequence(1);
}

void high1_low3() {
  high_sequence(1);
  low_sequence(3);
}

// Seems to be the end sequence of a message
void high3_low2() {
high_sequence(3);
low_sequence(2);
}
/* ******************************************************************* */

// Toggles lights on_off
void on_off_sequence() {
  //011100010001000101110111011100010001011101110111000100010001011100
  training();
  start_bit();
  high3_low3();
  high1_low3();
  high1_low3();
  high_low();
  high3_low1();
  high3_low1();
  high3_low3();
  high1_low3();
  high_low();
  high3_low1();
  high3_low1();
  high3_low3();
  high1_low3();
  high1_low3();
  high_low();
  high3_low2();

  end_bit();
}

// Sets lights to fade sequence
void fade_sequence() {
  training();
  start_bit();
  high3_low3();
  high1_low3();
  high1_low3();
  high1_low3();
  high_low();
  high3_low3();
  high1_low3();
  high1_low3();
  high1_low3();
  high_low();
  high3_low1();
  high3_low1();
  high3_low3();
  high_low();
  high3_low1();
  high3_low1();
  high3_low2();
  end_bit();
}

int get_input() {
  int b0_v, b1_v, b2_v;
  int pattern;
  b0_v = digitalRead(b0);
  b1_v = digitalRead(b1);
  b2_v = digitalRead(b2);
  int bit_array[3];
  bit_array[0] = b0_v;
  bit_array[1] = b1_v;
  bit_array[2] = b2_v;
  pattern = map_bits(bit_array);
  execute_input(pattern);
}

int map_bits(int bit_array[]){
//  000 - 111
int out;
if (bit_array[0] == 0 && bit_array[1] == 0 && bit_array[2] == 0) {
  out = 0;
}
else if (bit_array[0] == 0 && bit_array[1] == 0 && bit_array[2] == 1) {
  out = 1;
}
else if (bit_array[0] == 0 && bit_array[1] == 1 && bit_array[2] == 0) {
  out = 2;
}
else if (bit_array[0] == 0 && bit_array[1] == 1 && bit_array[2] == 1) {
  out = 3;
}
else if (bit_array[0] == 1 && bit_array[1] == 0 && bit_array[2] == 0) {
  out = 4;
}
else if (bit_array[0] == 1 && bit_array[1] == 0 && bit_array[2] == 1) {
  out = 5;
}
else if (bit_array[0] == 1 && bit_array[1] == 1 && bit_array[2] == 0) {
  out = 6;
}
else if (bit_array[0] == 1 && bit_array[1] == 1 && bit_array[2] == 1) {
  out = 7;
}
else {
  out = -1;
}
Serial.print("Out: ");
Serial.println(out);
return out;
}

void execute_input(int pattern_case) {
  switch (pattern_case){
    case 0:
      Serial.println("Sending On/Off Sequence");
      on_off_sequence();
      break;
    case 1:
      Serial.println("Sending Fade Sequence");
      fade_sequence();
      break;
    case 2:
      Serial.println("Invalid for now: 2");
      break;
    case 3:
      Serial.println("Invalid for now: 3");    
      break;
    case 4:
      Serial.println("Invalid for now: 4");
      break;
    case 5:
      Serial.println("Invalid for now: 5");
      break;
    case 6:
      Serial.println("Invalid for now: 6");
      break;
    case 7:
      Serial.println("Invalid for now: 7");
      break;
    default:
      Serial.println("invalid input");
      break;
  }
}
 
// the loop routine runs over and over again forever:
void loop() {
  get_input();
  delay(1000);
}
