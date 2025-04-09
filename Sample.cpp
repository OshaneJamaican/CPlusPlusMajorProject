#include <iostream>
#include <string>
#include <cstdlib> 
#include <ctime>
#include <vector>

using namespace std;

// Forward declaration of classes 
class Door;  
class Room;  
class Player;

// Door class representing a door that leads to a room and may be an exit
class Door {
public:
    Room* leadsTo;  // Room this door leads to
    bool isExit;     // Exit door is true or false

    Door() : leadsTo(nullptr), isExit(false) {}

    void setDoor(Room* room, bool exit = false) {
        leadsTo = room;
        isExit = exit;
    }

// Returns the room this door leads to
    Room* openDoor() {
        return leadsTo;
    }
};

class Room {
protected:
    string name;
    vector<Door> doors;

public:
    Room(string roomName) : name(roomName) {}

// Adding a door to this room
    void addDoor(Door door) {
        doors.push_back(door);
    }

// Show room's name
    virtual void displayTheRoom() {
        cout << "Your now in the " << name << "." << endl;
    }

    vector<Door>& getDoors() {
        return doors;
    }

    virtual bool isExit() {
        return false;
    }
};

// Show the name of the room being displayed
class Bedroom : public Room {
public:
    Bedroom() : Room("Bedroom") {}
    void displayTheRoom() override {
        cout << "You're now in the Bedroom." << endl;
    }
};

class Bathroom : public Room {
public:
    Bathroom() : Room("Bathroom") {}

    void displayTheRoom() override {
        cout << "You're now in the Bathroom." << endl;
    }
};

class LivingRoom : public Room {
public:
    LivingRoom() : Room("Living Room") {}

    void displayTheRoom() override {
        cout << "You're now in the Living Room." << endl;
    }
};

class Kitchen : public Room {
public:
    Kitchen() : Room("Kitchen") {}

    void displayTheRoom() override {
        cout << "You're now in the Kitchen." << endl;
    }
};

class Basement : public Room {
public:
    Basement() : Room("Basement") {}

    void displayTheRoom() override {
        cout << "You're now in the Basement." << endl;
    }
};

class Player {
private:
    string name; 
    int lives;
    Room* currentRoom;

public:
    Player(string playerName) : name(playerName), lives(3), currentRoom(nullptr) {}

    void setCurrentRoom(Room* room) {
        currentRoom = room;
    }

    Room* getCurrentRoom() {
        return currentRoom;
    }

    void loseLife() {
        lives--;
        cout << "You lost a life! Lives remaining: " << lives << endl;
    }

    int getLives() {
        return lives;
    }

    string getName() {
        return name;
    }
};

// Setup the game rooms and connections
void setupGame(vector<Room*>& rooms, vector<Player>& players) {
    // Creating rooms
    Bedroom* bedroom = new Bedroom();
    Bathroom* bathroom = new Bathroom();
    LivingRoom* livingRoom = new LivingRoom();
    Kitchen* kitchen = new Kitchen();
    Basement* basement = new Basement();

    rooms.push_back(bedroom);
    rooms.push_back(bathroom);
    rooms.push_back(livingRoom);
    rooms.push_back(kitchen);
    rooms.push_back(basement);

    // Create doors and connecting rooms
    Door bedroomDoor1, bedroomDoor2, bathroomDoor1, bathroomDoor2, livingRoomDoor1, livingRoomDoor2, kitchenDoor1, kitchenDoor2, basementDoor1, basementDoor2;

    bedroomDoor1.setDoor(bathroom, false);
    bedroomDoor2.setDoor(livingRoom, false);

    bathroomDoor1.setDoor(bedroom, false);
    bathroomDoor2.setDoor(kitchen, false);

    livingRoomDoor1.setDoor(bedroom, false);
    livingRoomDoor2.setDoor(basement, false);

    kitchenDoor1.setDoor(bathroom, false);
    kitchenDoor2.setDoor(livingRoom, false);

    basementDoor1.setDoor(livingRoom, false);
    basementDoor2.setDoor(kitchen, false);

    // Setting the doors for rooms
    bedroom->addDoor(bedroomDoor1);
    bedroom->addDoor(bedroomDoor2);
    bathroom->addDoor(bathroomDoor1);
    bathroom->addDoor(bathroomDoor2);
    livingRoom->addDoor(livingRoomDoor1);
    livingRoom->addDoor(livingRoomDoor2);
    kitchen->addDoor(kitchenDoor1);
    kitchen->addDoor(kitchenDoor2);
    basement->addDoor(basementDoor1);
    basement->addDoor(basementDoor2);
}

// Choose random exit door
void setExitDoor(vector<Room*>& rooms) {
    int randomRoomIndex = rand() % rooms.size();
    Room* exitRoom = rooms[randomRoomIndex];

    // Function to ensure bathroom doesn't lead outside
    while(dynamic_cast<Bathroom*>(exitRoom)) {
        randomRoomIndex = rand() % rooms.size();
        exitRoom = rooms[randomRoomIndex];
    }

    Door* exitDoor = new Door();
    exitDoor->setDoor(exitRoom, true);
    exitRoom->getDoors()[rand() % 2] = *exitDoor;

// Main part of the game
void startGame(vector<Room*>& rooms, Player& player) {
    int doorsOpened = 0;
    while(player.getLives() > 0 && doorsOpened < 7) {
        player.getCurrentRoom()->displayTheRoom();

        cout << "You have " << player.getLives() << " lives left. You have opened " << doorsOpened << " doors." << endl;
        cout << "Choose a door (1 or 2): ";
        
        int choice;
        cin >> choice;

        if(choice == 1 || choice == 2) {
            Door& door = player.getCurrentRoom()->getDoors()[choice - 1];
            player.setCurrentRoom(door.openDoor());
            doorsOpened++;

            if(door.isExit) {
                cout << "Awesome! " << player.getName() << ", you've exited the house, and won the game!" << endl;
                break;
            }
        } else {
            cout << "Wrong choice! You just lost 1 life!" << endl;
            player.loseLife();
            doorsOpened++;
        }

        if(player.getLives() <= 0) {
            cout << "Game Over! You've died!" << endl;
        }
    }
}

int main() {
    srand(time(0)); 
    
    // Create the rooms and the player vector
    vector<Room*> rooms;
    vector<Player> players;

    string playerName;
    cout << "What is your name? : ";
    getline(cin, playerName);

    Player player(playerName);
    players.push_back(player);

    setupGame(rooms, players);
    setExitDoor(rooms);
    
    player.setCurrentRoom(rooms[0]); 

    // Start the game
    startGame(rooms, player);

    return 0;
}
