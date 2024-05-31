#ifndef STUDENTWORLD_H_
#define STUDENTWORLD_H_

#include "GameWorld.h"
#include "Level.h"
#include <string>
#include <vector>

// Students:  Add code to this file, StudentWorld.cpp, Actor.h, and Actor.cpp
#include "Actor.h"
//class Actor;

class StudentWorld : public GameWorld
{
public:
  StudentWorld(std::string assetPath);
  virtual int init();
  virtual int move();
  virtual void cleanUp();
  void addPoints(int num);
  void  addActor(Actor* actor);
  int loadNewLevel();
  void Sound(int ID);
  bool playerAndPeas();
  bool playerCanMove(int dir);
  void decreaseCrystal();
  Avatar* myPlayer();
  bool haveMarble(int x, int y);
  bool havePit(int x, int y);
  bool haveWallFac(int x, int y);
  bool haveBot(int x, int y);
  int haveGoodie(int x, int y);
  void deleteGoodie(int x, int y);
  bool peaOnMarbleOrBot(int x, int y);
  bool obstacleforBot(int x, int y);
  bool allCrystalCollected();
  int playerX();
  int playerY();
  void increaseLive();
  int thiefCount(int sx, int sy, int ex, int ey);
  bool haveTB(int x, int y);
  ~StudentWorld();
  
private:
	
Level* curLevel_; // The level the player is currently in
Avatar * player_;
std::vector<Actor*> actors_;
int playerLive;
int points;
int bonus;
int numCry;
};

#endif // STUDENTWORLD_H_
