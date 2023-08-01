#include "Actor.h"
#include "StudentWorld.h"
#include "Board.h"

// Students:  Add code to this file, Actor.h, StudentWorld.h, and StudentWorld.cpp



////////////////////////////CONSTRUCTORS////////////////////////////////////////////////

GameObject::GameObject(StudentWorld* world,int imageID, int startX, int startY, int dir, int depth, double size) : GraphObject(imageID, startX*SPRITE_WIDTH, startY*SPRITE_HEIGHT, dir, depth, size), myWorldBro(world),alive(true),canBeHitByVortex(false){}



Player::Player(StudentWorld* world, int imageID, int x, int y, int side) : GameObject(world, imageID, x, y, right, 0, 1), ticksToMove(0), waitingToRoll(true), walkDirection(right), side(side), coins(0), stars(0), vortex(false), hasWaitedFor(0), inactivated(true), touchingEnemy(false){setDirection(right);}



ActivatingObject::ActivatingObject(StudentWorld *world, int imageID, int startX, int startY, int dir, int depth, double size) : GameObject(world, imageID, startX, startY, dir, depth, size){}



ActivateOnPlayer::ActivateOnPlayer(StudentWorld *world, int imageID, int startX, int startY,
                                   int dir, int depth, double size) : ActivatingObject(world, imageID, startX, startY, dir, depth, size){}



CoinSquare::CoinSquare(StudentWorld* world, int imageID, int startX, int startY, bool givesCoins) :ActivateOnPlayer(world, imageID, startX, startY, right, 1, 1), givesCoins(givesCoins) {}



StarSquare::StarSquare(StudentWorld *world, int imageID, int startX, int startY) :ActivateOnPlayer(world, imageID, startX, startY, right, 1, 1){}



DirectionalSquare::DirectionalSquare(StudentWorld *world, int imageID, int startX, int startY, int direction) : ActivateOnPlayer(world, imageID, startX, startY, direction, 1, 1), forcingDir(direction){}



BankSquare::BankSquare(StudentWorld *world, int imageID, int startX, int startY) :ActivateOnPlayer(world, imageID, startX, startY, right, 1, 1){}


EventSquare::EventSquare(StudentWorld *world, int imageID, int startX, int startY): ActivateOnPlayer(world, imageID, startX, startY, right, 1, 1){}


DroppingSquare::DroppingSquare(StudentWorld *world, int imageID, int startX, int startY) : ActivateOnPlayer(world, imageID, startX, startY, right, 1, 1){}


Enemy::Enemy(StudentWorld *world, int imageID, int startX, int startY, int dir, double depth, int size, int num_sq_to_move, int number_of_ticks_to_pause):ActivateOnPlayer(world, imageID, startX, startY, dir, depth, size), walkingDirection(right), numSqrsToMove(num_sq_to_move), numTicksToPause(number_of_ticks_to_pause), isPaused(true){setDirection(right);}


Bowser::Bowser(StudentWorld *world, int imageID, int startX, int startY):Enemy(world, imageID, startX, startY, right, 0, 1, 0, 180){setCanBeHitByVortex(true);}


Boo::Boo(StudentWorld *world, int imageID, int startX, int startY):Enemy(world, imageID, startX, startY, right, 0, 1, 0, 180){setCanBeHitByVortex(true);}

Vortex::Vortex(StudentWorld *world, int imageID, int startX, int startY, int dir) : ActivatingObject(world, imageID, startX, startY, dir, 0, 1), movingDirection(dir){}


//EMPTY DESTRUCTORS
GameObject::~GameObject(){}
Player::~Player(){}


////////////////////////////////////////////////////////////////////////////////////////////////////
//..........TRIVIAL FUNCTIONS..........

void GameObject::setAlive(){alive = true;}

int GameObject::getForcingDirIfExists(){return -1;}

void GameObject::setCanBeHitByVortex(bool set){canBeHitByVortex = set;}

bool GameObject::getCanBeHitByVortex(){return canBeHitByVortex;}

bool Player::isWaitingToRoll(){
    return waitingToRoll;
}

int Player::getWalkDirection(){
    return walkDirection;
}

void Player::setWalkDirection(int dir){
    walkDirection = dir;
    if(getWalkDirection()==left)
        setDirection(left);
    else
        setDirection(right);
}

void Player::setWaitingToRoll(bool stat){
    waitingToRoll = stat;
}

void Player::increaseCoins(int num){
    coins += num;
}


int Player::decreaseCoins(int num){
    if(coins>=num){
        coins -= num;
        return num;
    }
    else{
        int temp = coins;
        coins = 0;
        return temp;
    }
}

void Player::increaseStars(){
    stars++;
}

int Player::getCoins() const{
    return coins;
}

int Player::getStars() const{
    return stars;
}




////////////////////////////////////////////////////////////////////////////////////////////////////
//..........PLAYER NON-TRIVIAL FUNCTIONS..........
void Player::walk(){
    int dir = getWalkDirection();
    switch(dir){
        case left:
            moveTo(getX()-2, getY());
            break;
        case right:
            moveTo(getX()+2, getY());
            break;
        case up:
            moveTo(getX(), getY()+2);
            break;
        case down:
            moveTo(getX(), getY()-2);
            break;
        default:
            break;
    }
}

void Player::autoChangeDir(){
    int dir = getWalkDirection();
    if(dir == right){
        int xnew, ynew;
        getPositionInThisDirection(up, SPRITE_HEIGHT, xnew, ynew);
        if(getWorld()->isValidPos(xnew/16,ynew/16) && getX()%16==0 && getY()%16==0)
            setWalkDirection(up);
        else setWalkDirection(down);
    }
    else if (dir == left){
        int xnew, ynew;
        getPositionInThisDirection(up, SPRITE_HEIGHT, xnew, ynew);
        if(getWorld()->isValidPos(xnew/16,ynew/16) && getX()%16==0 && getY()%16==0)
            setWalkDirection(up);
        else setWalkDirection(down);
    }
    else if (dir == up){
        int xnew, ynew;
        getPositionInThisDirection(left, SPRITE_WIDTH, xnew, ynew);
        if(getWorld()->isValidPos(xnew/16,ynew/16) && getX()%16==0 && getY()%16==0)
            setWalkDirection(left);
        else setWalkDirection(right);
    }
    else if(dir == down){
        int xnew, ynew;
        getPositionInThisDirection(left, SPRITE_WIDTH, xnew, ynew);
        if(getWorld()->isValidPos(xnew/16,ynew/16) && getX()%16==0 && getY()%16==0)
            setWalkDirection(left);
        else setWalkDirection(right);
    }
}


bool Player::forkCondition(){
    if(getX()%16!=0 || getY()%16!=0) return false;
    int count = 0;
    int dir[4] = {right, left, up, down};
    for(int i = 0; i<4; i++){
        int newX, newY;
        getPositionInThisDirection(dir[i], 16, newX, newY);
        if(getWorld()->isValidPos(newX/SPRITE_WIDTH, newY/SPRITE_HEIGHT)){
            count++;
        }
    }
    return count>=3;
}




void Player::doSomething(){
    int a,b,dir;
        if(isWaitingToRoll()){
        int action = getWorld()->getAction(side);
        switch (action) {
            case ACTION_ROLL:
                ticksToMove = (randInt(1, 10)*8);
                setWaitingToRoll(false);
                break;
            case ACTION_FIRE:
                if(hasVortex()){
                    dir = getWalkDirection();
                    getPositionInThisDirection(dir, SPRITE_WIDTH, a, b);
                    getWorld()->addVortex(a/SPRITE_WIDTH,b/SPRITE_HEIGHT,dir);
                    getWorld()->playSound(SOUND_PLAYER_FIRE);
                    setVortex(false);
                }
                return;
                break;
            default:
                hasWaitedFor++;
                return;
        }
    }
    
    if(!isWaitingToRoll()){
        int onDSQ = getWorld()->isPlayerOnDirectionalSquare(this);
        if(onDSQ != -1){
            setWalkDirection(onDSQ);
        }
        else if(forkCondition()){
//            std::cerr << "CHECKBRO ";
//            std::cerr<<"("<<getX()<<","<<getY()<<")";
            int action = getWorld()->getAction(side);
            int x,y;
            switch(action){
                case ACTION_UP:
                    if(up==oppositeDir(getWalkDirection())) return;
                    getPositionInThisDirection(up, SPRITE_WIDTH, x, y);
                    if(getWorld()->isValidPos(x/16, y/16) && getX()%16==0 && getY()%16==0)
                        setWalkDirection(up);
                    else return;
                    break;
                case ACTION_DOWN:
                    if(down==oppositeDir(getWalkDirection())) return;
                    getPositionInThisDirection(down, SPRITE_WIDTH, x, y);
                    if(getWorld()->isValidPos(x/16, y/16) && getX()%16==0 && getY()%16==0)
                        setWalkDirection(down);
                    else return;
                    break;
                case ACTION_RIGHT:
                    if(right==oppositeDir(getWalkDirection())) return;
                    getPositionInThisDirection(right, SPRITE_WIDTH, x, y);
                    if(getWorld()->isValidPos(x/16, y/16) && getX()%16==0 && getY()%16==0)
                        setWalkDirection(right);
                    else return;
                    break;
                case ACTION_LEFT:
                    if(left==oppositeDir(getWalkDirection())) return;
                    getPositionInThisDirection(left, SPRITE_WIDTH, x, y);
                    if(getWorld()->isValidPos(x/16, y/16) && getX()%16==0 && getY()%16==0)
                        setWalkDirection(left);
                    else return;
                    break;
                default:
                    return;
            }
        }
        
        else{
            int x, y;
            int dir = getWalkDirection();
            getPositionInThisDirection(dir, SPRITE_WIDTH, x, y);
            if(!getWorld()->isValidPos(x/SPRITE_WIDTH, y/SPRITE_HEIGHT) && getX()%16==0 && getY()%16==0){
                    autoChangeDir();
            }
        }
    }
    walk();
    hasWaitedFor = 0;
    ticksToMove--;
    if(ticksToMove==0)
        setWaitingToRoll(true);
}


int Player::oppositeDir(int dir){
    int opp;
    switch(dir){
        case right:
            opp = left;
            break;
        case left:
            opp = right;
            break;
        case up:
            opp = down;
            break;
        case down:
            opp = up;
            break;
        default:
            opp = -1;
            break;
    }
    return opp;
}






















void Player::swapPlayers(Player *p1, Player *p2){
    int p1X = p1->getX();
    int p1Y = p1->getY();
    int p2X = p2->getX();
    int p2Y = p2->getY();
    p1->moveTo(p2X, p2Y);
    p2->moveTo(p1X, p1Y);
    
    int p1Ticks = p1->getTicksToMove();
    int p2Ticks = p2->getTicksToMove();
    p1->setTicksToMove(p2Ticks);
    p2->setTicksToMove(p1Ticks);
    
    int p1Walk = p1->getWalkDirection();
    int p2Walk = p2->getWalkDirection();
    p1->setWalkDirection(p2Walk);
    p2->setWalkDirection(p1Walk);
    
    bool p1State = p1->isWaitingToRoll();
    bool p2State = p2->isWaitingToRoll();
    p1->setWaitingToRoll(p2State);
    p2->setWaitingToRoll(p1State);
    getWorld()->playSound(SOUND_PLAYER_TELEPORT);
}



////////////////////////////////////////////////////////////////////////////////////////////////////
//..........BLUECOINSQUARE FUNCTIONS..........

//Check this.
void ActivatingObject::doSomething(){
    if(!is_alive()){
        return;
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
//..........GAMEOBJECT FUNCTIONS..........

bool GameObject::is_alive() const{
    return alive;
}

void GameObject::setDead(){
    alive = false;
}
///
//bool GameObject::is_a_square() const{
//    return false; //PLACEHOLDER
//}

//bool GameObject::can_be_hit_by_vortex() const{
//    return false; //PLACEHOLDER
//}


StudentWorld* GameObject::getWorld(){
    return myWorldBro;
}


////////////////////////////////////////////////////////////////////////////////////////////////////
//..........ACTIVATEONPLAYER FUNCTIONS..........

bool ActivateOnPlayer::hasPlayerLanded(Player* Player){
    int xPlayer, yPlayer;
    
    xPlayer = Player->getX();
    yPlayer = Player->getY();
    
    bool PlayerIsWaiting = Player->isWaitingToRoll();
    
    int myX = this->getX();
    int myY =this->getY();
    
    if(myX == xPlayer && myY == yPlayer && PlayerIsWaiting){
        if(Player->getHasWaitedFor() == 0){
            return true;
        }
    }
    return false;
}


////////////////////////////////////////////////////////////////////////////////////////////////////
//..........SQUARE FUNCTIONS..........


void CoinSquare::doSomething(){

    ActivatingObject::doSomething();  //Return if dead

    Player* peach = getWorld()->returnPeach();
    Player* yoshi = getWorld()->returnYoshi();


    if(hasPlayerLanded(peach)){
        if(givesCoins){
            peach->increaseCoins(3);
                getWorld()->playSound(SOUND_GIVE_COIN);
        }
        else{
            peach->decreaseCoins(3);
                getWorld()->playSound(SOUND_TAKE_COIN);
        }
    }

    if(hasPlayerLanded(yoshi)){
        if(givesCoins){
            yoshi->increaseCoins(3);
            getWorld()->playSound(SOUND_GIVE_COIN);
        }
        else{
            yoshi->decreaseCoins(3);
                getWorld()->playSound(SOUND_TAKE_COIN);
        }
    }
}


void StarSquare::doEverything(Player* player){
    int playerCoins = player->getCoins();
    bool playerOverlaps = (player->getX()==this->getX() && player->getY()==this->getY());
    
    if(hasPlayerLanded(player) || (playerOverlaps && player->getInactivated() && !player->isWaitingToRoll())){
        player->setInactivated(false);
        if(playerCoins<20) return;
        player->decreaseCoins(20);
        player->increaseStars();
            getWorld()->playSound(SOUND_GIVE_STAR);
    }
    if(!playerOverlaps){
        player->setInactivated(true);
    }
    
}


void StarSquare::doSomething(){
    Player* peach = getWorld()->returnPeach();
    Player* yoshi = getWorld()->returnYoshi();
    doEverything(peach);
    doEverything(yoshi);
}



//////////////////
void DirectionalSquare::doSomething(){
    ;
}


void BankSquare::doEverything(Player *player){
    
    if(hasPlayerLanded(player)){
//        std::cerr<<"Landed";
        int balance = getWorld()->returnBankBalance();
        player->increaseCoins(balance);
        getWorld()->setBankBalance(0);
            getWorld()->playSound(SOUND_WITHDRAW_BANK);
    }
    else{
        bool playerOverlaps = (player->getX()==this->getX() && player->getY()==this->getY());

        if(playerOverlaps && player->getInactivated() && !player->isWaitingToRoll()){
            player->setInactivated(false);
            int coins = player->decreaseCoins(5);
            getWorld()->setBankBalance(getWorld()->returnBankBalance()+coins);
                getWorld()->playSound(SOUND_DEPOSIT_BANK);
//            std::cerr<<"Overlap B:" << getWorld()->returnBankBalance()<<" P:" << player->getCoins() <<" ";
        }
        
        if(!playerOverlaps){
            player-> setInactivated(true);
        }
    }
}


void BankSquare::doSomething(){
    Player* peach = getWorld()->returnPeach();
    Player* yoshi = getWorld()->returnYoshi();
    doEverything(peach);
    doEverything(yoshi);
}


void EventSquare::giveVortex(Player* player){
    player->setVortex(true);
        getWorld()->playSound(SOUND_GIVE_VORTEX);
}

void EventSquare::teleport(Player *player){
    int x,y;
    getWorld()->randomSqr(x, y);
    player->moveTo(x, y);
//    int randDir = randInt(1, 4)*90;
    player->setWalkDirection(player->getWalkDirection());
        getWorld()->playSound(SOUND_PLAYER_TELEPORT);
}

void EventSquare::doEverything(Player* player){
    if(hasPlayerLanded(player)){
        int action = randInt(1, 3);
        switch (action) {
            case 1:
                teleport(player);
                break;
            case 2:
                giveVortex(player);
                break;
            case 3:
                player->swapPlayers(getWorld()->returnYoshi(), getWorld()->returnPeach());
                break;
        }
    }
}

void EventSquare::doSomething(){
    Player* peach = getWorld()->returnPeach();
    Player* yoshi = getWorld()->returnYoshi();
    doEverything(peach);
    doEverything(yoshi);
}

void DroppingSquare::doEverything(Player *player){
    if(hasPlayerLanded(player)){
        int rand = randInt(1, 2);
        if(rand == 1){
            player->decreaseCoins(10);
        }
        else{
            player->decreaseStars();
        }
        getWorld()->playSound(SOUND_DROPPING_SQUARE_ACTIVATE);
    }
}

void DroppingSquare::doSomething(){
    Player* peach = getWorld()->returnPeach();
    Player* yoshi = getWorld()->returnYoshi();
    doEverything(peach);
    doEverything(yoshi);
}



////////////////////////////////////////////////////////////////////////////////////////////////////
//..........ENEMY FUNCTIONS..........


void Enemy::setWalkDirection(int dir){
    walkingDirection = dir;
    if(getWalkDirection()==left)
        setDirection(left);
    else
        setDirection(right);
}



void Enemy::autoChangeDir(){
    int dir = getWalkDirection();
    if(dir == right){
        int xnew, ynew;
        getPositionInThisDirection(up, SPRITE_HEIGHT, xnew, ynew);
        if(getWorld()->isValidPos(xnew/16,ynew/16) && getX()%16==0 && getY()%16==0)
            setWalkDirection(up);
        else setWalkDirection(down);
    }
    else if (dir == left){
        int xnew, ynew;
        getPositionInThisDirection(up, SPRITE_HEIGHT, xnew, ynew);
        if(getWorld()->isValidPos(xnew/16,ynew/16) && getX()%16==0 && getY()%16==0)
            setWalkDirection(up);
        else setWalkDirection(down);
    }
    else if (dir == up){
        int xnew, ynew;
        getPositionInThisDirection(left, SPRITE_WIDTH, xnew, ynew);
        if(getWorld()->isValidPos(xnew/16,ynew/16) && getX()%16==0 && getY()%16==0)
            setWalkDirection(left);
        else setWalkDirection(right);
    }
    else if(dir == down){
        int xnew, ynew;
        getPositionInThisDirection(left, SPRITE_WIDTH, xnew, ynew);
        if(getWorld()->isValidPos(xnew/16,ynew/16) && getX()%16==0 && getY()%16==0)
            setWalkDirection(left);
        else setWalkDirection(right);
    }
}



int Enemy::oppositeDir(int dir){
    int opp;
    switch(dir){
        case right:
            opp = left;
            break;
        case left:
            opp = right;
            break;
        case up:
            opp = down;
            break;
        case down:
            opp = up;
            break;
        default:
            opp = -1;
            break;
    }
    return opp;
}

void Enemy::walk(){
    int dir = getWalkDirection();
    switch(dir){
        case left:
            moveTo(getX()-2, getY());
            break;
        case right:
            moveTo(getX()+2, getY());
            break;
        case up:
            moveTo(getX(), getY()+2);
            break;
        case down:
            moveTo(getX(), getY()-2);
            break;
        default:
            break;
    }
}


int Enemy::pickRandomValidDir(){
    int dirs[4] = {up, right, left, down};
    std::vector<int> validDirs;
    for(int i = 0; i<4; i++){
        int newX, newY;
        getPositionInThisDirection(dirs[i], SPRITE_WIDTH, newX, newY);
        if(getWorld()->isValidPos(newX/16, newY/16) && getX()%16==0 && getY()%16==0){
            validDirs.push_back(dirs[i]);
        }
    }
    int size = validDirs.size();
    int rand = randInt(0, size-1);
    return validDirs[rand];
}

bool Enemy::onSqr(){
    int x = getX();
    int y = getY();
    if(getWorld()->isValidPos(x/16, y/16) && getX()%16==0 && getY()%16==0){
        return true;
    }
    return false;
}

bool Enemy::isAtFork(){
    if(getX()%16!=0 || getY()%16!=0) return false;
    int count = 0;
    int dir[4] = {right, left, up, down};
    for(int i = 0; i<4; i++){
        int newX, newY;
        getPositionInThisDirection(dir[i], 16, newX, newY);
        if(getWorld()->isValidPos(newX/SPRITE_WIDTH, newY/SPRITE_HEIGHT)){
            count++;
        }
    }
    return count>=3;
}


bool Enemy::isAtTurningPoint(){
    if(getX()%16!=0 || getY()%16!=0) return false;
    int x, y;
    getPositionInThisDirection(getWalkDirection(), 16, x, y);
    if(getWorld()->isValidPos(x/SPRITE_WIDTH, y/SPRITE_HEIGHT)){
        return false;
    }
    int count = 0;
    int dir[4] = {right, left, up, down};
    for(int i = 0; i<4; i++){
        int newX, newY;
        getPositionInThisDirection(dir[i], 16, newX, newY);
        if(getWorld()->isValidPos(newX/SPRITE_WIDTH, newY/SPRITE_HEIGHT)){
            count++;
        }
    }
    return count==2;
}



void Enemy::doEverything(){
    Player* peach = getWorld()->returnPeach();
    Player* yoshi = getWorld()->returnYoshi();
    if(getIsPaused()){
        if(hasPlayerLanded(peach) || hasEnemyLanded(peach)){
            doPausedStateActivity(peach);
        }
        
        if(hasPlayerLanded(yoshi) || hasEnemyLanded(yoshi)){
            doPausedStateActivity(yoshi);
        }
        decreasePause();
        if(getTicksToPause()==0){
            int rand = randInt(1, getMaxTicks());
            setSqrsToMove(rand);
            setTicksToMove(rand*8);
//            GET LEGAL TO WALK DIRECTION. SLEEPING NOW.
            int dir = pickRandomValidDir();
            setWalkDirection(dir);
            setIsPause(false);
        }
    }
    if(!getIsPaused()){
        if(onSqr()&&isAtFork()){
            int dir = pickRandomValidDir();
            setWalkDirection(dir);
        }
        else if(onSqr()&&isAtTurningPoint()){
            autoChangeDir();
            }
        walk();
        setTicksToMove(getTicksToMove()-1);
        if(getTicksToMove()==0){
            setIsPause(true);
            setTicksToPause(180);
            mayPerformAction();
        }
    }
}


void Enemy::doSomething(){
    doEverything();
}

void Bowser::doPausedStateActivity(Player* player){
    int rand= randInt(0, 1);
    if(rand==0){
        player->resetCoins();
        player->resetStars();
        getWorld()->playSound(SOUND_BOWSER_ACTIVATE);
    }
}

void Bowser::mayPerformAction(){
    int rand = randInt(0, 3);
    if(rand ==0){
        getWorld()->depositDroppingSquare(getX(), getY());
        getWorld()->playSound(SOUND_DROPPING_SQUARE_CREATED);
    }
}

void Boo::doPausedStateActivity(Player *player){
    Player* peach = getWorld()->returnPeach();
    Player* yoshi = getWorld()->returnYoshi();
    int rand = randInt(0, 1);
    if(rand==0){
        getWorld()->starSwap(peach, yoshi);
        getWorld()->playSound(SOUND_BOO_ACTIVATE);
    }
    else{
        getWorld()->coinSwap(peach, yoshi);
        getWorld()->playSound(SOUND_BOO_ACTIVATE);
    }
}

void Boo::mayPerformAction(){
    ;
}


bool Enemy::hasEnemyLanded(Player* Player){
    int xPlayer, yPlayer;
    
    xPlayer = Player->getX();
    yPlayer = Player->getY();
    
    bool EnemyIsWaiting = getIsPaused();
    
    int xEnemy = this->getX();
    int yEnemy =this->getY();
    
    if(xPlayer == xEnemy && yPlayer == yEnemy && EnemyIsWaiting){
        if(getTicksToPause() == 180){
            return true;
        }
    }
    return false;
}




void Vortex::doSomething(){
    if(!is_alive()) return;
    move();
    if(getX()<=0 || getX()>=VIEW_WIDTH || getY()<=0 || getY()>=VIEW_HEIGHT){
        this->setDead();
    }
    GameObject* enemy = getWorld()->overlapsWithVortex(this);
    if(enemy!= nullptr){
        enemy->hit_by_vortex();
        this->setDead();
        getWorld()->playSound(SOUND_HIT_BY_VORTEX);
    }
}

void Vortex::move(){
    int dir = getMovingDirection();
    switch(dir){
        case left:
            moveTo(getX()-2, getY());
            break;
        case right:
            moveTo(getX()+2, getY());
            break;
        case up:
            moveTo(getX(), getY()+2);
            break;
        case down:
            moveTo(getX(), getY()-2);
            break;
        default:
            break;
    }
}


void Enemy::hit_by_vortex(){
    int x,y;
    getWorld()->randomSqr(x, y);
    this->moveTo(x, y);
    this->setWalkDirection(right);
    setIsPause(true);
    setTicksToPause(180);
}
