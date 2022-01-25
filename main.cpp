#include <simplecpp>
#include <string>
#include <vector>
#include <stdio.h>
#include <stdlib.h>
using namespace simplecpp;

//movingObject.h
#ifndef _MOVINGOBJECT_INCLUDED_
#define _MOVINGOBJECT_INCLUDED_

#include <simplecpp>
#include <vector>
#include <composite.h>
#include <sprite.h>

using namespace simplecpp;

class MovingObject : public Sprite {
  vector<Sprite*> parts;
  double vx, vy;
  double ax, ay;
  bool paused;
  void initMO(double argvx, double argvy, double argax, double argay, bool argpaused=true) {
    vx=argvx; vy=argvy; ax=argax; ay=argay; paused=argpaused;
  }
 public:
 MovingObject(double argvx, double argvy, double argax, double argay, bool argpaused=true)
    : Sprite() {
    initMO(argvx, argvy, argax, argay, argpaused);
  }
 MovingObject(double speed, double angle_deg, double argax, double argay, bool argpaused, bool rtheta) : Sprite() {
   double angle_rad = angle_deg*PI/180.0;
   double argvx = speed*cos(angle_rad);
   double argvy = -speed*sin(angle_rad);
   initMO(argvx, argvy, argax, argay, argpaused);
  }
  void set_vx(double argvx) { vx = argvx; }
  void set_vy(double argvy) { vy = argvy; }
  void set_ax(double argax) { ax = argax; }
  void set_ay(double argay) { ay = argay; }
  double getXPos();
  double getYPos();
  void reset_all(double argx, double argy, double speed, double angle_deg, double argax, double argay, bool argpaused, bool rtheta);

  void pause() { paused = true; }
  void unpause() { paused = false; }
  bool isPaused() { return paused; }

  void addPart(Sprite* p) {
    parts.push_back(p);
  }
  void nextStep(double t);
  void getAttachedTo(MovingObject *m);
};

#endif

//MovingObject.cpp

void MovingObject::nextStep(double t) {
  if(paused) { return; }
  //cerr << "x=" << getXPos() << ",y=" << getYPos() << endl;
  //cerr << "vx=" << vx << ",vy=" << vy << endl;
  //cerr << "ax=" << ax << ",ay=" << ay << endl;

  for(size_t i=0; i<parts.size(); i++){
    parts[i]->move(vx*t, vy*t);
  }
  vx += ax*t;
  vy += ay*t;
} // End MovingObject::nextStep()

double MovingObject::getXPos() {
  return (parts.size() > 0) ? parts[0]->getX() : -1;
}

double MovingObject::getYPos() {
  return (parts.size() > 0) ? parts[0]->getY() : -1;
}

void MovingObject::reset_all(double argx, double argy, double speed, double angle_deg, double argax, double argay, bool argpaused, bool rtheta) {
  for(size_t i=0; i<parts.size(); i++){
    parts[i]->moveTo(argx, argy);
  }
  double angle_rad = angle_deg*PI/180.0;
  double argvx = speed*cos(angle_rad);
  double argvy = -speed*sin(angle_rad);
  vx = argvx; vy = argvy; ax = argax; ay = argay; paused = argpaused;
} // End MovingObject::reset_all()

void MovingObject::getAttachedTo(MovingObject *m) {
  double xpos = m->getXPos();
  double ypos = m->getYPos();
  for(size_t i=0; i<parts.size(); i++){
    parts[i]->moveTo(xpos, ypos);
  }
  initMO(m->vx, m->vy, m->ax, m->ay, m->paused);
}

//coin.h
#ifndef __COIN_H__
#define __COIN_H__



class Coin : public MovingObject {
  double coin_start_x;
  double coin_start_y;
  double release_speed;
  double release_angle_deg;
  double coin_ax;
  double coin_ay;

  // Moving parts
  Circle coin_circle;

 public:
 Coin(double speed, double angle_deg, double argax, double argay, bool argpaused, bool rtheta) : MovingObject(speed, angle_deg, argax, argay, argpaused, rtheta) {
    release_speed = speed;
    release_angle_deg = angle_deg;
    coin_ax = argax;
    coin_ay = argay;
    initCoin();
  }
  int level;
  int level2;
  int level3;
  void initCoin();
  void resetCoin();
  void resetCoin2();
  void resetCoin3();

};

class Bomb : public MovingObject {
  double bomb_start_x;
  double bomb_start_y;
  double release_speed;
  double release_angle_deg;
  double bomb_ax;
  double bomb_ay;


  // Moving parts
  Circle bomb_circle;

 public:
 Bomb(double speed, double angle_deg, double argax, double argay, bool argpaused, bool rtheta) : MovingObject(speed, angle_deg, argax, argay, argpaused, rtheta) {
    release_speed = speed;
    release_angle_deg = angle_deg;
    bomb_ax = argax;
    bomb_ay = argay;
    initBomb();
  }
  int level;
  void initBomb();
  void resetBomb();
  void resetBomb2();

};  // End class Coin

#endif

//lasso.h
#ifndef __LASSO_H__
#define __LASSO_H__

//#define WINDOW_X 1200
//#define WINDOW_Y 960
#define WINDOW_X 800
#define WINDOW_Y 600

#define STEP_TIME 0.05

#define PLAY_X_START 100
#define PLAY_Y_START 0
#define PLAY_X_WIDTH (WINDOW_X-PLAY_X_START)
#define PLAY_Y_HEIGHT (WINDOW_Y-100)

#define LASSO_X_OFFSET 100
#define LASSO_Y_HEIGHT 100
#define LASSO_BAND_LENGTH LASSO_X_OFFSET
#define LASSO_THICKNESS 5

#define COIN_GAP 1

#define RELEASE_ANGLE_STEP_DEG 5
#define MIN_RELEASE_ANGLE_DEG 0
#define MAX_RELEASE_ANGLE_DEG (360-RELEASE_ANGLE_STEP_DEG)
#define INIT_RELEASE_ANGLE_DEG 45

#define RELEASE_SPEED_STEP 20
#define MIN_RELEASE_SPEED 0
#define MAX_RELEASE_SPEED 200
#define INIT_RELEASE_SPEED 100

#define COIN_SPEED 120
#define COIN_ANGLE_DEG 90

#define LASSO_G 30
#define COIN_G 30

#define LASSO_SIZE 10
#define LASSO_RADIUS 50
#define COIN_SIZE 5

class Lasso : public MovingObject {
  double lasso_start_x;
  double lasso_start_y;
  double release_speed;
  double release_angle_deg;
  double lasso_ax;
  double lasso_ay;
  bool gotcoin1 = false;
  bool gotcoin2 = false;
  bool gotcoin3 = false;
  bool gotbomb = false;
  bool gotbomb2 = false;

  // Moving parts
  Circle lasso_circle;
  Circle lasso_loop;

  // Non-moving parts
  Line lasso_line;
  Line lasso_band;

  // State info
  bool lasso_looped;
  Coin *the_coin;
  Coin *the_coin2;
  Coin *the_coin3;
  Bomb *the_bomb;
  int num_coins;
  int lives;

  void initLasso();
 public:
 Lasso(double speed, double angle_deg, double argax, double argay, bool argpaused, bool rtheta) : MovingObject(speed, angle_deg, argax, argay, argpaused, rtheta) {
    release_speed = speed;
    release_angle_deg = angle_deg;
    lasso_ax = argax;
    lasso_ay = argay;
    initLasso();
  }

  int level;
  void draw_lasso_band();
  void yank();
  void loopit();
  void addAngle(double angle_deg);
  void addSpeed(double speed);

  void nextStep(double t);
  void check_for_coin(Coin *coin);
  void check_for_coin2(Coin *coin2);
  void check_for_coin3(Coin *coin3);
  void check_for_bomb(Bomb *bomb);
  int getNumCoins() { return num_coins; }
  int getLives(){ return lives; }
  int getLevel(){ return level; }
  void updateLevel(int l);

}; // End class Lasso

#endif

//coin.h

void Coin::initCoin() {
  coin_start_x = (PLAY_X_START+WINDOW_X)/2;
  coin_start_y = PLAY_Y_HEIGHT;
  coin_circle.reset(coin_start_x, coin_start_y, COIN_SIZE);
  coin_circle.setColor(COLOR("green"));
  coin_circle.setFill(true);
  addPart(&coin_circle);
  level=1;
  level2=1;
  level3=1;
}

void Bomb::initBomb() {
  bomb_start_x = (PLAY_X_START+WINDOW_X)/2+50;
  bomb_start_y = PLAY_Y_HEIGHT;
  bomb_circle.reset(bomb_start_x, bomb_start_y, COIN_SIZE);
  bomb_circle.setColor(COLOR("black"));
  bomb_circle.setFill(true);
  addPart(&bomb_circle);
  level=1;
}

void Coin::resetCoin() {
  double coin_speed = COIN_SPEED;
  double coin_angle_deg = 90;
  coin_ax = 0;
  coin_ay = COIN_G;
  bool paused = true, rtheta = true;
  if(level==1){
    reset_all(coin_start_x, coin_start_y, coin_speed, coin_angle_deg, coin_ax, coin_ay, paused, rtheta);
  }
  else if(level==2){
    reset_all(coin_start_x, coin_start_y, 1.2*coin_speed, coin_angle_deg, coin_ax, coin_ay, paused, rtheta);
  }
  if(level==3){
    reset_all(coin_start_x, coin_start_y, 1.4*coin_speed, coin_angle_deg, coin_ax, coin_ay, paused, rtheta);
  }
}

void Coin::resetCoin2() {
  double coin_speed = COIN_SPEED;
  double coin_angle_deg = 100;
  coin_ax = 0;
  coin_ay = COIN_G;
  bool paused = true, rtheta = true;
  if(level2==1){
    reset_all(coin_start_x, coin_start_y, coin_speed, coin_angle_deg, coin_ax, coin_ay, paused, rtheta);
  }
  else if(level2==2){
    reset_all(coin_start_x, coin_start_y, 1.2*coin_speed, coin_angle_deg, coin_ax, coin_ay, paused, rtheta);
  }
  if(level2==3){
    reset_all(coin_start_x, coin_start_y, 1.4*coin_speed, coin_angle_deg, coin_ax, coin_ay, paused, rtheta);
  }
}

void Coin::resetCoin3() {
  double coin_speed = COIN_SPEED;
  double coin_angle_deg = 80;
  coin_ax = 0;
  coin_ay = COIN_G;
  bool paused = true, rtheta = true;
  if(level3==1){
    reset_all(coin_start_x, coin_start_y, coin_speed, coin_angle_deg, coin_ax, coin_ay, paused, rtheta);
  }
  else if(level3==2){
    reset_all(coin_start_x, coin_start_y, 1.2*coin_speed, coin_angle_deg, coin_ax, coin_ay, paused, rtheta);
  }
  if(level3==3){
    reset_all(coin_start_x, coin_start_y, 1.4*coin_speed, coin_angle_deg, coin_ax, coin_ay, paused, rtheta);
  }
}

void Bomb::resetBomb() {
  double bomb_speed = COIN_SPEED;
  double bomb_angle_deg = 100;
  bomb_ax = 0;
  bomb_ay = COIN_G;
  bool paused = true, rtheta = true;
  if(level==1){
    reset_all(bomb_start_x, bomb_start_y, bomb_speed, bomb_angle_deg, bomb_ax, bomb_ay, paused, rtheta);
  }
  else if(level==2){
    reset_all(bomb_start_x, bomb_start_y, 1.2*bomb_speed, bomb_angle_deg, bomb_ax, bomb_ay, paused, rtheta);
  }
  if(level==3){
    reset_all(bomb_start_x, bomb_start_y, 1.4*bomb_speed, bomb_angle_deg, bomb_ax, bomb_ay, paused, rtheta);
  }
}

//lasso.cpp


void Lasso::draw_lasso_band() {
  double len = (release_speed/MAX_RELEASE_SPEED)*LASSO_BAND_LENGTH;
  double arad = release_angle_deg*PI/180.0;
  double xlen = len*cos(arad);
  double ylen = len*sin(arad);
  lasso_band.reset(lasso_start_x, lasso_start_y, (lasso_start_x-xlen), (lasso_start_y+ylen));
  lasso_band.setThickness(LASSO_THICKNESS);
} // End Lasso::draw_lasso_band()

void Lasso::initLasso() {
  lasso_start_x = (PLAY_X_START+LASSO_X_OFFSET);
  lasso_start_y = (PLAY_Y_HEIGHT-LASSO_Y_HEIGHT);
  lasso_circle.reset(lasso_start_x, lasso_start_y, LASSO_SIZE);
  lasso_circle.setColor(COLOR("blue"));
  lasso_circle.setFill(true);
  lasso_loop.reset(lasso_start_x, lasso_start_y, LASSO_SIZE/2);
  lasso_loop.setColor(COLOR("green"));
  lasso_loop.setFill(true);
  addPart(&lasso_circle);
  addPart(&lasso_loop);
  lasso_looped = false;
  the_coin = NULL;
  the_coin2 = NULL;
  the_coin3 = NULL;
  the_bomb=NULL;
  num_coins = 0;
  lives=3;
  level=1;

  lasso_line.reset(lasso_start_x, lasso_start_y, lasso_start_x, lasso_start_y);
  lasso_line.setColor(COLOR("green"));

  lasso_band.setColor(COLOR("blue"));
  draw_lasso_band();

} // End Lasso::initLasso()

void Lasso::yank() {
  bool paused = true, rtheta = true;
  reset_all(lasso_start_x, lasso_start_y, release_speed, release_angle_deg, lasso_ax, lasso_ay, paused, rtheta);
  lasso_loop.reset(lasso_start_x, lasso_start_y, LASSO_SIZE/2);
  lasso_loop.setFill(true);
  lasso_looped = false;

  if(the_coin != NULL && gotcoin1) {
    num_coins++;
    gotcoin1=false;
    the_coin->resetCoin();
  }
  if(the_coin2 != NULL && gotcoin2) {
    num_coins++;
    gotcoin2=false;
    the_coin2->resetCoin2();
  }
  if(the_coin3 != NULL && gotcoin3) {
    num_coins++;
    gotcoin3=false;
    the_coin3->resetCoin3();
  }
  if(the_bomb != NULL && gotbomb) {
    num_coins--;
    lives--;
    gotbomb=false;
    the_bomb->resetBomb();
  }

  level=num_coins/10+1;
  if(level>3){
    level=3;
  }
  if(the_coin!=NULL){
    the_coin->level=level;
  }
  if(the_coin2!=NULL){
    the_coin2->level2=level;
  }
  if(the_coin3!=NULL){
    the_coin3->level3=level;
  }
  if(the_bomb!=NULL){
    the_bomb->level=level;
  }
}// End Lasso::yank()

void Lasso::loopit() {
  if(lasso_looped) { return; } // Already looped
  lasso_loop.reset(getXPos(), getYPos(), LASSO_RADIUS);
  lasso_loop.setFill(false);
  lasso_looped = true;
} // End Lasso::loopit()

void Lasso::addAngle(double angle_deg) {
  release_angle_deg += angle_deg;
  if(release_angle_deg < MIN_RELEASE_ANGLE_DEG) { release_angle_deg = MIN_RELEASE_ANGLE_DEG; }
  if(release_angle_deg > MAX_RELEASE_ANGLE_DEG) { release_angle_deg = MAX_RELEASE_ANGLE_DEG; }
  bool paused = true, rtheta = true;
  reset_all(lasso_start_x, lasso_start_y, release_speed, release_angle_deg, lasso_ax, lasso_ay, paused, rtheta);
} // End Lasso::addAngle()

void Lasso::addSpeed(double speed) {
  release_speed += speed;
  if(release_speed < MIN_RELEASE_SPEED) { release_speed = MIN_RELEASE_SPEED; }
  if(release_speed > MAX_RELEASE_SPEED) { release_speed = MAX_RELEASE_SPEED; }
  bool paused = true, rtheta = true;
  reset_all(lasso_start_x, lasso_start_y, release_speed, release_angle_deg, lasso_ax, lasso_ay, paused, rtheta);
} // End Lasso::addSpeed()

void Lasso::nextStep(double stepTime) {
  draw_lasso_band();
  MovingObject::nextStep(stepTime);
  if(getYPos() > PLAY_Y_HEIGHT) { yank(); }
  lasso_line.reset(lasso_start_x, lasso_start_y, getXPos(), getYPos());
} // End Lasso::nextStep()

void Lasso::check_for_coin(Coin *coinPtr) {
  double lasso_x = getXPos();
  double lasso_y = getYPos();
  double coin_x = coinPtr->getXPos();
  double coin_y = coinPtr->getYPos();
  double xdiff = (lasso_x - coin_x);
  double ydiff = (lasso_y - coin_y);
  double distance = sqrt((xdiff*xdiff)+(ydiff*ydiff));
  if(distance <= LASSO_RADIUS) {
    the_coin = coinPtr;
    gotcoin1=true;
    the_coin->getAttachedTo(this);
  }
}

void Lasso::check_for_coin2(Coin *coinPtr2) {
  double lasso_x = getXPos();
  double lasso_y = getYPos();
  double coin_x = coinPtr2->getXPos();
  double coin_y = coinPtr2->getYPos();
  double xdiff = (lasso_x - coin_x);
  double ydiff = (lasso_y - coin_y);
  double distance = sqrt((xdiff*xdiff)+(ydiff*ydiff));
  if(distance <= LASSO_RADIUS) {
    the_coin2 = coinPtr2;
    gotcoin2=true;
    the_coin2->getAttachedTo(this);
  }
}

void Lasso::check_for_coin3(Coin *coinPtr3) {
  double lasso_x = getXPos();
  double lasso_y = getYPos();
  double coin_x = coinPtr3->getXPos();
  double coin_y = coinPtr3->getYPos();
  double xdiff = (lasso_x - coin_x);
  double ydiff = (lasso_y - coin_y);
  double distance = sqrt((xdiff*xdiff)+(ydiff*ydiff));
  if(distance <= LASSO_RADIUS) {
    the_coin3 = coinPtr3;
    gotcoin3=true;
    the_coin3->getAttachedTo(this);
  }
}

void Lasso::check_for_bomb(Bomb *bombPtr) {
  double lasso_x = getXPos();
  double lasso_y = getYPos();
  double bomb_x = bombPtr->getXPos();
  double bomb_y = bombPtr->getYPos();
  double xdiff = (lasso_x - bomb_x);
  double ydiff = (lasso_y - bomb_y);
  double distance = sqrt((xdiff*xdiff)+(ydiff*ydiff));
  if(distance <= LASSO_RADIUS) {
    the_bomb = bombPtr;
    gotbomb=true;
    the_bomb->getAttachedTo(this);
  }
}// End Lasso::check_for_coin()


main_program {
  cout<<"Welcome to LASSO GAME\n";
  string username;
  cout<<"\nPlease set a username :";
  cin>>username;
  cout<<"Hello "<<username<<", please press 'r' to see the controls : ";


  char c;  //rules to play the game
  while (true){
    cin>>c;
    if(c=='r'){
      cout<<"\n";
      cout<<"Press t to throw lasso \n";
      cout<<"Press l to loop lasso \n";
      cout<<"Press l again to collect coins or bomb \n";
      cout<<"Press y to yank lasso \n";
      cout<<"Press ] to increase the angle of throw  of lasso with the horizontal \n";
      cout<<"Press [ to decrease the angle of throw  of lasso with the horizontal \n";
      cout<<"Press = to increase the speed of lasso throw \n";
      cout<<"Press - to decrease the speed of lasso throw \n";
      cout<<"Press q to quit the game \n";
      break;
    }

    else cout<<"Wrong command, press r to see the controls .\n";
  }

  cout<<"Target : you have to score as much as possible by collecting coins only \n";
  cout<<"         Every coin will increse your score by one \n";
  cout<<"         Do not catch the bombs which are black in color it will cost you a score and a life \n";
  cout<<"         Initially you have only 3 lives \n";
  cout<<"         GAME OVER if you lose all the 3 lives \n";

  cout<<"\n"<<username<<" , hope you will enjoy the game :).\n";

  char s;
  cout<<"Press s to start the game : ";

  while(true){
  cin>>s;
  if(s=='s'){
  initCanvas("Lasso", WINDOW_X, WINDOW_Y);
  int stepCount = 0;
  float stepTime = STEP_TIME;
  float runTime = -1; // sec; -ve means infinite
  float currTime = 0;

  // Draw lasso at start position
  double release_speed = INIT_RELEASE_SPEED; // m/s
  double release_angle_deg = INIT_RELEASE_ANGLE_DEG; // degrees
  double lasso_ax = 0;
  double lasso_ay = LASSO_G;
  bool paused = true;
  bool rtheta = true;
  Lasso lasso(release_speed, release_angle_deg, lasso_ax, lasso_ay, paused, rtheta);

  Line b1(0, PLAY_Y_HEIGHT, WINDOW_X, PLAY_Y_HEIGHT);
  b1.setColor(COLOR("blue"));
  Line b2(PLAY_X_START, 0, PLAY_X_START, WINDOW_Y);
  b2.setColor(COLOR("blue"));

  string msg("Cmd: _");
  Text charPressed(PLAY_X_START+50, PLAY_Y_HEIGHT+20, msg);
  char coinScoreStr[256];
  char livesStr[256];
  char levelStr[256];
  sprintf(coinScoreStr, "Coins: %d", lasso.getNumCoins());
  sprintf(livesStr, "Lives: %d", lasso.getLives());
  sprintf(levelStr, "LEVEL : %d", lasso.getLevel());
  Text coinScore(PLAY_X_START+50, PLAY_Y_HEIGHT+50, coinScoreStr);
  Text livesRem(PLAY_X_START+50, PLAY_Y_HEIGHT+70, livesStr);
  Text levelNow(WINDOW_X/2, 20, levelStr);

  paused = true; rtheta = true;
  double coin_speed = COIN_SPEED;
  double coin_angle_deg = COIN_ANGLE_DEG;
  double coin_ax = 0;
  double coin_ay = COIN_G;
  Coin coin(coin_speed, coin_angle_deg, coin_ax, coin_ay, paused, rtheta);
  Coin coin2(coin_speed, coin_angle_deg+10, coin_ax, coin_ay, paused, rtheta);
  Coin coin3(coin_speed, coin_angle_deg-10, coin_ax, coin_ay, paused, rtheta);
  Bomb bomb(coin_speed, coin_angle_deg+10, coin_ax, coin_ay, paused, rtheta);




  // After every COIN_GAP sec, make the coin jump
  double last_coin_jump_end = 0;

  // When t is pressed, throw lasso
  // If lasso within range, make coin stick
  // When y is pressed, yank lasso
  // When l is pressed, loop lasso
  // When q is pressed, quit

  for(;;) {
    if((runTime > 0) && (currTime > runTime)) {
      break;
    }
    if(lasso.getLives()==0){
      break;
    }
    XEvent e;
    bool pendingEv = checkEvent(e);
    if(pendingEv) {
      char c = charFromEvent(e);
      msg[msg.length()-1] = c;
      charPressed.setMessage(msg);
      switch(c) {
      case 't':
	lasso.unpause();
	break;
      case 'y':
	lasso.yank();
	break;
      case 'l':
	lasso.loopit();
	lasso.check_for_coin(&coin);
	lasso.check_for_coin2(&coin2);
	lasso.check_for_coin3(&coin3);
	lasso.check_for_bomb(&bomb);
	wait(STEP_TIME*5);
	break;
      case '[':
	if(lasso.isPaused()) { lasso.addAngle(-RELEASE_ANGLE_STEP_DEG);	}
	break;
      case ']':
	if(lasso.isPaused()) { lasso.addAngle(+RELEASE_ANGLE_STEP_DEG); }
	break;
      case '-':
	if(lasso.isPaused()) { lasso.addSpeed(-RELEASE_SPEED_STEP); }
	break;
      case '=':
	if(lasso.isPaused()) { lasso.addSpeed(+RELEASE_SPEED_STEP); }
	break;
      case 'q':
	exit(0);
      default:
	break;
      }
    }

    lasso.nextStep(stepTime);

    coin.nextStep(stepTime);
    if(coin.isPaused()) {
      if((currTime-last_coin_jump_end) >= COIN_GAP) {
	coin.unpause();
      }
    }

    if(coin.getYPos() > PLAY_Y_HEIGHT) {
      coin.resetCoin();
      last_coin_jump_end = currTime;
    }

    coin2.nextStep(stepTime);
    if(coin2.isPaused()) {
      if((currTime-last_coin_jump_end) >= COIN_GAP) {
	coin2.unpause();
      }
    }
    if(coin2.getYPos() > PLAY_Y_HEIGHT) {
      coin2.resetCoin2();
      last_coin_jump_end = currTime;
    }

    coin3.nextStep(stepTime);
    if(coin3.isPaused()) {
      if((currTime-last_coin_jump_end) >= COIN_GAP) {
	coin3.unpause();
      }
    }

    if(coin3.getYPos() > PLAY_Y_HEIGHT) {
      coin3.resetCoin3();
      last_coin_jump_end = currTime;
    }

    bomb.nextStep(stepTime);
    if(bomb.isPaused()) {
      if((currTime-last_coin_jump_end) >= COIN_GAP) {
	bomb.unpause();
      }
    }

    if(bomb.getYPos() > PLAY_Y_HEIGHT) {
      bomb.resetBomb();
      last_coin_jump_end = currTime;
    }

    sprintf(coinScoreStr, "Coins: %d", lasso.getNumCoins());
    coinScore.setMessage(coinScoreStr);

    sprintf(livesStr, "Lives: %d", lasso.getLives());
    livesRem.setMessage(livesStr);

    sprintf(levelStr, "LEVEL : %d", lasso.getLevel());
    levelNow.setMessage(levelStr);

    stepCount++;
    currTime += stepTime;
    wait(stepTime);
  } // End for(;;)

  wait(3);
  break;
  }

  else cout<<"Wrong command , press 's' to start the game : ";
  }
} // End main_program
