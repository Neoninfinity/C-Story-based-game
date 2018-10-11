#include <random>
#include <iostream>
#include <time.h>
#include <map>
#include <vector>
#include <string>
#include <sstream>
#include <chrono>
#include <thread>

#define RESET   "\033[0m"
#define BLACK   "\033[30m"      /* Black */
#define RED     "\033[31m"      /* Red */
#define GREEN   "\033[32m"      /* Green */
#define YELLOW  "\033[33m"      /* Yellow */
#define BLUE    "\033[34m"      /* Blue */
#define MAGENTA "\033[35m"      /* Magenta */
#define CYAN    "\033[36m"      /* Cyan */
#define WHITE   "\033[37m"      /* White */
#define BOLDRED     "\033[1m\033[31m"      /* Bold Red */


#include "Classes_45.cpp" 
#include "Enemy_Selection.cpp"
Player myPlayer;
Player b;
Inventory inven;//temp

int *pointerExample;


using namespace std;

/* decides if the player or enemy can dodge takes the enemy speed and returns a boolean */

bool isDodge(int playerSpeed, int enemySpeed)
{
    //cout << enemySpeed << " " << playerSpeed;
	srand(time(0));
	if (playerSpeed >= enemySpeed)
	{
		float chance = (playerSpeed - enemySpeed);
		if (rand() % 100 < 50) {
			return true;
		}
		else {
			return false;
		}

	}
    else{
        return false;
    }
}


/* did allow the player to choose a move before the functionality was depreciated*/
string choosMove(map<string, float> Moves,int choice)
{
	int x = 1;
    pointerExample = &x; 
	string moveUsed;
	for (auto const& y : Moves)
	{
		moveUsed = y.first;
		if (x == choice)
		{
			break;
		}
		x++;
	}
	return moveUsed;
}

bool checkDead(int p,int e) 
{
	if (p <= 0)
	{
		return true;
	}
	else if (e <= 0)
	{
		return true;
	}
	else;
	{
		return false;
	}
}

int random_bool()
{
        srand(time(0));
		if (rand() % 100 < 10) {
			return 1;
		}
		else {
			return 0;
		}
}

int canFlee(int playerSpeed,int enemySpeed)
{
    if (playerSpeed < enemySpeed)
    {
        
        return 0;
    }
    else
    {
        int val = random_bool();
        return val;
    }
}

//Maths i cannot do https://stackoverflow.com/questions/5289613/generate-random-float-between-two-floats
float RandomFloat(float a, float b) {
    float random = ((float) rand()) / (float) RAND_MAX;
    float diff = b - a;
    float r = random * diff;
    return a + r;
}


int damageDone(float playStren, float playDef, int enemDef, float attackMultiplier) 
{
    int totalDamage = (playDef+attackMultiplier)*playStren - enemDef*4 ;
    if (random_bool() == 1){
        cout << BOLDRED << "Critical attack! ";
        totalDamage = totalDamage * 2;
    }
    totalDamage = totalDamage * RandomFloat(0.01,0.05);
    cout << RESET;
	return totalDamage;
}


int menu(Player myPlayer,Enemy Baddie)
{
    
    int input;
    vector<int> invenResponse;
	cout << RED << R"(
		+----------------------------+
		|                            |
		|  1.Fight      3.Placeholder|
		|                            |
		|                            |
		|                            |
		|  2.Items      4.Flee       |
		|                            |
		+----------------------------+ )" << '\n';
        try{
            cout << RESET << "enter you choice: ";
            cin >> input;
        }
        catch(...)
        {
            cout << "Please enter a valid input: " <<endl;
        }
    
    
    switch (input)
    {
        case 1:
            break;
        case 2:
            
            inven.showInvent(myPlayer.get_inv_id());
            inven.use();
            //invenResponse = player.use(); Call the inventory and store the returned value
            if (invenResponse.size() == 0)
            {
                break;
            }
            else 
            {

                break;
            }
        case 3:
            break;
            //idk
        case 4:
            {
            int flee = canFlee(myPlayer.get_agility(),Baddie.get_agility());
            cout << flee;
            if (flee == 1)
            {
                cout << "You managed to flee the encounter!" << endl;
                return 1;
            }
            else{cout << "You couldn't get away!" << endl;}
            return 0;
            }
        default:
            break;
            
    }
  return 0;
}


void saveStats(int newXP)
{
    sqlite::sqlite db("GameDatabase.sqlite");
    
    auto cur = db.get_statement();
    cur->set_sql("SELECT XP "
				"FROM Saves ");
	cur->prepare();      
	cur->step();
    int oldXP;
    oldXP = cur->get_int(0);
            
    cur = db.get_statement();
    cur->set_sql("UPDATE Saves SET XP = ? ");
   
    cur->prepare();
    cur->bind(1,oldXP+newXP);
    cur->step();
}


void sleep(int x)
{
    this_thread::sleep_for(std::chrono::milliseconds(x));
}

int battleStart(Player myPlayer,Enemy Baddie)
{
    
	int playHealth = myPlayer.get_health();
	int enemyHealth = Baddie.get_health();
    cout << "Battle start: " << myPlayer.get_name() << RED <<" VS " << Baddie.get_name() << endl; // playerNames
    
    cout << RED <<Baddie.get_name() << " has " << RESET << enemyHealth << " HP " << GREEN <<myPlayer.get_name() << " (you)" << " has " << RESET << playHealth << "HP" <<endl; // initDamage
	while (playHealth > 0 && enemyHealth > 0) 
	{
        sleep(2000);
        if ((menu(myPlayer,Baddie))==1)
            {
                return(0);
            }
        system("clear");
        cout<< WHITE;
		if (isDodge(myPlayer.get_agility(), Baddie.get_agility()))
		{
			cout << MAGENTA<<"The enemy has dodged the attack" << RESET <<endl;
            sleep(1000);
		}
		else 
		{
			int damage = damageDone(myPlayer.get_strength(), myPlayer.get_defence(), Baddie.get_defence(),myPlayer.get_attack()); //strength playDef enemDef pattac
            cout << GREEN << "You did " << damage << " damage!"<< endl;
            sleep(1000);
            cout << RESET;
            //health,attack,defence,agility,luck,strength,exp
			enemyHealth = enemyHealth - damage;
		}
		if (checkDead(playHealth, enemyHealth))
		{
			cout << endl << Baddie.get_name() << " fainted" << endl;
            cout << GREEN << "you gained 150 XP" << RESET <<endl;
            saveStats(150);
            sleep(1000);
			return 0;
		}
        
		//Enemys turn
		if (isDodge(Baddie.get_agility(), myPlayer.get_agility() == true))
		{
			cout << MAGENTA <<"You dodged the attack!!" << RESET <<endl;
            sleep(1000);
		}
		else 
		{
			int damage = damageDone(Baddie.get_strength(), Baddie.get_defence(), myPlayer.get_defence(), Baddie.get_attack());
            cout << "You took " << damage << " damage!"<< endl;
			playHealth = playHealth - damage;
            sleep(1000);
		}
        
        if (checkDead(playHealth, enemyHealth))
		{
			cout << endl << myPlayer.get_name() << " fainted" << endl;
            cout << GREEN << "you gained 50 XP" << RESET<<endl;
            saveStats(50);
            sleep(1000);
			return 0;
		}
        cout << Baddie.get_name() << " has " << enemyHealth << " HP " << myPlayer.get_name() << " (you)" << " has " << playHealth << "HP" <<endl; // playerNames
        
	}
    return 0;
}

int battle()
{
    // Baddie shit
  string baddieName = enemySelection(1);               //Get the name
  vector <int> vecStats = enemyStats(baddieName); //Get the stats
  class_create(vecStats,baddieName);             //Create the Enemy as an object
  myPlayer.set_skill("Health",120);
  myPlayer.set_name("Joshua");
  myPlayer.set_skill("Defence",20);
  myPlayer.set_skill("Attack",20);
  myPlayer.set_skill("Strength",15);
  myPlayer.set_skill("Agility",50);
     
  
  battleStart(myPlayer,Baddie);
    
  return 1;
    
}
