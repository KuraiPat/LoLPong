#define up 8
#define initialize 9
#define down A1
#define joysticky A2

void setup() {
  Serial.begin(9600);
  pinMode(down,0);
  pinMode(up,0);
  pinMode(initialize,0);
  pinMode(joysticky,0);
}
// big data
byte data=0b000;
// set initial "before" bits
bool ib = 0;
bool ub = 0;
bool db = 0;
// set initial "counter" bits (0 because the first press will be read instantaniously)
byte ic = 0;
byte uc = 0;
byte dc = 0;
// set initial "result" bits
bool ir=0;
bool ur=0;
bool dr=0;

bool u, d;
void loop() {
  int dy = analogRead(joysticky);
  if (dy<512-50) {
    u=0;
  }
  else {
    u=1;
  }
  if (dy>512+50) {
    d=0;
  }
  else {
    d=1;
  }
  bool i = digitalRead(initialize);// read input initialize button
  if (i==ib && ic > 0 && i==0){// if timer is not up
    ir=1;
    ic--;
  }
  else if (i==ib && ic==0 && i==0){// if timer up
    ir=0;
    ic=1000;
  }
  else {// do nothing and reset counter
    ir=1;
    ic=0;
  }
  if (digitalRead(up)==0){
    u = digitalRead(up);
  }
  if (u==ub && uc > 0 && u==0){// if timer is not up
    ur=1;
    uc--;
  }
  else if (u==ub && uc==0 && u==0){// if timer is up
    ur=0;
    uc=10;
  }
  else {// do nothing and reset counter
    ur=1;
    uc=0;
  }
  if (digitalRead(down)==0){
    d = digitalRead(down);
  }
  if (d==db && dc > 0 && d==0){// if timer is not up
    dr=1;
    dc--;  
  }
  else if (d==db && dc==0 && d==0) {// if timer is up
    dr=0;
    dc=10;
  }
  else {// do nothing and reset counter
    dr=1;
    dc=0;
  }
  data = ir*4+ur*2+dr; //translate data into bits (i like suffering so i do the maths)
  data ^= 7; // invert bits (why the hell do we need to do this why controller?! why?!?!?!?!?!?!)
  if (data !=0){
    Serial.write(data); //(testing)
  }
  delay(10);// refresh rate
  // set the before values to the current value
  ib = i;
  ub = u;
  db = d;
}