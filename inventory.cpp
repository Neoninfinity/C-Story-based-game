// inventory.cpp : Defines the entry point for the console application.
//


#include "libsqlite.hpp"
#include <array>
#include <iostream>
#include <string>

using namespace std;

class Inventory {
	public:
	
    vector <int> use() {
    /* Asks the user for input, returns the item stats for the inputted integer
     * in a vector*/
	int item;
	cout << "What item slot would you like to use?" << endl;
			
    while ( !( cin >> item ) ) {  //checks that the input is an integer
      cin.clear();
      cin.ignore( 256, '\n' );
      cout << "Please enter an integer " << endl;
    }
    while (item <=0 || item >=9){
      cout << "That isn't an item slot, please chose a correct item slot" << endl;
      cin >> item;
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
        
		vector <int> stats = {}; //Loops through the stats and places them in a vector
        for(int k = 2; k < 8; k++){
             
           int stat = cur2->get_int(k);
           stats.emplace_back(stat);
              
            
           
        }
        return stats;
				
				
      }
      

			
      catch(sqlite::exception e){
        vector <int> stats = {0,0,0};
        return stats;
      }
    }
  void showInvent(int);
  
  void createInv (int inventID){
    /* Creates an inventory in the database with a given integer value*/
    sqlite::sqlite db("GameDatabase.sqlite");
    
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
      
    sqlite::sqlite db("GameDatabase.sqlite");
    auto cur = db.get_statement();
    cur->set_sql("DELETE FROM PlayerInv WHERE Invent_ID = ?");
    cur->prepare();
    cur->bind(1, inventID);
    cur->step();
  }
  void addItem (int inventID, int itemSlot, int itemID ){
    /* Changes an item in the PlayerInv table with a given inventory ID, item slot number
     * and item ID. All values are integers*/
    sqlite::sqlite db("GameDatabase.sqlite");
    
    sqlite::statement_ptr cur = db.get_statement();
       
    cur->set_sql("UPDATE PlayerInv SET Item_ID = ? WHERE Invent_ID = ? and ItemSlotNum = ?");
    
    cur->prepare();
      
    cur->bind(1, itemID);
    cur->bind(2, inventID);
    cur->bind(3, itemSlot);
    cur->step();
      
    cout << "Change successful!"<< endl;
      
    
}
};

void Inventory::showInvent(int inventID) {
    /* Takes an inventory ID and displays it in a pleasing form*/
	array <string, 8> Headers = { "ItemSlot", "Item", "Health", "Attack", "Defence", "Agility", "Luck", "Intel" };
	
    vector <int> Items = {};
	sqlite::sqlite db("GameDatabase.sqlite");
  
    auto cur2 = db.get_statement();
  
    cur2->set_sql("select * "
                  "from PlayerInv "
                  "join Items "
                  "on PlayerInv.Item_ID = Items.Item_ID "
                  "where Invent_ID = ?"); //Joins the two tables together to make for easier data retrieval
    cur2->prepare();
    cur2->bind(1,inventID);
    


	for (string header : Headers) { //Loops through the headers array for displaying
		int space = 9 - header.size();

		cout << header;
		for (int i = 0; i < space; i++) {
			cout << " ";
		}
	}
	cout << endl << endl;
  
  
	
	
  
  while (cur2->step()){
    
    cout << cur2->get_int(1); //Displays the item slot number
    for (int j = 0; j < 8; j++) 
      {
        cout << " ";
      }  
    string itemName = cur2->get_text(4); //Displays the name
    int space = 9 - itemName.size(); //Gets the number of spaces needed to create equal gaps between values
    cout << itemName;
    for (int i = 0; i < space; i++)
    {
      cout << " ";
    }
    for (int i = 5; i < 11; i++){ //Loops to display item stats

      cout << cur2->get_int(i);
        
      for (int j = 0; j < 8; j++) 
      {
        cout << " ";
      }
    }
	cout << endl;
    
  }
  
  
  
  cout << endl;
  return; 
}


int main()
{
	
	
  Inventory test;
  test.showInvent(1);
  test.addItem(1,3,0);
  test.showInvent(1);
  //vector <int> stats = test.use();
  //for( auto i:stats ) cout << i << ", "; cout << endl;
  //Inventory newChar;
  //newChar.createInv(2);
  //newChar.delInv(2);
	

    return 0;
};