#include "Floor.h"

#include <algorithm>

using namespace std;

int Floor::tick(int currentTime) {
    int numPeopleRemoved = 0;
    int index = 0;
    int indicesToRemove[MAX_PEOPLE_PER_FLOOR] = {};

    for (int i = 0; i < numPeople; i++) {
        if (people[i].tick(currentTime)) {
            numPeopleRemoved++;
            indicesToRemove[index] = i;
            index++;
        }
    }
    removePeople(indicesToRemove, index);
    return numPeopleRemoved;
}

void Floor::addPerson(Person p, int request) {
    if (numPeople < MAX_PEOPLE_PER_FLOOR) {
        people[numPeople] = p;
        numPeople += 1;
        
        if (request > 0) {
            hasUpRequest = true;
        }
        else if (request < 0) {
            hasDownRequest = true;
        }
    }
}

void Floor::removePeople(int indicesToRemove[MAX_PEOPLE_PER_FLOOR],
                         int numPeopleToRemove) {
    Person tempArr[MAX_PEOPLE_PER_FLOOR] = {};
    int counter = 0;
    bool isMatch = false;
    
    for (int p = 0; p < MAX_PEOPLE_PER_FLOOR; p++) {
        isMatch = false;
        for (int r = 0; r < numPeopleToRemove; r++) {
            if (p == indicesToRemove[r]) {
                isMatch = true;
            }
        }
        if (!isMatch) {
            tempArr[counter] = people[p];
            counter++;
        }
    }

    for (int i = 0; i < MAX_PEOPLE_PER_FLOOR; i++) {
        people[i] = tempArr[i];
    }
    
    numPeople -= numPeopleToRemove;
    resetRequests();
}

void Floor::resetRequests() {
    hasDownRequest = false;
    hasUpRequest = false;

    for (int i = 0; i < numPeople; i++) {
        int request = people[i].getTargetFloor() - people[i].getCurrentFloor();

        if (request > 0) {
            hasUpRequest = true;
        }
        else if (request < 0) {
            hasDownRequest = true;
        }
    }
}

//////////////////////////////////////////////////////
////// DO NOT MODIFY ANY CODE BENEATH THIS LINE //////
//////////////////////////////////////////////////////

Floor::Floor() {
    hasDownRequest = false;
    hasUpRequest = false;
    numPeople = 0;
}

void Floor::prettyPrintFloorLine1(ostream& outs) const {
	string up = "U";
	outs << (hasUpRequest ? up : " ") << " ";
	for (int i = 0; i < numPeople; ++i){
		outs << people[i].getAngerLevel();
		outs << ((people[i].getAngerLevel() < MAX_ANGER) ? "   " : "  ");
	}
	outs << endl;
}

void Floor::prettyPrintFloorLine2(ostream& outs) const {
	string down = "D";
	outs << (hasDownRequest ? down : " ") << " ";
	for (int i = 0; i < numPeople; ++i) {
		outs << "o   ";
	}
	outs << endl;
}

void Floor::printFloorPickupMenu(ostream& outs) const {
	cout << endl;
	outs << "Select People to Load by Index" << endl;
	outs << "All people must be going in same direction!";
	/*  O   O
	// -|- -|-
	//  |   |
	// / \ / \  */
	outs << endl << "              ";
	for (int i = 0; i < numPeople; ++i) {
		outs << " O   ";
	}
	outs << endl << "              ";
	for (int i = 0; i < numPeople; ++i) {
		outs << "-|-  ";
	}
	outs << endl << "              ";
	for (int i = 0; i < numPeople; ++i) {
		outs << " |   ";
	}
	outs << endl << "              ";
	for (int i = 0; i < numPeople; ++i) {
		outs << "/ \\  ";
	}
	outs << endl << "INDEX:        ";
	for (int i = 0; i < numPeople; ++i) {
		outs << " " << i << "   ";
	}
	outs << endl << "ANGER:        ";
	for (int i = 0; i < numPeople; ++i) {
		outs << " " << people[i].getAngerLevel() << "   ";
	}
	outs << endl << "TARGET FLOOR: ";
	for (int i = 0; i < numPeople; ++i) {
		outs << " " << people[i].getTargetFloor() << "   ";
	}
}

void Floor::setHasUpRequest(bool hasRequest) {
    hasUpRequest = hasRequest;
}

bool Floor::getHasUpRequest() const {
	return hasUpRequest;
}

void Floor::setHasDownRequest(bool hasRequest) {
    hasDownRequest = hasRequest;
}

bool Floor::getHasDownRequest() const {
	return hasDownRequest;
}

int Floor::getNumPeople() const {
    return numPeople;
}

Person Floor::getPersonByIndex(int index) const {
    return people[index];
}
