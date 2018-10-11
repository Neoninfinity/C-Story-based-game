#include <string>
#include <iostream>
#include <array>
#include <vector>
#include "libsqlite.hpp"

class Character {


public:
	//all characters methods
	void take_damage(int dmg) {
		Health -= dmg;
	}

	void attack() {
		/*
		basic idea of character attack
		*/
	}
    // Return stats 
    
	std::string get_name(){
		return Name;
	}
    int get_health(){
        return Health;
    }
	
     int get_attack(){
        return Attack;
    }
	
     int get_defence(){
        return Defence;
    }
	
     int get_agility(){
        return Agility;
    }
	
     int get_luck(){
        return Luck;
    }
	
     int get_strength(){
        return Strength;
    }
	
     int get_xp(){
        return XP;
    }
    
    int get_intelligence(){
        return Intelligence;
    }
     
        
        
	//int get_health() { return Health; }
	void set_name(std::string chara_name) {
		Name = chara_name;
	}

	void set_skill(std::string skill_name, int points) {
		if (skill_name == "Health") {
			Health = points;
		}
		if (skill_name == "Attack") {
			Attack = points;
		}
		if (skill_name == "Defence") {
			Defence = points;
		}
		if (skill_name == "Strength") {
			Strength = points;
		}
		if (skill_name == "Agility") {
			Agility = points;
		}
		if (skill_name == "Luck") {
			Luck = points;
		}
		if (skill_name == "Intelligence") {
			Intelligence = points;
		}
	}

	void set_XP(int charac_xp) {
		XP = charac_xp;
	}
	
	void set_ID(std::string);
	
	void set_inventID(int ID){
		InventID = ID;
	}
    int get_inv_id(){
        return InventID;
    }
	
	
	

protected:
	int Health,Attack, Defence, Strength, Agility, Luck, Intelligence, Level, XP,InventID;
	std::string Name;

};





class Player : public Character {

	std::string char_class;
	

public:
	
	//methods enemies can do that the player can't do 
	//void drop_item(std::string item) {

	//}

	//void pickup_item(int InventID, int ItemSlot, int ItemID){

	//}

	void level_up() {
		Level += 1;
	}

	void set_class(std::string class_name) {
		char_class = class_name;
	}

	void inc_skill(std::string skill_name, int points) {
		if (skill_name == "Health") {
			Health += 10*points;
		}
		if (skill_name == "Attack") {
			Attack += points;
		}
		if (skill_name == "Defence") {
			Defence += points;
		}
		if (skill_name == "Strength") {
			Strength += points;
		}
		if (skill_name == "Agility") {
			Agility += points;
		}
		if (skill_name == "Luck") {
			Luck += points;
		}
		if (skill_name == "Intelligence") {
			Intelligence += points;
		}
	}

	void inc_XP(int xp_gain) {
		if (XP += xp_gain >= 100) {
			XP -= 100 + xp_gain;
			level_up();
		}
		else {
			XP += xp_gain;
		}
	}
	void set_level(int level_enter){
		Level = level_enter;
	}
	
	int get_max_saveID(){
		
		sqlite::sqlite db("GameDatabase.sqlite");
		
		auto cur = db.get_statement();
		cur->set_sql("SELECT Max(SaveID) FROM Saves");
		cur->prepare();
		cur->step();
		
		int max_saveID = cur->get_int(0);
		return max_saveID;
		
	}
	
	void save_game(int saveID){
		sqlite::sqlite db("GameDatabase.sqlite");
		auto cur = db.get_statement();
		
		cur->set_sql("INSERT INTO Saves"
					 "VALUES (?,?,?,?,?,?,?,?,?,?,?,?)");
		cur->prepare();
		
		cur->bind(1,saveID); cur->bind(2,Name); cur->bind(3,char_class);
		cur->bind(4,Health); cur->bind(5,Attack); cur->bind(6,Defence); 
		cur->bind(7,Agility); cur->bind(8, Luck); cur->bind(9,Intelligence);
		cur->bind(10,InventID); cur->bind(11,Level); cur->bind(12,XP);
		
		cur->step();
	}
	
	std::string load_char_name(int save_ID){
		
		sqlite::sqlite db("GameDatabase.sqlite");
		auto cur = db.get_statement();
		
		cur->set_sql("SELECT Name FROM Saves WHERE SaveID = ?");
		
		cur->prepare();
		
		cur->bind(1,save_ID);
		cur->step();
		
		std::string name = cur->get_text(0);
		return name;
		
		
	}
	
	std::array<int,6> load_player_stats(int save_ID){
		std::array<int,6> stats;
		sqlite::sqlite db("GameDatabase.sqlite");
		auto cur = db.get_statement();
		
		cur->set_sql("SELECT Health,Attack,Defence,Agility,Luck,Intelligence FROM Saves WHERE Saves"
					"WHERE SaveID = ?");
		cur->prepare();
		cur->bind(1,save_ID);
		
		cur->step();
		for(int i = 0; i < 6;i++){
			int val = cur->get_int(i);
			stats[i] = val;
		}
		return stats;
	}
	
	int load_level(int save_ID){
		sqlite::sqlite db("GameDatabase.sqlite");
		auto cur = db.get_statement();
		
		cur->set_sql("SELECT Level FROM Saves WHERE SaveID = ?");
		cur->prepare();
		cur->bind(1,save_ID);
		cur->step();
		
		int db_level = cur->get_int(0);
		return db_level;
	}
	
	int load_XP(int save_ID){
		sqlite::sqlite db("GameDatabase.sqlite");
		auto cur = db.get_statement();
		
		cur->set_sql("SELECT XP FROM Saves WHERE SaveID = ?");
		cur->prepare();
		cur->bind(1,save_ID);
		cur->step();
		
		int db_XP = cur->get_int(0);
		return db_XP;
	}
	
	
	int load_inventory_ID(int save_ID){
		sqlite::sqlite db("GameDatabase.sqlite");
		auto cur = db.get_statement();
		
		cur->set_sql("SELECT InventoryID FROM Saves WHERE SaveID = ?");
		cur->prepare();
		cur->bind(1,save_ID);
		cur->step();
		
		int inv_id = cur->get_int(0);
		return inv_id;
	}
	
	

};
class Inventory : public Character {
	public:
	
    std::vector <int> use() {
    /* Asks the user for input, returns the item stats for the inputted integer
     * in a vector*/
		int item;
		std::cout << "What item slot would you like to use?" << std::endl;
			
		while ( !( std::cin >> item ) ) {  //checks that the input is an integer
		  std::cin.clear();
		  std::cin.ignore( 256, '\n' );
		  std::cout << "Please enter an integer " << std::endl;
		}
		while (item <=0 || item >=9){
		  std::cout << "That isn't an item slot, please chose a correct item slot" << std::endl;
		  std::cin >> item;
		}
      
		sqlite::sqlite db("GameDatabase.sqlite");
		auto cur = db.get_statement();
		cur->set_sql("select Item_ID "
				"from PlayerInv "
				"where ItemSlotNum = ?"); // finds the ID of item that they want to use
		cur->prepare();      
		cur->bind(1, item);
			
		cur->step();
		int itemID;
		itemID = cur->get_int(0);// Gets the item id
      
      
		try{
        
			
        
			auto cur2 = db.get_statement();
        
			cur2->set_sql("select * "
						"from Items "
						"where Item_ID = ?"); //Searches for the stats in the table
        
			cur2->prepare();
			cur2->bind(1, itemID);
			cur2->step();
        
			std::vector <int> stats = {}; //Loops through the stats and places them in a vector
			for(int k = 2; k < 8; k++){
             
			   int stat = cur2->get_int(k);
			   stats.emplace_back(stat);
              
            
           
			}
        return stats;
				
				
		}catch(sqlite::exception e){
			std::vector <int> stats = {0,0,0};
        return stats;
      }
    }
  void showInvent(int);
  
  void createInv (int inventID){
    /* Creates an inventory in the database with a given integer value*/
    sqlite::sqlite db("test.sqlite");
    
    auto cur = db.get_statement();
    cur->set_sql("INSERT INTO PlayerInv (Invent_ID, ItemSlotNum, Item_ID) VALUES (?,?,0)");
    for(int i = 1; i<9; i++){
      
      cur->prepare();
      cur->bind(1,inventID);
      cur->bind(2, i);
      cur->step();
    }
  }
  void delInv (int inventID){
    /* Deletes an inventory from the database with a given integer value*/
      
    sqlite::sqlite db("test.sqlite");
    auto cur = db.get_statement();
    cur->set_sql("DELETE FROM PlayerInv WHERE Invent_ID = ?");
    cur->prepare();
    cur->bind(1, inventID);
    cur->step();
  }
  void changeItem (int inventID, int itemSlot, int itemID ){
    /* Changes an item in the PlayerInv table with a given inventory ID, item slot number
     * and item ID. All values are integers*/
    sqlite::sqlite db("test.sqlite");
    
    sqlite::statement_ptr cur = db.get_statement();
       
    cur->set_sql("UPDATE PlayerInv SET Item_ID = ? WHERE Invent_ID = ? and ItemSlotNum = ?");
    
    cur->prepare();
      
    cur->bind(1, itemID);
    cur->bind(2, inventID);
    cur->bind(3, itemSlot);
    cur->step();
      
    std::cout << "Change successful!"<< std::endl;
      
    
}
	int get_max_invID(){
		sqlite::sqlite db("GameDatabase.sqlite");
		auto cur3 = db.get_statement();
		
		
		cur3->set_sql("SELECT MAX(Invent_ID) AS LargestInvent FROM PlayerInv");
		
		cur3->prepare();
		cur3->step();
		
		int max_invent_ID = cur3->get_int(0);
		return max_invent_ID;
		
	}
};

void Inventory::showInvent(int inventID) {
    /* Takes an inventory ID and displays it in a pleasing form*/
	std::array <std::string, 8> Headers = { "ItemSlot", "Item", "Health", "Attack", "Defence", "Agility", "Luck", "Intel" };
	
    std::vector <int> Items = {};
	sqlite::sqlite db("GameDatabase.sqlite");
  
    auto cur2 = db.get_statement();
  
    cur2->set_sql("select * "
                  "from PlayerInv "
                  "join Items "
                  "on PlayerInv.Item_ID = Items.Item_ID "
                  "where Invent_ID = ?"); //Joins the two tables together to make for easier data retrieval
    cur2->prepare();
    cur2->bind(1,inventID);
    


	for (std::string header : Headers) { //Loops through the headers array for displaying
		int space = 9 - header.size();

		std::cout << header;
		for (int i = 0; i < space; i++) {
			std::cout << " ";
		}
	}
	std::cout << std::endl << std::endl;
  
  
	
	
  
  while (cur2->step()){
    
    std::cout << cur2->get_int(1); //Displays the item slot number
    for (int j = 0; j < 8; j++) 
      {
        std::cout << " ";
      }  
    std::string itemName = cur2->get_text(4); //Displays the name
    int space = 9 - itemName.size(); //Gets the number of spaces needed to create equal gaps between values
    std::cout << itemName;
    for (int i = 0; i < space; i++)
    {
      std::cout << " ";
    }
    for (int i = 5; i < 11; i++){ //Loops to display item stats

      std::cout << cur2->get_int(i);
        
      for (int j = 0; j < 8; j++) 
      {
        std::cout << " ";
      }
    }
	std::cout << std::endl;
    
  }
  
  
  
  std::cout << std::endl;
  return; 
}






class Enemy : public Character {

public:
	//methods enemies can do that the player can't do 
};