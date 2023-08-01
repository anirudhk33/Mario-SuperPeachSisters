#include "StudentWorld.h"
#include "GameConstants.h"
#include <string>
#include <sstream>  // defines the type std::ostringstream
#include <vector>
using namespace std;

GameWorld* createStudentWorld(string assetPath)
{
	return new StudentWorld(assetPath);
}

// Students:  Add code to this file, StudentWorld.h, Actor.h, and Actor.cpp

StudentWorld::StudentWorld(string assetPath)
: GameWorld(assetPath), cleaned(false), bankBalance(0)
{}

int StudentWorld::init()
{
    string bdNum = to_string(getBoardNumber());
    string board_file = assetPath() + "board0" + bdNum + ".txt";
    Board::LoadResult result = bd.loadBoard(board_file);
    if (result == Board::load_fail_file_not_found){
        cerr << "Could not find board01.txt data file\n";
        return GWSTATUS_BOARD_ERROR;
    }
    else if (result == Board::load_fail_bad_format){
        cerr << "Your board was improperly formatted\n";
        return GWSTATUS_BOARD_ERROR;
    }
    else if (result == Board::load_success){
        cerr << "Successfully loaded board\n";
        for(int i = 0; i!=SPRITE_WIDTH; i++){
            for(int j = 0; j!=SPRITE_HEIGHT; j++){
                int obj = bd.getContentsOf(i, j);
                switch(obj){
                    case Board::player:
                        peach = new Player(this, IID_PEACH, i, j, 1);
                        yoshi = new Player(this, IID_YOSHI, i, j, 2);
                    case Board::blue_coin_square:
                        actors.push_back(new CoinSquare(this, IID_BLUE_COIN_SQUARE, i, j, true));
                        break;
                    case Board::red_coin_square:
                        actors.push_back(new CoinSquare(this, IID_RED_COIN_SQUARE, i, j, false));
                        break;
                    case Board::star_square:
                        actors.push_back(new StarSquare(this, IID_STAR_SQUARE, i, j));
                        break;
                    case Board::up_dir_square:
                        actors.push_back(new DirectionalSquare(this, IID_DIR_SQUARE, i, j, 90));
                        break;
                    case Board::down_dir_square:
                        actors.push_back(new DirectionalSquare(this, IID_DIR_SQUARE, i, j, 270));
                        break;
                    case Board::left_dir_square:
                        actors.push_back(new DirectionalSquare(this, IID_DIR_SQUARE, i, j, 180));
                        break;
                    case Board::right_dir_square:
                        actors.push_back(new DirectionalSquare(this, IID_DIR_SQUARE, i, j, 0));
                        break;
                    case Board::bank_square:
                        actors.push_back(new BankSquare(this, IID_BANK_SQUARE, i, j));
                        break;
                    case Board::event_square:
                        actors.push_back(new EventSquare(this, IID_EVENT_SQUARE, i, j));
                        break;
                    case Board::bowser:
                        actors.push_back(new Bowser(this, IID_BOWSER, i, j));
                        actors.push_back(new CoinSquare(this, IID_BLUE_COIN_SQUARE, i, j, true));
                        break;
                    case Board::boo:
                        actors.push_back(new Boo(this, IID_BOO, i, j));
                        actors.push_back(new CoinSquare(this, IID_BLUE_COIN_SQUARE, i, j, true));
                }
            }
        }
    }
    startCountdownTimer(99);
    return GWSTATUS_CONTINUE_GAME;
}

int StudentWorld::move()
{
    
    if(timeRemaining()==0){
        playSound(SOUND_GAME_FINISHED);
        int status;
        int winnerCoins, winnerStars;
        int peachCoins = peach->getCoins();
        int peachStars = peach->getStars();
        int yoshiCoins = yoshi->getCoins();
        int yoshiStars = yoshi->getStars();
        if(peachStars>yoshiStars){
            winnerCoins = peachCoins;
            winnerStars = peachStars;
            status = GWSTATUS_PEACH_WON;
        }
        else if(yoshiStars>peachStars){
            winnerCoins = yoshiCoins;
            winnerStars = yoshiStars;
            status = GWSTATUS_YOSHI_WON;
        }
        else{
            if(peachCoins>yoshiCoins){
                winnerCoins = peachCoins;
                winnerStars = peachStars;
                status = GWSTATUS_PEACH_WON;
            }
            else if(yoshiCoins>peachCoins){
                winnerCoins = yoshiCoins;
                winnerStars = yoshiStars;
                status = GWSTATUS_YOSHI_WON;
            }
            else{
                int rand = randInt(0, 1);
                if(rand ==0){
                    winnerCoins = yoshiCoins;
                    winnerStars = yoshiStars;
                    status = GWSTATUS_YOSHI_WON;
                }
                else{
                    winnerCoins = peachCoins;
                    winnerStars = peachStars;
                    status = GWSTATUS_PEACH_WON;
                }
            }
        }
        
        setFinalScore(winnerStars, winnerCoins);
        return status;
    }
    
    peach->doSomething();
    yoshi->doSomething();
    for(int i = 0; i!=actors.size(); i++){
        actors[i]->doSomething();
    }
    
    vector<GameObject*>::iterator it = actors.begin();
    while (it != actors.end())
        {
            if ((*it)->is_alive() == false)
            {
                delete (*it);
                it = actors.erase(it);
            }
            else
            {
                it++;
            }
        }

    
    ostringstream oss;
    oss << "P1 Roll: ";
    int p1Roll = peach->getTicksToMove()/8;
    oss<<p1Roll<<" Stars: ";
    int p1Stars = peach->getStars();
    oss <<p1Stars<<" $$: ";
    int p1Coins = peach->getCoins();
    oss << p1Coins << " ";
    if(peach->hasVortex()){
        oss<< "VOR ";
    }
    oss << "| Time: ";
    int time = timeRemaining();
    oss << time << " | Bank: ";
    int balance = returnBankBalance();
    oss << balance << " | P2 Roll: ";
    
    int p2Roll = yoshi->getTicksToMove()/8;
    oss<<p2Roll<<" Stars: ";
    int p2Stars = yoshi->getStars();
    oss <<p2Stars<<" $$: ";
    int p2Coins = yoshi->getCoins();
    oss << p2Coins << " ";
    if(yoshi->hasVortex()){
        oss<< "VOR ";
    }
    
    string stream = oss.str();
    setGameStatText(stream);
    
	return GWSTATUS_CONTINUE_GAME;
}


void StudentWorld::cleanUp()
{
    delete peach;
    delete yoshi;
    for(int i = 0; i!=actors.size(); i++){
        delete actors[i];
    }
    cleaned = true;
}

StudentWorld::~StudentWorld(){
    if(!cleaned)
        cleanUp();
}

Board StudentWorld::returnBoard(){
    return bd;
}

Player* StudentWorld::returnPeach(){
    return peach;
}

Player* StudentWorld::returnYoshi(){
    return yoshi;
}

bool StudentWorld::isValidPos(int x, int y){
    if(returnBoard().getContentsOf(x, y) == Board::empty){
        return false;
    }
    return true;
}

int StudentWorld::isPlayerOnDirectionalSquare(Player *obj){
    int objX = obj->getX();
    int objY = obj->getY();

    for(int i = 0; i<actors.size(); i++){
        if(actors[i]->getForcingDirIfExists() != -1){
            int dsqX = actors[i]->getX();
            int dsqY = actors[i]->getY();
            if(dsqX==objX && dsqY==objY){
                return actors[i]->getForcingDirIfExists();
            }
        }
    }
    return -1;
}

int StudentWorld::returnBankBalance(){
    return bankBalance;
}

void StudentWorld::setBankBalance(int num){
    bankBalance = num;
}

void StudentWorld::randomSqr(int &x, int &y){
    int rand = randInt(0, actors.size()-1);
    while(!actors[rand]->is_a_square()){
        rand = randInt(0, actors.size()-1);
    }
    x = actors[rand]->getX();
    y = actors[rand]->getY();
}



bool StudentWorld::isEnemyOnThisSqr(int x, int y){
    for(int i = 0; i<actors.size(); i++){
        if(actors[i]->getCanBeHitByVortex() && actors[i]->getX()==x && actors[i]->getY()==y)
            return true;
    }
    return false;
}

void StudentWorld::depositDroppingSquare(int x, int y){
    for(int i = 0; i<actors.size(); i++){
        if(actors[i]->is_a_square() && actors[i]->getX()==x && actors[i]->getY()==y){
            actors[i]->setDead();
            GameObject* temp = actors[i];
            actors[i] = new DroppingSquare(this, IID_DROPPING_SQUARE, x/SPRITE_WIDTH, y/SPRITE_HEIGHT);
            delete temp;
        }
    }
}

void StudentWorld::coinSwap(Player *p1, Player *p2){
    int p1Coins = p1->getCoins();
    int p2Coins = p2->getCoins();
    p1->setCoinsTo(p2Coins);
    p2->setCoinsTo(p1Coins);
}

void StudentWorld::starSwap(Player *p1, Player *p2){
    int p1Stars = p1->getStars();
    int p2Stars = p2->getStars();
    p1->setStarsTo(p2Stars);
    p2->setStarsTo(p1Stars);
}


bool StudentWorld::doTheyOverlap(GameObject *obj1, GameObject *obj2){
    int o1X = obj1->getX();
    int o1Y = obj1->getY();
    int o2X = obj2->getX();
    int o2Y = obj2->getY();

    int o1Left = o1X;
    int o1Right = o1X + SPRITE_WIDTH-1;
    int o1Bottom = o1Y;
    int o1Top =  o1Y+SPRITE_HEIGHT-1;

    int o2Left = o2X;
    int o2Right = o2X + SPRITE_WIDTH-1;
    int o2Bottom = o2Y;
    int o2Top =  o2Y+SPRITE_HEIGHT-1;

    bool verticalOverlap = (o1Top>=o2Bottom && o1Bottom<=o2Top) || (o2Top>=o1Bottom && o2Bottom<=o1Top);
    bool horizontalOverlap = (o1Right>=o2Left && o1Left<=o2Right) || (o2Right>=o1Left && o2Left<=o1Right);

    return (verticalOverlap && horizontalOverlap);
}

GameObject* StudentWorld::overlapsWithVortex(GameObject* Vortex){
    for(int i = 0; i<actors.size(); i++){
        if(actors[i]->can_be_hit_by_vortex()){
            if(doTheyOverlap(Vortex, actors[i])){
                return actors[i];
            }
        }
    }
    return nullptr;
}


void StudentWorld::addVortex(int x, int y, int dir){
    GameObject* ptr =new Vortex(this, IID_VORTEX, x, y, dir);
    actors.push_back(ptr);
}
