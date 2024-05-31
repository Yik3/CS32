#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"

class StudentWorld;
// Students:  Add code to this file, Actor.cpp, StudentWorld.h, and StudentWorld.cpp
class Actor:public GraphObject{

    public:
    // Constructor
    Actor(int ID, int x,int y, bool visable,StudentWorld *world);
    virtual void doSomething();
    virtual void detect();
    virtual bool playerGo();
    void setLive(bool stat);
    bool isAlive() const;
    StudentWorld* getWorld();
    virtual bool canMove();
    virtual bool isWall();
    virtual bool isPit();
    virtual bool isMarble();
    virtual bool isCrystal();
    virtual bool isPeas();
    virtual bool isBot();
    virtual int isGoodie();
    virtual bool isFac();
    virtual void damage(int amt);
    virtual bool isThief();
    int random(int start, int end);

    private:
    bool LIVE;
    StudentWorld *ptr;

};
class Peas: public Actor{
    public:
    Peas(int x,int y,int dir, StudentWorld *world);
    void doSomething();
    void detect();
    bool isPeas();
private:
    int dir_;
};


class Avatar : public Actor {
    public:
        Avatar(int x,int y, StudentWorld *world);
        void doSomething();
        virtual bool canMove();
        void restore(); 
        void addPeas();
        int perHP();
        int nPeas();
        void damage(int amt);
    
    private:
        double hitPoints;
        int peas;
};
class Crystal :public Actor {
public:
    Crystal(int x, int y, StudentWorld* world);
    void doSomething();
    bool isCrystal();
};

class Marble :public Actor {
public:
    Marble(int x, int y, StudentWorld* world);
    void doSomething();
    void detect();
    bool isMarble();
    void damage(int amt);

private:
    int hp_;
};

class Pit :public Actor {
public:
    Pit(int x, int y, StudentWorld* world);
    void detect();
    bool isPit();
};
class  Enemy : public Actor {
public:
    Enemy(int ID, int x, int y, StudentWorld* world, int hp);
    int myCir();
    bool isBot();
    void damage(int amt);
    bool dead();
    virtual bool shoot(int dir);
    
    virtual bool isThief();
private:
    int m_hp;
    int ticCircle;
};


class RageBots:public  Enemy{
public:
    RageBots(int x, int y, StudentWorld* world, int dir);
    void doSomething();
    virtual bool isThief();
    //bool shoot(int dir);

private:
    int m_dir;
    int currT;
};

class ThiefBots:public Enemy{
public:
    ThiefBots(int x, int y, StudentWorld* world);
    ThiefBots(int ID, int x, int y, StudentWorld* world, int liv);
    virtual void doSomething();
    bool isThief();
    virtual bool shoot(int dir);
    int getDir();

private:
    int m_dir;
    int m_currT;
    int disTogo;
    int disHaveGo;
    int GoodieType;
};

class MeanThiefBots:public ThiefBots{
    public:
        MeanThiefBots(int x, int y, StudentWorld* world);
        void doSomething();
        bool shoot(int dir);
};

class Factory:public Actor{
public:
    Factory(int x, int y, StudentWorld* world, bool meanOrNot);
    void doSomething();
    bool isFac();
private:
    int roboCount;
    bool mean;
};

class Exit :public Actor{
    public:
    Exit(int x,int y,StudentWorld *world);
    void doSomething();
    bool playerGo();

private:
    bool canShow;
    bool firstShow;
    bool playergo_;
};

class Wall: public Actor{
    public:
    Wall(int x, int y, StudentWorld* world);
    bool isWall();
    
};

class Goodie : public Actor {
public:
    Goodie(int x, int y, int ID, StudentWorld* world, int point);
    virtual void doSomething();
    void detect();
    virtual void special();
    virtual int isGoodie();

private:
    int point_;
};

class ExtraLifeGoodie : public Goodie {
public:
    ExtraLifeGoodie(int x, int y, StudentWorld* world);
    void special();
    int isGoodie();
};

class RestoreHealthGoodie : public Goodie {
public:
    RestoreHealthGoodie(int x, int y, StudentWorld* world);
    void special();
    int isGoodie();
};

class AmmoGoodie : public Goodie {
public:
    AmmoGoodie(int x, int y, StudentWorld* world);
    void special();
    int isGoodie();
};


#endif // ACTOR_H_
