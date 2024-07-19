#define up 8
#define down A1
void setup() {
  Serial.begin(9600);
  pinMode(down,0);
  pinMode(up,0);
}
// big data
byte data=0b00;
// set initial "before" bits
bool ub = 0;
bool db = 0;
// set initial "counter" bits (0 because the first press will be read instantaniously)
byte uc = 0;
byte dc = 0;
// set initial "result" bits
bool ur=0;
bool dr=0;

void loop() {
  bool u = digitalRead(up);// read input up button
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
  bool d = digitalRead(down);// read input down button
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
  data = (ur*2+dr); //translate data into bits (i like suffering so i do the maths) and times 8 to shift the bits 3 spaces
  data ^= 3;   //inversio
  if (data !=0){
    if (data==1){
      data=0;
    }
    if (data==2){
      data=3;
    }
    Serial.print(data); //(testing)
  }
  delay(10);// refresh rate
  // set the before values to the current value
  ub = u;
  db = d;
}