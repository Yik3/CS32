#include "StudentWorld.h"
#include "GameConstants.h"
#include <string>
#include <iostream>
#include <sstream>
#include <iomanip>
using namespace std;

GameWorld* createStudentWorld(string assetPath)
{
	return new StudentWorld(assetPath);
}

// Students:  Add code to this file, StudentWorld.h, Actor.h, and Actor.cpp

StudentWorld::StudentWorld(string assetPath)
: GameWorld(assetPath)
{
    points = 0;
    bonus = 1000;
    numCry = 0;
    playerLive = 3;
}

int StudentWorld::init()
{
    bonus = 1000; // every game 1000 bonus is given
    numCry = 0;
    return loadNewLevel(); 
    
    return GWSTATUS_CONTINUE_GAME;
}

int StudentWorld::move()
{
    if (playerLive <= 0) {

     }
    for (int i = 0;i < actors_.size();i++) { // let peas shoot first
        if (actors_[i]->isPeas())actors_[i]->doSomething();
    }
    player_->doSomething();
    if (!player_->isAlive()) {
        playerLive -= 1;
        decLives();
        playSound(SOUND_PLAYER_DIE);
        return GWSTATUS_PLAYER_DIED;//start a new round
    }

    for(int i=0;i<actors_.size();i++){ //other actors do
        if(!actors_[i]->isPeas())actors_[i]->doSomething();
    }

    for (int i = 0;i < actors_.size();i++) { 
        actors_[i]->detect();
        if (actors_[i]->playerGo()) { // end a level
            points += 2000;
            points += bonus;
            increaseScore(points);
            return GWSTATUS_FINISHED_LEVEL;
        }
    }

    for (int j = 0;j < actors_.size();j++) { 
        if (!actors_[j]->isAlive()) {
            actors_[j]->setVisible(false);
            delete actors_[j];
            actors_.erase(actors_.begin() + j);
            j--;// Make sure every element is checked
        }
    }

    if(bonus>0)bonus-=1;

    ostringstream oss;
    oss << "Score: ";
    oss.fill('0');
        oss<<setw(7)<<points;

    oss << "  " << "Level: ";
        oss << setw(2)<<getLevel();


    oss.fill(' ');
    oss << "  " << "Lives: ";
    oss << setw(2) << playerLive;
    oss << "  " << "Health: ";
    oss << setw(3) << player_->perHP() << '%';
    oss << "  " << "Ammo: ";
    oss << setw(3) << player_->nPeas();
    oss << "  " << "Bonus: ";
    oss << setw(4) << bonus;
    string output = oss.str();
    setGameStatText(output);
    
	return GWSTATUS_CONTINUE_GAME;
}

void StudentWorld::cleanUp()
{
    
    delete  player_;
    player_ = nullptr;
    vector<Actor*>::iterator it = actors_.begin();
    while (it != actors_.end()) {
        delete* it;
        it = actors_.erase(it);
    }
    actors_.clear();
    /*
    for(int i=0;i<actors_.size();i++){
        delete  actors_[i];
    }
    */
    
}

void  StudentWorld::addActor(Actor* actor){
    actors_.push_back(actor);
}

void StudentWorld::Sound(int ID){
    playSound(ID);
}

int StudentWorld::loadNewLevel(){
    Level lev(assetPath());
    int leve = getLevel();
    char mylev1='0';
    mylev1 += leve % 10;
    char mylev2 = '0';
    mylev2 += (leve % 100)/10; // only extra level
    std::string mid2 = "  ";
    mid2[0] = mylev2;
    mid2[1] = mylev1;
    std::string end = ".txt";
    std::string path = "level"+mid2+end;
    Level::LoadResult result =lev.loadLevel(path);
    if(getLevel()>=99){ // not considering level 99 case here 
        return GWSTATUS_PLAYER_WON;
    }
    if(result==Level::load_fail_bad_format||result==Level::load_fail_file_not_found){
        return GWSTATUS_LEVEL_ERROR;
    }
    for(int i=0;i<15;i++){
        for(int j=0;j<15;j++){
            Level::MazeEntry item = lev.getContentsOf(i,j);
            Actor* add;
            Avatar* addp;
            switch(item){
                case Level::empty:
                break;

                case Level::exit:
                add = new Exit(i,j, this);
                addActor(add);
                break;

                case Level::player:

                addp = new Avatar(i,j,this);
                player_ = addp;
                break;

                case Level::wall:
                    add = new Wall(i, j,this);
                //Actor *obj3 = new Wall(i,j);//not sure
                addActor(add);
                break;

                case Level::crystal:
                    add = new Crystal(i, j, this);
                    addActor(add);
                    numCry += 1; // for exit occur
                    break;

                case Level::marble:
                    add = new Marble(i, j, this);
                    addActor(add);
                    break;

                case Level::pit:
                    add = new Pit(i, j, this);
                    addActor(add);
                    break;

                case Level::ammo:
                    add = new AmmoGoodie(i, j, this);
                    addActor(add);
                    break;

                case Level::extra_life:
                    add = new ExtraLifeGoodie(i, j, this);
                    addActor(add);
                    break;

                case Level::restore_health:
                    add = new RestoreHealthGoodie(i, j, this);
                    addActor(add);
                    break;

                case Level::horiz_ragebot:
                    add = new RageBots(i, j, this, 0);
                    addActor(add);
                    break;

                case Level::vert_ragebot:
                    add = new RageBots(i, j, this, 270);
                    addActor(add);
                    break;

                case Level::thiefbot_factory:
                    add = new Factory(i, j, this, false);
                    addActor(add);
                    break;
                
                case Level::mean_thiefbot_factory:
                    add = new Factory(i, j, this, true);
                    addActor(add);
                    break;
            }
        }
    }

    return GWSTATUS_CONTINUE_GAME;

}

bool StudentWorld::playerAndPeas(){
    for(int i=0;i<actors_.size();i++){
        if((actors_[i])->isPeas()) {
            if(actors_[i]->getX()==player_->getX()&&actors_[i]->getY()==player_->getY()){
                actors_[i]->setLive(false);
                actors_[i]->setVisible(false);
                return true;
            }
        }
    }

    return false;
}

bool StudentWorld::playerCanMove(int dir){
    
    for(int i=0;i<actors_.size();i++){
        if ((actors_[i])->isWall()||actors_[i]->isPit()||actors_[i]->isBot()||actors_[i]->isFac()) { // change later
            switch (dir) {
            case 0: //right
                if (actors_[i]->getX() == player_->getX() + 1 && actors_[i]->getY() == player_->getY()) {
                    return false;
                }
                break;
            case 180://left
                if (actors_[i]->getX() == player_->getX() - 1 && actors_[i]->getY() == player_->getY()) {
                    return false;
                }
                break;
            case 90://up
                if (actors_[i]->getX() == player_->getX() && actors_[i]->getY() == player_->getY() + 1) {
                    return false;
                }
                break;
            case 270://down
                if (actors_[i]->getX() == player_->getX() && actors_[i]->getY() == player_->getY() - 1) {
                    return false;
                }
                break;

            }
        }

        if (actors_[i]->isMarble()) { // push Marble
            switch (dir) {
            case 0: //right
                if (actors_[i]->getX() == player_->getX() + 1 && actors_[i]->getY() == player_->getY()) {
                    for (int j = 0;j < actors_.size();j++) {
                        if (actors_[j]-> isWall()|| actors_[j]->isMarble()||actors_[j]->isBot()||actors_[j]->isFac()|| actors_[j]->isCrystal()|| actors_[j]->isGoodie()) {// ADD CRYSTAL
                            if (actors_[j]->getX() == actors_[i]->getX() + 1 && actors_[j]->getY() == actors_[i]->getY()) {
                                return false;
                            }
                        }
                    }
                    
                }
                break;
            case 180://left
                if (actors_[i]->getX() == player_->getX() - 1 && actors_[i]->getY() == player_->getY()) {
                    for (int j = 0;j < actors_.size();j++) {
                        if (actors_[j]->isWall() || actors_[j]->isMarble() || actors_[j]->isBot() || actors_[j]->isFac() || actors_[j]->isCrystal() || actors_[j]->isGoodie()) {
                            if (actors_[j]->getX() == actors_[i]->getX() - 1 && actors_[j]->getY() == actors_[i]->getY()) {
                                return false;
                            }
                        }
                    }
                }
                break;
            case 90://up
                if (actors_[i]->getX() == player_->getX() && actors_[i]->getY() == player_->getY() + 1) {
                    for (int j = 0;j < actors_.size();j++) {
                        if (actors_[j]->isWall() || actors_[j]->isMarble() || actors_[j]->isBot() || actors_[j]->isFac() || actors_[j]->isCrystal() || actors_[j]->isGoodie()) {
                            if (actors_[j]->getX() == actors_[i]->getX()&& actors_[j]->getY() == actors_[i]->getY()+1) {
                                return false;
                            }
                        }
                    }
                }
                break;
            case 270://down
                if (actors_[i]->getX() == player_->getX() && actors_[i]->getY() == player_->getY() - 1) {
                    for (int j = 0;j < actors_.size();j++) {
                        if (actors_[j]->isWall() || actors_[j]->isMarble() || actors_[j]->isBot() || actors_[j]->isFac() || actors_[j]->isCrystal() || actors_[j]->isGoodie()) {
                            if (actors_[j]->getX() == actors_[i]->getX()&& actors_[j]->getY() == actors_[i]->getY()-1) {
                                return false;
                            }
                        }
                    }
                }
                break;

            }// Marble is one place adjacent to player and one place after marble is not obstacle
        }
    }
    return true;
}

void StudentWorld::decreaseCrystal() {
    if(numCry>0)numCry -= 1;
}

int StudentWorld::playerX() {
    return player_->getX();
}
int StudentWorld::playerY() {
    return player_->getY();
}

Avatar* StudentWorld::myPlayer() {
    return player_;
}

bool StudentWorld::haveMarble(int x, int y) {
    for (int j = 0;j < actors_.size();j++) {
        if (actors_[j]->isMarble()) {
            if (x == actors_[j]->getX() && y == actors_[j]->getY()) {
                return true;
            }
        }
    }
    return false;
}

bool StudentWorld::havePit(int x, int y) {
    for (int j = 0;j < actors_.size();j++) {
        if (actors_[j]->isPit()) {
            if (x == actors_[j]->getX() && y == actors_[j]->getY()) {
                return true;
            }
        }
    }
    return false;
}

bool StudentWorld::haveWallFac(int x, int y) {
    for (int j = 0;j < actors_.size();j++) {
        if (actors_[j] ->isWall()||actors_[j]->isFac()) {
            if (x == actors_[j]->getX() && y == actors_[j]->getY()) {
                return true;
            }
        }
    }
    return false;
}

bool StudentWorld::peaOnMarbleOrBot(int x, int y) {
    for (int j = 0;j < actors_.size();j++) {
        if (actors_[j]->isMarble()||actors_[j]->isBot()) { 
            if (x == actors_[j]->getX() && y == actors_[j]->getY()) {
                actors_[j]->damage(2); // -2 hp
                return true;
            }
        }
    }
    return false;
}

int StudentWorld::haveGoodie(int x, int y) {
    for (int j = 0;j < actors_.size();j++) {
        if (actors_[j]->isGoodie()!=0) { // 0 is not Goodie
            if (x == actors_[j]->getX() && y == actors_[j]->getY()) {
                return actors_[j]->isGoodie();
            }
        }
    }
    return 0;
}

void StudentWorld::deleteGoodie(int x, int y) {
    if (haveGoodie(x, y)) {
        for (int j = 0;j < actors_.size();j++) {
            if (actors_[j]->isGoodie() != 0) {// is a goodie
                if (x == actors_[j]->getX() && y == actors_[j]->getY()) {
                    actors_[j]->setLive(false);
                    actors_[j]->setVisible(false);
                    break;
                }
            }
        }
    }
}

bool StudentWorld::allCrystalCollected() {
    return (numCry == 0);
}

void StudentWorld::addPoints(int num) {
    points += num;
}
StudentWorld::~StudentWorld(){
    if(player_!=nullptr)cleanUp(); // prevent multiple calling
}

void StudentWorld::increaseLive() {
    playerLive += 1;
    incLives();
}

bool StudentWorld::haveBot(int x, int y) {
    for (int j = 0;j < actors_.size();j++) {
        if (actors_[j]->isBot()) {
            if (x == actors_[j]->getX() && y == actors_[j]->getY()) {
                return true;
            }
        }
    }
    return false;
}

bool StudentWorld::obstacleforBot(int x, int y) {
    if (haveMarble(x, y)) {
        return true;
    }
    if (havePit(x, y)) {
        return true;
    }
    if (haveWallFac(x, y)) {
        return true;
    }
    if (haveBot(x, y)) {
        return true;
    }
    if (playerX() == x && playerY() == y) {
        return true;
    }
    return false;
}

int StudentWorld::thiefCount(int sx, int sy, int ex, int ey) {
    int count = 0;
    for (int j = 0;j < actors_.size();j++) {
        if (actors_[j]->isThief()) {
            if (actors_[j]->getX() >= sx && actors_[j]->getX() <= ex && actors_[j]->getY() >= sy && actors_[j]->getY() <= ey) {
                count += 1; // in a range(inclusive)
            }
        }
    }
    return count;
}

bool StudentWorld::haveTB(int x, int y) {
    for (int j = 0;j < actors_.size();j++) {
        if (actors_[j]->isThief()) { 
            if (x == actors_[j]->getX() && y == actors_[j]->getY()) {
                return true;
            }
        }
    }
    return false;
}