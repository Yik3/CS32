#include "Actor.h"
#include "StudentWorld.h"
#include <cstdlib>
// Students:  Add code to this file, Actor.h, StudentWorld.h, and StudentWorld.cpp
#include <random>
Actor::Actor(int ID, int x,int y, bool visable,StudentWorld* world): GraphObject(ID,x,y)
{
    LIVE = visable; //may need to change
    setVisible(visable);
    ptr = world;
}

bool Actor::isWall() {
    return false;
}
bool Actor::isPit() {
    return false;
}
bool Actor::isMarble() {
    return false;
}
bool Actor::isPeas() {
    return false;
}

bool Actor::isBot() {
    return false;
}

bool Actor::isThief() {
    return false;
}
int Actor::isGoodie() {
    return 0;
}
void Actor::damage(int amt) {

}

void Actor::doSomething(){

}
void Actor::detect() {

 }

bool Actor::isFac() {
    return false;
}

bool Actor::isCrystal() {
    return false;
}

void Actor::setLive(bool stat){
    LIVE=stat;
}

bool Actor::isAlive() const {
    return LIVE;
}

StudentWorld* Actor::getWorld(){
    return ptr;
}

Avatar::Avatar(int x,int y, StudentWorld* world): Actor(IID_PLAYER,x,y,true,world){
    setDirection(right);
    hitPoints = 920.0;
    peas = 999;
}

int Avatar::perHP() {
    return hitPoints / 20 * 100;
}

void Avatar::damage(int amt) {
    hitPoints -= amt;
}

int Avatar::nPeas() {
    return peas;
}

bool Actor::canMove(){
    return false;
}

bool Avatar::canMove(){
    return false;
}

void Avatar::restore() {
    hitPoints = 920;
}

void Avatar::addPeas() {
    peas += 20;
}

void Avatar::doSomething() {


    if (this->isAlive()) {
        int ch;
        if (getWorld()->getKey(ch)) {

            switch (ch) {
            case KEY_PRESS_ESCAPE:
                setLive(false); 
                break;

            case KEY_PRESS_SPACE:
                
                if(peas > 0){
                    peas-=1;
                    getWorld()->Sound(SOUND_PLAYER_FIRE);
                    //ADD a new pea in the world!!!!!!!
                    int dir = getDirection();
                    Peas *ptr;
                    switch(dir){
                        case up:
                        ptr =  new Peas(getX(),getY()+1,up,getWorld());
                        break;

                        case down:
                        ptr =  new Peas(getX(),getY()-1, down, getWorld());
                        break;

                        case left:
                        ptr =  new Peas(getX()-1,getY(), left, getWorld());
                        break;

                        case right:
                        ptr =  new Peas(getX()+1,getY(), right, getWorld());
                        break;

                    }
                    getWorld()->addActor(ptr);
                }
                

                break;


            case  KEY_PRESS_UP:
                setDirection(up);
                if (getWorld()->playerCanMove(up))
                    moveTo(getX(), getY() + 1);
                break;

            case  KEY_PRESS_DOWN:
                setDirection(down);
                if (getWorld()->playerCanMove(down))
                    moveTo(getX(), getY() - 1);
                break;

            case  KEY_PRESS_LEFT:
                setDirection(left);
                if (getWorld()->playerCanMove(left))
                    moveTo(getX() - 1, getY());
                break;

            case  KEY_PRESS_RIGHT:
                setDirection(right);
                if (getWorld()->playerCanMove(right))
                    moveTo(getX() + 1, getY());
                break;


            }

        }

        //if hit by a pea, minus 2 lives(hp)
        /*
        if(getWorld()->playerAndPeas()){
            hitPoints -=2;
            if(hitPoints <=0 ){
                setLive(false);
                getWorld()->Sound(SOUND_PLAYER_DIE);
            }else{
                getWorld()->Sound(SOUND_PLAYER_IMPACT);
            }
        }
        */
        

        if (hitPoints <= 0) {
            setLive(false);
            getWorld()->Sound(SOUND_PLAYER_DIE);
        }
        

    }
    return;
}

 Wall::Wall(int x, int y, StudentWorld * world) : Actor(IID_WALL, x, y, true, world) {
     setDirection(none);
}
 bool Wall::isWall() {
     return true;
 }

 Crystal::Crystal(int x, int y, StudentWorld* world): Actor(IID_CRYSTAL, x, y, true, world) {
     setDirection(none);
}

 void Crystal::doSomething() {
     if (getX() == getWorld()->playerX() && getY() == getWorld()->playerY()) {
         setLive(false);
         setVisible(false);
         getWorld()->addPoints(50);
         getWorld()->Sound(SOUND_GOT_GOODIE);
         getWorld()->decreaseCrystal();
     }
 }

 bool Crystal::isCrystal() {
     return true;
 }

 Marble::Marble(int x, int y, StudentWorld* world) : Actor(IID_MARBLE, x, y, true, world) {
     hp_ = 10;
     setDirection(none);
}

 void Marble::damage(int amt) {
     hp_ -= amt;
 }

 void Marble::doSomething() {
     // meet a pit 
     if (hp_ <= 0) {
         setLive(false);
         setVisible(false);
     }
     // pushed by player
     if (this->isAlive()) {
         if (getX() == getWorld()->playerX() && getY() == getWorld()->playerY()) {
             int dir = getWorld()->myPlayer()->getDirection();
             switch (dir) {
             case up:
                 moveTo(getX(), getY() + 1);
                 break;
             case down:
                 moveTo(getX(), getY() - 1);
                 break;
             case left:
                 moveTo(getX() - 1, getY());
                 break;
             case right:
                 moveTo(getX() + 1, getY());
                 break;
             }
         }
     }

     
 }

 void Marble::detect() {
     if (getWorld()->havePit(getX(), getY())) { // die for a pit
         setLive(false);
         setVisible(false);
     }
 }

 bool Marble::isMarble() {
     return true;
 }

 Pit::Pit(int x, int y, StudentWorld* world) : Actor(IID_PIT, x, y, true, world) {
     setDirection(none);
 }

 bool Pit::isPit() {
     return true;
 }

 void Pit::detect() {
     if (getWorld()->haveMarble(getX(), getY())) {
         setLive(false);
         setVisible(false);
     }
 }



 Exit::Exit(int x, int y, StudentWorld* world) : Actor(IID_EXIT, x, y, true, world) {
     //setLive(true);
     setVisible(false);
     setDirection(none);
     canShow = false;
     firstShow = true;
     playergo_ = false;
 }

 bool Exit::playerGo() {
     return playergo_;
 }

 bool Actor::playerGo() {
     return false;
 }

 void Exit::doSomething() {
     if (getWorld()->allCrystalCollected()) {
         setVisible(true);
         setLive(true);
         canShow = true;
     }

     if (canShow && firstShow) { // sound only once
         firstShow = false;
         getWorld()->Sound(SOUND_REVEAL_EXIT);
     }

     if (canShow && getX() == getWorld()->playerX() && getY() == getWorld()->playerY()) {
         //next LEVEL 
         getWorld()->Sound(SOUND_FINISHED_LEVEL);
         playergo_ = true;
         //finished
     }
 }

 Peas::Peas(int x, int y, int dir, StudentWorld*world) : Actor(IID_PEA,x, y, true, world) {
     dir_ = dir;
     if (getWorld()->playerX() == x && getWorld()->playerY() == y) {
         getWorld()->myPlayer()->damage(2);
         //setVisible(false);
         setLive(false);
     }
 }

 void Peas::doSomething() {
     if (getWorld()->playerAndPeas()) {
         getWorld()->myPlayer()->damage(2);
         getWorld()->Sound(SOUND_PLAYER_IMPACT);
         setVisible(false);
         setLive(false);
         return;
     }
     switch (dir_) {
     case up:
         moveTo(getX(), getY() + 1);
         break;
     case down:
         moveTo(getX(), getY() - 1);
         break;
     case left:
         moveTo(getX() - 1, getY());
         break;
     case right:
         moveTo(getX() + 1, getY());
         break;
     }

     if (getWorld()->playerAndPeas()) {
         getWorld()->myPlayer()->damage(2);
         getWorld()->Sound(SOUND_PLAYER_IMPACT);
         setVisible(false);
         setLive(false);
     }
 }

 void Peas::detect() {
     // kill bot prior!!!
     if (getWorld()->peaOnMarbleOrBot(getX(), getY())) {
         if (!getWorld()->haveMarble(getX(), getY())) {
             getWorld()->Sound(SOUND_ROBOT_IMPACT);
         }
         setVisible(false);
         setLive(false);
         return; 
     }
     if (getWorld()->haveWallFac(getX(), getY())) { // nothing to wall or factory
         setVisible(false);
         setLive(false);
     }
     
 }

 bool Peas::isPeas() {
     return true;
 }

 Goodie::Goodie(int x, int y, int ID, StudentWorld* world,int points) :Actor(ID, x, y, true, world) {
     point_ = points;
     setDirection(none);
 }

 void Goodie::doSomething() {
     if (isAlive()) {
         if (getWorld()->playerX() == getX() && getWorld()->playerY() == getY()) {
             getWorld()->addPoints(point_);
             special();
             getWorld()->Sound(SOUND_GOT_GOODIE);
         }
     }
}

 void Goodie::detect() {
     if (getWorld()->playerX()==getX()&&getWorld()->playerY()==getY()) {
         setVisible(false);
         setLive(false);
     }
 }

 void Goodie::special() {

 }

int Goodie::isGoodie() {
     return 1;
 }

 ExtraLifeGoodie::ExtraLifeGoodie(int x, int y, StudentWorld* world):Goodie(x,y,IID_EXTRA_LIFE,world,1000) {

 }

 void ExtraLifeGoodie::special() {
     getWorld()->increaseLive();
 }

 int ExtraLifeGoodie::isGoodie() {
     return 2;
}

 RestoreHealthGoodie::RestoreHealthGoodie(int x, int y, StudentWorld* world) :Goodie(x, y, IID_RESTORE_HEALTH, world, 500) {

 }

 void RestoreHealthGoodie::special() {
     getWorld()->myPlayer()->restore();
 }

 int RestoreHealthGoodie::isGoodie() {
     return 3;
 }

 AmmoGoodie::AmmoGoodie(int x, int y, StudentWorld* world) :Goodie(x, y, IID_AMMO, world, 100) {

 }

 void AmmoGoodie::special() {
     getWorld()->myPlayer()->addPeas();
 }

 int AmmoGoodie::isGoodie() {
     return 4;
 }

 Enemy::Enemy(int ID, int x, int y, StudentWorld* world, int hp) :Actor(ID, x, y, true, world) {
     m_hp = hp;
     int ticks = (28 - getWorld()->getLevel())/4
         ;
     if (ticks < 3) {
         ticks = 3;
     }
     ticCircle = ticks;
 }

 int Enemy::myCir() {
     return ticCircle;
 }

 RageBots::RageBots(int x, int y, StudentWorld* world, int dir) :Enemy(IID_RAGEBOT, x, y, world,10) {
     m_dir = dir;
     setDirection(m_dir);
     currT = 0;
}

 int Actor::random(int start, int end) {
     /*
     std::random_device rd;
     std::mt19937 gen(rd());
     std::uniform_int_distribution<> dis(start, end);
     return dis(gen);
     */
     int random = rand();

     return random % (end - start + 1) + start;
 }

 bool Enemy::isBot() {
     return true;
 }

 void Enemy::damage(int amt) {
     m_hp -= amt;
     if (m_hp > 0) getWorld()->Sound(SOUND_ROBOT_IMPACT);
 }

 bool Enemy::dead() {
     return m_hp <= 0;
 }

 bool Enemy::shoot(int dir) {
     bool DONE = false; // if shoot
     bool OBSTACLENOSHOOT = false; // if can shoot
     switch (dir) {
         Peas* ptr;
     case up:
         if (getX() == getWorld()->playerX() && getY() <= getWorld()->playerY()) {
             for (int i = getY() + 1;i < getWorld()->playerY();i++) {
                 if (getWorld()->haveMarble(getX(), i) || getWorld()->haveWallFac(getX(), i) || getWorld()->haveBot(getX(), i)) {
                     OBSTACLENOSHOOT = true;
                     break;
                 }
             }
             if (OBSTACLENOSHOOT) break;
             ptr = new Peas(getX(), getY() + 1, up, getWorld());
             getWorld()->addActor(ptr);
             getWorld()->Sound(SOUND_ENEMY_FIRE);
             DONE = true;
         }
         break;
     case down:
         if (getX() == getWorld()->playerX() && getY() >= getWorld()->playerY()) {
             for (int i = getY() - 1;i > getWorld()->playerY();i--) {
                 if (getWorld()->haveMarble(getX(), i) || getWorld()->haveWallFac(getX(), i) || getWorld()->haveBot(getX(), i)) {
                     OBSTACLENOSHOOT = true;
                     break;
                 }
             }
             if (OBSTACLENOSHOOT) break;
             ptr = new Peas(getX(), getY() - 1, down, getWorld());
             getWorld()->addActor(ptr);
             getWorld()->Sound(SOUND_ENEMY_FIRE);
             DONE = true;
         }
         break;
     case left:
         if (getY() == getWorld()->playerY() && getX() >= getWorld()->playerX()) {
             for (int i = getX() - 1;i > getWorld()->playerX();i--) {
                 if (getWorld()->haveMarble(i, getY()) || getWorld()->haveWallFac(i, getY()) || getWorld()->haveBot(i, getY())) {
                     OBSTACLENOSHOOT = true;
                     break;
                 }
             }
             if (OBSTACLENOSHOOT) break;
             ptr = new Peas(getX() - 1, getY(), left, getWorld());
             getWorld()->addActor(ptr);
             getWorld()->Sound(SOUND_ENEMY_FIRE);
             DONE = true;
         }
         break;
     case right:
         if (getY() == getWorld()->playerY() && getX() <= getWorld()->playerX()) {
             for (int i = getX() + 1;i < getWorld()->playerX();i++) {
                 if (getWorld()->haveMarble(i, getY()) || getWorld()->haveWallFac(i, getY()) || getWorld()->haveBot(i, getY())) {
                     OBSTACLENOSHOOT = true;
                     break;
                 }
             }
             if (OBSTACLENOSHOOT) break;
             ptr = new Peas(getX() + 1, getY(), right, getWorld());
             getWorld()->addActor(ptr);
             getWorld()->Sound(SOUND_ENEMY_FIRE);
             DONE = true;
         }
         break;

     }
     return DONE;
 }

 void RageBots::doSomething() {
     currT += 1;
     if (dead()) {
         setVisible(false);
         setLive(false);
         getWorld()->Sound(SOUND_ROBOT_DIE);
         getWorld()->addPoints(100);
     }
     if (this->isAlive()) {
         if (currT % myCir() == 0) { // rest or can go
             
             if (!shoot(m_dir)) {
                 switch (m_dir) {
                 case up:
                     if (!getWorld()->obstacleforBot(getX(), getY() + 1)) {
                         moveTo(getX(), getY() + 1);
                     }
                     else {
                         m_dir = down;
                     }
                     break;
                 case down:
                     if (!getWorld()->obstacleforBot(getX(), getY() - 1)) {
                         moveTo(getX(), getY() - 1);
                     }
                     else {
                         m_dir = up;
                     }
                     break;
                 case left:
                     if (!getWorld()->obstacleforBot(getX()-1, getY())) {
                         moveTo(getX()-1, getY());
                     }
                     else {
                         m_dir = right;
                     }
                     break;
                 case right:
                     if (!getWorld()->obstacleforBot(getX() + 1, getY())) {
                         moveTo(getX() + 1, getY());
                     }
                     else {
                         m_dir = left;
                     }
                     break;

                 }
             }
         }
     }

     setDirection(m_dir);
 }

 ThiefBots::ThiefBots(int x, int y, StudentWorld* world) :Enemy(IID_THIEFBOT, x, y, world, 5) {
     m_dir = right;
     setDirection(m_dir);
     m_currT = 0;
     GoodieType = 0; // record what is goodie picked up
     disTogo = random(1, 6);
     disHaveGo = 0;
 }

 bool ThiefBots::shoot(int dir) {
     return false;
 }

 ThiefBots::ThiefBots(int ID, int x, int y, StudentWorld* world, int liv) :Enemy(ID, x, y, world, liv) {
     m_dir = right;
     setDirection(m_dir);
     m_currT = 0;
     GoodieType = 0;
     disTogo = random(1, 6);
 }
 // for derived mean one
 int ThiefBots::getDir() {
     return m_dir;
 }

 void ThiefBots::doSomething() {
     m_currT += 1;
     if (dead()) {
         switch (GoodieType) {
             Goodie* ptr;
         case 2:
             ptr = new ExtraLifeGoodie(getX(), getY(), getWorld());
             getWorld()->addActor(ptr);
             break;
         case 3:
             ptr = new RestoreHealthGoodie(getX(), getY(), getWorld());
             getWorld()->addActor(ptr);
             break;
         case 4:
             ptr = new AmmoGoodie(getX(), getY(), getWorld());
             getWorld()->addActor(ptr);
             break;
         }
         setVisible(false);
         setLive(false);
         getWorld()->Sound(SOUND_ROBOT_DIE);
         getWorld()->addPoints(10);
         //drop the goodies!!!
     }
     bool Picked = false;
     if (isAlive()) {
         if (m_currT % myCir() == 0) {
             if(shoot(m_dir)) return;

             if (getWorld()->haveGoodie(getX(), getY())&&GoodieType==0) {
                 //Some function to set goodie invisable and delete it. Remember the type. 
                 if (random(1, 10) == 1) { 
                     getWorld()->Sound(SOUND_ROBOT_MUNCH);
                     GoodieType = getWorld()->haveGoodie(getX(), getY());
                     getWorld()->deleteGoodie(getX(), getY());
                     Picked = true; // do nothing more
                 }
             }
             bool Canmove = true;
             bool Trapped = false;

             if (!Picked) {

                 switch (m_dir) {
                 case up:
                     if (getWorld()->obstacleforBot(getX(), getY() + 1)||disTogo==disHaveGo) {
                         Trapped = true;
                     }

                     if (!Trapped) {
                             moveTo(getX(), getY() + 1);
                             disHaveGo += 1;
                         Canmove = false;
                     }
                     break;
                 case down:
                     if (getWorld()->obstacleforBot(getX(), getY() - 1) || disTogo == disHaveGo) {
                         Trapped = true;
                     }

                     if (!Trapped) {
                         moveTo(getX(), getY() -1 );
                         disHaveGo += 1;
                         Canmove = false;
                     }
                     break;
                 case left:
                     if (getWorld()->obstacleforBot(getX()-1, getY()) || disTogo == disHaveGo) {
                         Trapped = true;
                     }

                     if (!Trapped) {
                         moveTo(getX()-1, getY());
                         disHaveGo += 1;
                         Canmove = false;
                     }
                     break;
                 case right:
                     if (getWorld()->obstacleforBot(getX()+1, getY()) || disTogo == disHaveGo) {
                         Trapped = true;
                     }

                     if (!Trapped) {
                         moveTo(getX()+1, getY());
                         disHaveGo += 1;
                         Canmove = false;
                     }
                     break;
                 }
             }
             bool NewTrap = false;
             
             if (Canmove && !Picked) {
                 disTogo = random(1, 6);
                 m_dir = random(0, 3);
                 setDirection(m_dir);
                 disHaveGo = 0;
                 m_dir *= 90;
                 bool haveChecked[4] = { false,false,false,false };// right, up, left, down
                 int consider = 0;
                 while (Canmove&&consider<=4) {
                     setDirection(m_dir);
                     consider += 1;
                     switch (m_dir) {
                     case up:
                         if (getWorld()->obstacleforBot(getX(), getY()+1)) {
                             NewTrap = true;
                             haveChecked[1] = true;
                             for (int k = 0;k < 4;k++) {
                                 if (!haveChecked[k]) {
                                     m_dir = k * 90;
                                     break;
                                 }
                             }
                             //consider += 1;
                             break;
                         }

                         if (!NewTrap) {
                             moveTo(getX(), getY()+1);
                             Canmove = false;
                             disTogo += 1;
                         }
                         break;
                     case down:
                         if (getWorld()->obstacleforBot(getX(), getY()-1)) {
                             NewTrap = true;
                             haveChecked[3] = true;
                             for (int k = 0;k < 4;k++) {
                                 if (!haveChecked[k]) {
                                     m_dir = k * 90;
                                     break;
                                 }
                             }
                             //consider += 1;
                             break;
                         }

                         if (!NewTrap) {
                             moveTo(getX(), getY()-1);
                             disTogo += 1;
                             Canmove = false;
                         }
                         break;
                     case left:
                             if (getWorld()->obstacleforBot(getX()-1, getY())) {
                                 NewTrap = true;
                                 haveChecked[2] = true;
                                 for (int k = 0;k < 4;k++) {
                                     if (!haveChecked[k]) {
                                         m_dir = k * 90;
                                         break;
                                     }
                                 }
                                 //consider += 1;
                                 break;
                             }

                             if (!NewTrap) {
                                 moveTo(getX() - 1, getY());
                                 disTogo += 1;
                                 Canmove = false;
                             }
                             break;
                     case right:
                         if (getWorld()->obstacleforBot(getX()+1, getY())) {
                             NewTrap = true;
                             haveChecked[0] = true;
                             for (int k = 0;k < 4;k++) {
                                 if (!haveChecked[k]) {
                                     m_dir = k * 90;
                                     break;
                                 }
                             }
                             //consider += 1;
                             break;
                         }

                         if (!NewTrap) {
                             moveTo(getX() + 1, getY());
                             disTogo += 1;
                             Canmove = false;
                         }

                         break;
                     }
                 }

             }
             
         }
     }
 }



 Factory::Factory(int x, int y, StudentWorld* world, bool meanOrNot) :Actor(IID_ROBOT_FACTORY, x, y, true, world) {
     setDirection(none);
     if (meanOrNot) {
         mean = true;
     }
     else {
         mean = false;
     }
     roboCount = 0;
 }

 void Factory::doSomething() {
     roboCount = getWorld()->thiefCount(getX() - 3, getY() - 3, getX() + 3, getY() + 3);
     if (roboCount < 3 && !getWorld()->haveTB(getX(),getY())) {
         if (random(1, 50) == 7) { // I LIKE 7! SIUUUUUUUUU
             ThiefBots* ptr;
             if (mean) {
                 // MTB here
                 ptr = new MeanThiefBots(getX(), getY(), getWorld());
                 getWorld()->addActor(ptr);
                 getWorld()->Sound(SOUND_ROBOT_BORN);
             }
             else {
                 ptr = new ThiefBots(getX(), getY(), getWorld());
                 getWorld()->addActor(ptr);
                 getWorld()->Sound(SOUND_ROBOT_BORN);
             }
         }
     }
 }

 bool Factory::isFac() {
     return true;
 }

 bool Enemy::isThief() {
     return false;
 }

 bool RageBots::isThief() {
     return false;
 }

 bool ThiefBots::isThief() {
     return true;
 }

 MeanThiefBots::MeanThiefBots(int x, int y, StudentWorld* world) :ThiefBots(IID_MEAN_THIEFBOT, x, y, world,8) {

 }

 void MeanThiefBots::doSomething() {
     if (dead()) {
         getWorld()->addPoints(10);// 20=10+10
     }
     ThiefBots::doSomething();
 }

 bool MeanThiefBots::shoot(int dir) {
     return Enemy::shoot(getDir()); // thief cannot shoot
 }
