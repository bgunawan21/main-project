#include <cmath>
#include <sstream>
#include <stdio.h>      
#include <stdlib.h>
#include "Move.h"

using namespace std;

Move::Move(string commandString) : Move() {
    if (commandString != "") {
        stringstream ss(commandString);
        char c;
        ss >> c >> elevatorId >> c;
        
        if (c == 'f') {
            ss >> targetFloor;
        }
        else if (c == 'p') {
            isPickup = true;
        }
        else if (c == 'Q' || c == 'q') {
            isQuit = true;
        }
        else if (c == 'S' || c == 's') {
            isSave = true;
        }
    }
    else {
        isPass = true;
    }
}

bool Move::isValidMove(Elevator elevators[NUM_ELEVATORS]) const {
    if (isPass || isSave || isQuit) {
        return true;
    }
    if ((0 <= elevatorId) && (elevatorId < NUM_ELEVATORS)
        && !elevators[elevatorId].isServicing()) {
            if (!isPickup && !(((0 <= targetFloor) && (targetFloor < NUM_FLOORS) &&
                                targetFloor != elevators[elevatorId].getCurrentFloor()))) {
                return false;
            }
        return true;
    }
    return false;
}

void Move::setPeopleToPickup(const string& pickupList, const int currentFloor,
                             const Floor& pickupFloor) {
    numPeopleToPickup = 0;
    totalSatisfaction = 0;
    
    int largestFloorDist = 0;
    int endFloor = 0;
    
    for (int i = 0; i < pickupList.size(); i++) {
        char itemToConvert = ' ';
        itemToConvert = pickupList.at(i);
        int number = int(itemToConvert) - 48;
        peopleToPickup[i] = number;
        
        Person p1 = pickupFloor.getPersonByIndex(number);
        int angerLevel = MAX_ANGER - p1.getAngerLevel();
        totalSatisfaction = totalSatisfaction + angerLevel;
        numPeopleToPickup++;
        
        int subtractedDifference = p1.getTargetFloor() - currentFloor;
        
        if (subtractedDifference < 0) {
            subtractedDifference = subtractedDifference * -1;
        }
        if (subtractedDifference > largestFloorDist) {
            endFloor = p1.getTargetFloor();
            largestFloorDist = subtractedDifference;
        }
    }
    targetFloor = endFloor;
}


//////////////////////////////////////////////////////
////// DO NOT MODIFY ANY CODE BENEATH THIS LINE //////
//////////////////////////////////////////////////////

Move::Move() {
    elevatorId = -1;
    targetFloor = -1;
    numPeopleToPickup = 0;
    totalSatisfaction = 0;
	isPass = false;
    isPickup = false;
    isSave = false;
    isQuit = false;
}

bool Move::isPickupMove() const {
    return isPickup;
}

bool Move::isPassMove() const {
	return isPass;
}

bool Move::isSaveMove() const {
	return isSave;
}

bool Move::isQuitMove() const {
	return isQuit;
}

int Move::getElevatorId() const {
    return elevatorId;
}

int Move::getTargetFloor() const {
    return targetFloor;
}

int Move::getNumPeopleToPickup() const {
    return numPeopleToPickup;
}

int Move::getTotalSatisfaction() const {
    return totalSatisfaction;
}

void Move::setTargetFloor(int inTargetFloor) {
    targetFloor = inTargetFloor;
}

void Move::copyListOfPeopleToPickup(int newList[MAX_PEOPLE_PER_FLOOR]) const {
    for (int i = 0; i < numPeopleToPickup; ++i) {
        newList[i] = peopleToPickup[i];
    }
}
