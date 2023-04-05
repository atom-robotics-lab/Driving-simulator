const int JoyStick_pin = 8; //plug Joystick 'Button' into pin 8
const int X_pin = A0;       //plug joystick X direction into pin A0
const int Y_pin = A1;       //plug joystick Y direction into pin A1
int acc;
int yc;
int JSButton;

void setup() {
  for (int i = 0; i < 2; i++) {
    pinMode(JoyStick_pin, INPUT);
  }
  
  Serial.begin(115200);
}

void loop() {
 
  int x = analogRead(X_pin) ;  //read x direction value and -517 to bring back to around 0
  int y = analogRead(Y_pin) ;  //read y direction value and -512 to bring back to around 0
 /*
  if (x <-10) {         //joystick has off set of +/-8 so this negates that
    acc = 0;             //turn analogue value into integer. 0, 1 or 2 depending on state
  } else if (x >10) {   
    acc = 2;
  } else {
    acc = 1;
  }

  if (y <-10) {
    yc = 0;
  } else if (y >10) {
    yc = 2;
  } else {
    yc = 1;
  }
*/
  int buttonStates = 0;   //set starting value of Joystick button

// int x = analogRead(X_pin) - 517;  //read x direction value and -517 to bring back to around 0
//  int y = analogRead(Y_pin) - 512;  //read y direction value and -512 to bring back to around 0
int acc=map(x,0,1023,0,3);    
int yc=map(y,0,1023,0,5);    
  buttonStates |= ((digitalRead(JoyStick_pin) == LOW) ? 1 : 0) << 0;
// Serial.println(x);
// Serial.println(y);
  Serial.print("S");  //start printing the data, format is Sxc,yc,buttonStates > S1,1,0
  Serial.print(acc);
  Serial.print(",");
  Serial.print(yc);
  Serial.print(",");
//  Serial.println((buttonStates));
 Serial.println(0); 
  delay(40);

}
