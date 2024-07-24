#include <Charliplexing.h>
#include <Myfont.h>
#include <TimerOne.h>
#include <Globalvar.h>
#define WIN_CONDITION 5

//Objects
class Ball {
public:
  byte x, y;
  int8_t vx, vy;

  Ball(byte xi = 0, byte yi = 0, int8_t vxi = 0, int8_t vyi = 0) : x(xi), y(yi), vx(vxi), vy(vyi) {}

  void display() {
    LedSign::Set(x, y, 7);
  }

  void update() {
    LedSign::Set(x, y, 0);
    if (y == 0 || y == 8) {
      vy *= -1;
    }
    x += vx;
    y += vy;
  }
};

class Pad {
public:
  byte x, y;

  Pad(byte player = 1) {
    x = (player == 1) ? 0 : 13;
    y = 4;
  }

  void display() {
    LedSign::Set(x, y, 7);
    LedSign::Set(x, y + 1, 7);
    LedSign::Set(x, y - 1, 7);
  }

  void off() {
    LedSign::Set(x, y, 0);
    LedSign::Set(x, y + 1, 0);
    LedSign::Set(x, y - 1, 0);
  }
};

// Initial States & defining
Ball pong;
Pad pad1(1), pad2(2);
byte score1, score2;
float d;
int bounces, sl=50;
volatile bool stopBanner = false;

void setup() {
  LedSign::Init(0);
  Serial.begin(230400);
  randomSeed(analogRead(0));
  start();
  reset();
  Timer1.initialize(1000);
  Timer1.attachInterrupt(padUpdate);
}

void padUpdate() {
  pad1.display();
  pad2.display();

  if (Serial.available() > 0) {
    byte input = Serial.read();

    if (input == 0 && pad2.y < 7) {
      pad2.off();
      pad2.y++;
    }
    else if (input == 1 && pad1.y < 7) {
      pad1.off();
      pad1.y++;
    }
    else if (input == 2 && pad1.y > 1) {
      pad1.off();
      pad1.y--;
    }
    else if (input == 3 && pad2.y > 1) {
      pad2.off();
      pad2.y--;
    }
  }
}

void start() {
  Myfont::Banner(20, "Press BLUE to start!");

  while (true) {
    if (Serial.available() > 0) {
      byte input = Serial.read();
      
      if (input == 4) {
        stopBanner = true;
      }
    }
  }
  stopBanner = false;
  score1 = 0, score2 = 0;
}

void reset() {
  int8_t vxi, vyi;

  do {
    vxi = random(-1, 2);
  } while (vxi == 0);
  
  do {
    vyi = random(-1, 2);
  } while (vyi == 0);

  pong = Ball(random(6, 8), 4, vxi, vyi);
  pad1 = Pad(1);
  pad2 = Pad(2);
  bounces = 0;
}

void checkWinner() {
  if (score1 == WIN_CONDITION || score2 == WIN_CONDITION) {
    Myfont::Banner(14, (score1 == WIN_CONDITION) ? "Player 1 wins!" : "Player 2 wins!");
    start();
  }
}

void displayScore() {
  char scores[1];
  sprintf(scores, "%d-%d", score1, score2);
  Myfont::Banner(3, scores);
}

void loop() {  
  pong.display();
  
  if (pong.x == 0 || pong.x == 13) {
    (pong.x == 0) ? score2++ : score1++;
    Timer1.stop();
    displayScore();
    checkWinner();
    reset();
    while(Serial.available() > 0) {
      Serial.read();
    }
    Timer1.resume();
    pong.display();
  }
  
  if ((pong.x == pad1.x + 1 && abs(pong.y - pad1.y) <= 2) || (pong.x == pad2.x - 1 && abs(pong.y - pad2.y) <= 2)) {
    pong.vx *= -1;
    bounces +=1;
  }

  d=pow(5.0,3.3-0.0465*bounces)+sl;
  
  delay(d); // Consider using millis() for non-blocking timing
  pong.update();
}
