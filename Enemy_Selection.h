#include <iostream>
#include "libsqlite.hpp" // sqlite3 stuff
#include <string>
#include <vector>
#include <time.h> // to select a different random int each time the program is run
//#include "Classes_45.cpp" //Only use when testing
using namespace std;

Enemy Baddie; //Create global object so all functions have access to it 


//Takes an integer as input
//aquire all names from database that has a specific level and then select one at random
//return the name selected
string fun_Name(int level) 
{
 
  string randName;
  try 
  {
      
  //Query
  string sqliteFile = "GameDatabase.sqlite";
  sqlite::sqlite db(sqliteFile);
  auto cur = db.get_statement();
  cur->set_sql("SELECT name "
               "FROM Enemy_Selection "
               "WHERE lvl = ? ");
  cur->prepare();
  cur->bind(1,level);
    
  // adding the results of the query to a vector to select a name at random and return it
  int i = 1;
  vector <string> VecNames = {} ;
  while (cur->step())
    
        {
        VecNames.emplace_back(cur->get_text(0)); 
        i += 1;
        }
  int randomNum = rand() % VecNames.size(); 
  randName = VecNames[ randomNum ];
  return randName;
      
  }
    
    catch( sqlite::exception e )      // catch all sql issues
    {
        std::cerr << e.what() << std::endl;
        return "0";
    }
  return "0";
}



// takes a name as input and select the stats of the name from the database
// and stores them in a vector
// returns the vector
vector <int> fun_Stats(string name)
{
  
  vector <int> vecStats = {};
  try
  {
      
  //Query
  string sqliteFile = "GameDatabase.sqlite";
  sqlite::sqlite db(sqliteFile);
  auto cur = db.get_statement();
  cur->set_sql("SELECT * "
               "FROM Enemy_Stats "
               "WHERE name = ? ");
  cur->prepare();
  cur->bind(1,name); 
  int position = 1;
  while (cur->step()) // store values in a vector
  {
    while (position<=7)
      
    {
      vecStats.emplace_back(cur->get_int(position));
      position +=1;
    }
    
  }
  return vecStats;
  }
    
  catch( sqlite::exception e )      // catch all sql issues
    {
        std::cerr << e.what() << std::endl; 
    }
    
}



int class_create(vector<int> vecStats, string baddieName)
{
  //assigning stats to variables
  int health = vecStats[0];
  int attack = vecStats[1];
  int defence = vecStats[2];
  int agility = vecStats[3];
  int luck = vecStats[4];
  int strength = vecStats[5];
  int exp = vecStats[6]; 
  
  //Object Creation
  Baddie.set_name(baddieName);
  Baddie.set_skill("Health",health);
  Baddie.set_skill("Attack",attack);
  Baddie.set_skill("Defence",defence);
  Baddie.set_skill("Strength",strength);
  Baddie.set_skill("Agility",agility);
  Baddie.set_skill("Luck",luck);
  Baddie.set_XP(exp);
  
  //cout<<"In function:" << endl;
  //cout << Baddie.get_name()<<endl;
  //cout << Baddie.get_health()<<endl;
  return 0;
}



int __test__()
{
  srand (time(NULL)); //Has to be called once, outside of any loops  
    
  int i =0;
  while (i<3)
  {
  string baddieName = fun_Name(1);               //Get the name
  vector <int> vecStats = fun_Stats(baddieName); //Get the stats
  class_create(vecStats,baddieName);             //Create the Enemy as an object  
    
  cout << "Name:     " <<Baddie.get_name()<<endl;
  cout << "Health:   " <<Baddie.get_health()<<endl;
  cout << "Attack:   " <<Baddie.get_attack()<<endl;
  cout << "Defence:  " <<Baddie.get_defence()<<endl;
  cout << "Agility:  " <<Baddie.get_agility()<<endl;
  cout << "Luck:     " <<Baddie.get_luck()<<endl;
  cout << "Strength: " <<Baddie.get_strength()<<endl;
  cout << "Exp:      " <<Baddie.get_xp()<<endl;
  cout << "          " <<endl;
  i=i+1;   
  }
}
