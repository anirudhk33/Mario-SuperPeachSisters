#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"


// Students:  Add code to this file, Actor.cpp, StudentWorld.h, and StudentWorld.cpp

class StudentWorld;

class GameObject : public GraphObject{
public:
    GameObject(StudentWorld* world, int imageID, int startX, int startY, int dir, int depth, double size);
    virtual void doSomething() = 0;
    void setDead();
    void setAlive();
    virtual bool is_alive() const;        // is a game object still alive or should it be removed from game?
    StudentWorld* getWorld();
    virtual ~GameObject();
    
    virtual bool is_a_square() const = 0; //MAKE ABSTRACT
    virtual bool can_be_hit_by_vortex() const = 0; //MAKE ABSTRACT
    virtual int getForcingDirIfExists(); //Default return
    virtual void hit_by_vortex(){;};  // tell an game object it has been hit by a vortex
    void setCanBeHitByVortex(bool set);
    bool getCanBeHitByVortex();
    
private:
    bool alive;
    bool canBeHitByVortex;
    bool hitByVortex;
    StudentWorld* myWorldBro;

};


class Player : public GameObject{
public:
    Player(StudentWorld* world, int imageID, int x, int y, int side);
    virtual void doSomething();
    virtual bool is_a_square() const{return false;}
    bool isWaitingToRoll();
    int getWalkDirection();
    void setWaitingToRoll(bool stat);
    void autoChangeDir();
    void setWalkDirection(int dir);
    void walk();
    int nextX(int X);
    int nextY(int Y);
    int divRound(int numerator, int divisor, int dir);
    virtual ~Player();
    int getHasWaitedFor(){return hasWaitedFor;}
    void increaseCoins(int num);
    int decreaseCoins(int num);
    void increaseStars();
    int getCoins() const;   // used to display stats on status line
    void resetCoins(){coins = 0;}
    void resetStars(){stars = 0;}
    int getStars() const;   // used to display stats on status line
    void decreaseStars(){if(stars>0)stars--;}
    void setCoinsTo(int num){coins = num;}
    void setStarsTo(int num){stars = num;}
    bool getInactivated(){return inactivated;}
    void setInactivated(bool set){inactivated=set;}
    int getTicksToMove(){return ticksToMove;}
    void setTicksToMove(int num){ticksToMove = num;}
    bool hasVortex() {return vortex;}
    void setVortex(bool set){vortex = set;}
    void swapPlayers(Player* p1, Player* p2);
    bool forkCondition();
    int oppositeDir(int dir);
    bool getTouchingEnemy(){return touchingEnemy;}
    void setTouchingEnemy(bool set){touchingEnemy=set;}
    virtual bool can_be_hit_by_vortex() const {return false;}; //MAKE ABSTRACT


private:
    int ticksToMove;
    bool waitingToRoll;
    int walkDirection;
    int side;
    int coins;
    int stars;
    bool vortex;
    int hasWaitedFor;
    bool inactivated;
    bool touchingEnemy;
    
};



class ActivatingObject : public GameObject {
public:
    ActivatingObject(StudentWorld *world, int imageID, int startX, int startY, int dir, int depth, double size);
    virtual bool is_a_square() const{return false;}
    virtual void doSomething();
};



class ActivateOnPlayer : public ActivatingObject {
public:
    ActivateOnPlayer(StudentWorld *world, int imageID, int startX, int startY,
                   int dir, int depth, double size);
    bool hasPlayerLanded(Player* Player);
private:
};



class CoinSquare : public ActivateOnPlayer{
public:
    CoinSquare(StudentWorld* world, int imageID, int x, int y, bool givesCoins);
    bool is_a_square() const {return true;};
    bool can_be_hit_by_vortex() const {return false;};
    virtual void doSomething();
private:
    bool givesCoins;
};



class StarSquare : public ActivateOnPlayer {
public:
  StarSquare(StudentWorld *world, int imageID, int startX, int startY);
    virtual void doSomething();
    void doEverything(Player* player);
    bool is_a_square() const {return true;}
    bool can_be_hit_by_vortex() const {return false;}
private:
};


class DirectionalSquare : public ActivateOnPlayer {
public:
    DirectionalSquare(StudentWorld *world, int imageID, int startX, int startY, int direction);
    virtual void doSomething();
    bool is_a_square() const {return true;};
    bool can_be_hit_by_vortex() const {return false;}
    virtual int getForcingDirIfExists(){return forcingDir;}
private:
    int forcingDir;
};


class BankSquare : public ActivateOnPlayer {
public:
  BankSquare(StudentWorld *world, int imageID, int startX, int startY);
    virtual void doSomething();
    void doEverything(Player* player);
    bool is_a_square() const {return true;};
    bool can_be_hit_by_vortex() const {return false;};
};


class EventSquare : public ActivateOnPlayer {
public:
    virtual void doSomething();
    void doEverything(Player* player);
    void teleport(Player* player);
    void swap(Player* player1, Player* player2);
    void giveVortex(Player* player);
    EventSquare(StudentWorld *world, int imageID, int startX, int startY);
    bool is_a_square() const {return true;}
    bool can_be_hit_by_vortex() const {return false;}
};

class DroppingSquare : public ActivateOnPlayer {
public:
  DroppingSquare(StudentWorld *world, int imageID, int startX, int startY);
    virtual  void doSomething();
    void doEverything(Player* player);
    bool is_a_square() const {return true;}
    bool can_be_hit_by_vortex() const {return false;}
};


class Enemy : public ActivateOnPlayer {
public:
      
    Enemy(StudentWorld *world, int imageID, int startX, int startY,
            int dir, double depth, int size, int num_sq_to_move, int number_of_ticks_to_pause);
    virtual void doSomething();
    virtual void doEverything();
    bool is_a_square() const {return false;}
//    bool can_be_hit_by_vortex() const;
    virtual void hit_by_vortex();  // called when enemy is hit by a vortex projectile (called by vortex projectile)
    virtual void doPausedStateActivity(Player* player) = 0;
    bool getIsPaused(){return isPaused;}
    void setIsPause(bool set){isPaused = set;}
    int getWalkDirection(){return walkingDirection;}
    int getSqrsToMove(){return numSqrsToMove;}
    int getTicksToPause(){return numTicksToPause;}
    void setTicksToPause(int num){numTicksToPause=num;}
    void decreasePause(){numTicksToPause--;}
    void setSqrsToMove(int num){numSqrsToMove=num;}
    void setTicksToMove(int num){numTicksToMove = num;}
    int getTicksToMove(){return numTicksToMove;}
    int pickRandomValidDir();
    void setWalkDirection(int dir);
    bool onSqr();
    bool isAtFork();
    bool isAtTurningPoint();
    int oppositeDir(int dir);
    void autoChangeDir();
    void walk();
    virtual void mayPerformAction()=0;
    bool hasEnemyLanded(Player* Player);
    virtual int getMaxTicks() = 0;
    virtual bool can_be_hit_by_vortex() const {return true;}

    
private:
    bool isPaused;
    int walkingDirection;
    int spriteDirection;
    int numSqrsToMove;
    int numTicksToPause;
    int numTicksToMove;
    
};
class Bowser : public Enemy {
public:
    Bowser(StudentWorld *world, int imageID, int startX, int startY);
    virtual void doPausedStateActivity(Player* player);
    virtual void mayPerformAction();
    virtual int getMaxTicks() {return 10;}


};

class Boo : public Enemy {
public:
    Boo(StudentWorld *world, int imageID, int startX, int startY);
    virtual void doPausedStateActivity(Player* player);
    virtual void mayPerformAction();
    virtual int getMaxTicks() {return 3;}
};



class Vortex : public ActivatingObject {
public:
    Vortex(StudentWorld *world, int imageID, int startX, int startY, int dir);
    bool is_a_square() const {return false;}
    bool can_be_hit_by_vortex() const {return false;}
    std::vector<GameObject*> do_i_activate();
    void doSomething();
    void move();
    int getMovingDirection(){return movingDirection;}
    bool vortexOverlaps();
private:
    int movingDirection;
};



#endif // ACTOR_H_



