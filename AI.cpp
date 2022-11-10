#include "AI.h"
#include <cassert>

// This file is used only in the Reach, not the Core.
// You do not need to make any changes to this file for the Core
// Newest version as of: Fri, Dec 10 at 3:04 PM

string getAIMoveString(const BuildingState& buildingState) {
    string str = "";
    
    // Elevator Data
    int elevatorFloor[NUM_ELEVATORS] = {};
    int servicingCount = 0;
    
    for (int i = 0; i < NUM_ELEVATORS; i++) {
        elevatorFloor[i] = buildingState.elevators[i].currentFloor;
    }
    
    // Pickup Move
    for (int i = 0; i < NUM_ELEVATORS; i++) {
        // Checks for Pass Move
        if (buildingState.elevators[i].isServicing) {
            servicingCount++;
        }
        // Checks for people on elevator's floor
        if (buildingState.floors[elevatorFloor[i]].numPeople > 0) {
            for (int j = 0; j < NUM_ELEVATORS; j++) {
                if (buildingState.elevators[j].isServicing == false) {
                    if (buildingState.floors[buildingState.elevators[j].currentFloor].numPeople > 0) {
                        str = 'e' + to_string(buildingState.elevators[j].elevatorId) + 'p';
                        return str;
                    }
                }
            }
        }
    }
    // Handles Pass Move
    if (servicingCount == 3) {
        // If the servicing count is 3, then no free elevators, so pass move.
        str = "";
        return str;
    }
    // Go to Floor with Most People
    int targetFloor = buildingState.floors[0].floorNum;
    int numPeopleOnFloor = buildingState.floors[0].numPeople;

    for (int i = 0; i < NUM_FLOORS; i++) {
        // Greater amount of people
        if (buildingState.floors[i].numPeople > numPeopleOnFloor) {
            targetFloor = buildingState.floors[i].floorNum;
            numPeopleOnFloor = buildingState.floors[i].numPeople;
            for(int j = 0; j < NUM_ELEVATORS; j++) {
                if (!buildingState.elevators[j].isServicing) {
                    str = 'e' + to_string(buildingState.elevators[j].elevatorId) + 'f' + to_string(targetFloor);
                }
            }
        }
        // Angerrrrrrrr
        if (buildingState.floors[i].numPeople == numPeopleOnFloor) {
            if (buildingState.floors[i].people[i].angerLevel <= buildingState.floors[targetFloor].people[i].angerLevel) {
                targetFloor = buildingState.floors[i].floorNum;
                numPeopleOnFloor = buildingState.floors[i].numPeople;
                for(int j = 0; j < NUM_ELEVATORS; j++) {
                    if (!buildingState.elevators[j].isServicing) {
                        str = 'e' + to_string(buildingState.elevators[j].elevatorId) + 'f' + to_string(targetFloor);
                    }
                }
            }
        }
    }
    // Check for no people at all in building
    int numPeopleInBuilding = 0;
    for (int i = 0; i < NUM_FLOORS; i++) {
        numPeopleInBuilding += buildingState.floors[i].numPeople;
    }
    if (numPeopleInBuilding == 0) {
        for (int i = 0; i < NUM_ELEVATORS; i++) {
            if (buildingState.elevators[i].currentFloor != 0 && !buildingState.elevators[i].isServicing) {
                str = 'e' + to_string(buildingState.elevators[i].elevatorId) + 'f' + to_string(0);
            }
            else {
                str = "";
            }
        }
    }
    // Checks if Current Floor of Elevator has People to Pickup
    for (int i = 0; i < NUM_ELEVATORS; i++) {
        for (int j = 0; j < NUM_FLOORS; j++) {
            if (!buildingState.elevators[i].isServicing) {
                if (buildingState.floors[buildingState.elevators[i].currentFloor].numPeople > 0) {
                    str = 'e' + to_string(buildingState.elevators[i].elevatorId) + 'f' + to_string(buildingState.floors[j].floorNum);
                }
            }
        }
    }
    return str;
}

string getAIPickupList(const Move& move, const BuildingState& buildingState, const Floor& floorToPickup) {
    int upRequests = 0;
    int downRequests = 0;
    int upAngerTotal = 0;
    int downAngerTotal = 0;
    
    int numPeople = floorToPickup.getNumPeople();
    int currentFloor = floorToPickup.getPersonByIndex(0).getCurrentFloor();
    
    string pickUpList = "";
    
    for (int i = 0; i < numPeople; i++) {
        int targetFloor = floorToPickup.getPersonByIndex(i).getTargetFloor();
        if (targetFloor - currentFloor > 0) {
            upRequests++;
            upAngerTotal += floorToPickup.getPersonByIndex(i).getAngerLevel();
        }
        else {
            downRequests++;
            downAngerTotal += floorToPickup.getPersonByIndex(i).getAngerLevel();

        }
    }
    if (upRequests - downRequests < -3 && downAngerTotal > upAngerTotal) {
        for (int i = 0; i < numPeople; i++) {
            int targetFloor = floorToPickup.getPersonByIndex(i).getTargetFloor();
            
            if (targetFloor - currentFloor < 0) {
                pickUpList = pickUpList + to_string(i);
            }
        }
    }
    else if (upRequests > downRequests) {
        for (int i = 0; i < numPeople; i++) {
            int targetFloor = floorToPickup.getPersonByIndex(i).getTargetFloor();
            
            if (targetFloor - currentFloor >= 0) {
                pickUpList = pickUpList + to_string(i);
            }
        }
    }
    else if (upRequests < downRequests) {
        for (int i = 0; i < numPeople; i++) {
            int targetFloor = floorToPickup.getPersonByIndex(i).getTargetFloor();
            
            if (targetFloor - currentFloor < 0) {
                pickUpList = pickUpList + to_string(i);
            }
        }
    }
    else if (upRequests == downRequests) {
        if (upAngerTotal > downAngerTotal) {
            for (int i = 0; i < numPeople; i++) {
                int targetFloor = floorToPickup.getPersonByIndex(i).getTargetFloor();
                
                if (targetFloor - currentFloor >= 0) {
                    pickUpList = pickUpList + to_string(i);
                }
            }
        }
        else {
            for (int i = 0; i < numPeople; i++) {
                int targetFloor = floorToPickup.getPersonByIndex(i).getTargetFloor();
                
                if (targetFloor - currentFloor <= 0) {
                    pickUpList = pickUpList + to_string(i);
                }
            }
        }
    }
    return pickUpList;
}
