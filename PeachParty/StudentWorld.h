#ifndef STUDENTWORLD_H_
#define STUDENTWORLD_H_

#include "GameWorld.h"
#include "Board.h"
#include "Actor.h"
#include <string>
#include <vector>

class GameObject;
class ManualActor;

// Students:  Add code to this file, StudentWorld.cpp, Actor.h, and Actor.cpp

class StudentWorld : public GameWorld
{
public:
    StudentWorld(std::string assetPath);
    virtual int init();
    virtual int move();
    virtual void cleanUp();
    virtual ~StudentWorld();
    Board returnBoard();
    Player* returnPeach();
    Player* returnYoshi();
    bool isValidPos(int x, int y);
    int isPlayerOnDirectionalSquare(Player* obj);
    int returnBankBalance();
    void setBankBalance(int num);
    void randomSqr(int& x, int& y);
    bool isEnemyOnThisSqr(int x, int y);
    void depositDroppingSquare(int x, int y);
    void coinSwap(Player* p1, Player* p2);
    void starSwap(Player* p1, Player* p2);
    bool doTheyOverlap(GameObject *obj1, GameObject *obj2);
    GameObject* overlapsWithVortex(GameObject* Vortex);
    void addVortex(int x, int y, int dir);
    
private:
    Player* peach;
    Player* yoshi;
    std::vector<GameObject*> actors;
    Board bd;
    bool cleaned;
    int bankBalance;
};



#endif // STUDENTWORLD_H_
