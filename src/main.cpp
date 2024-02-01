// g++ -I/home/noname/.local/include -fPIC main.cpp -L/home/noname/.local/lib -lsimlib -static -o main

#include <simlib.h>
#include <iostream>
#include <iomanip>
#include <fstream>

using namespace std;

// Define global variables for simulation parameters
const int TOTAL_PEOPLE = 1000; // Total number of people arriving at the station
static bool isLastStation = false; // Whether the train is at the last station
static bool isInStation = false; // Whether the train is in the station

ofstream logFile;

class Person : public Process {
public:
    int personId;
    bool isEmployee;
    bool hasLeft = false;
    bool foodPrepared = false;
    bool StationShop, StationToilet, TrainToilet, TrainFood = false;
    float seatFinding;

    // Constructor
    Person(int id, bool employee) : personId(id), isEmployee(employee) {}

    // Method to define the behavior of the person
    void Behavior() override {
        if (Random() < 0.01) {
            // 1% chance that the person is an employee working on the station
            logFile << "Employee " << std::setw(2) << personId << " arrives at the station." << std::endl;
        } else {
            // Check the information board
            Wait(Exponential(2)); // Assume 2 minutes to check the board

            // Decide whether to leave due to being late (5% chance)
            if (Random() < 0.05) {
                logFile << "Late! Leaves the station." << std::endl;
                hasLeft = true;
                return;
            }

            // Decide on an activity (buying, toilet, waiting)
            double activityDecision = Random();
            if (activityDecision < 0.20) {
                logFile << "Person " << std::setw(2) << personId << " is going shopping." << std::endl;
                StationShop = true;
                Wait(Exponential(15)); // Assume 15 minutes for shopping
                if (Random() < 0.10) {
                    logFile << "Person " << std::setw(2) << personId << " is going to the toilet after shopping." << std::endl;
                    StationToilet = true;
                    Wait(Exponential(5)); // Assume 5 minutes for toilet

                    Wait(Exponential(40)); // Assume 40 minutes of waiting
                } else {
                    Wait(Exponential(45)); // Assume 45 minutes of waiting
                }
            } else if (activityDecision < 0.30) {
                logFile << "Person " << std::setw(2) << personId << " is going to the toilet." << std::endl;
                StationToilet = true;
                Wait(Exponential(5)); // Assume 5 minutes for toilet
                if (Random() < 0.20) {
                    logFile << "Person " << std::setw(2) << personId << " is going shopping after toilet." << std::endl;
                    StationShop = true;
                    Wait(Exponential(15)); // Assume 15 minutes for shopping

                    Wait(Exponential(40)); // Assume 40 minutes of waiting
                } else {
                    Wait(Exponential(55)); // Assume 55 minutes of waiting
                }
            } else {
                Wait(Exponential(60)); // Assume 1 hour of waiting
            }
        }

        // Check if a train is present for cleaning and depot
        if (!isLastStation && !hasLeft) {
            // Move to the platform with 2 minutes delay
            Wait(2);

            if (isInStation) {
                logFile << "Person " << std::setw(2) << personId << " boards the train." << std::endl;
            } else {
                if (Random() < 0.01) {
                    // Employee behaviors in the train
                    Wait(Exponential(15)); // Employee preparation time
                    if (Random() < 0.67) {
                        // Serving food
                        logFile << "Employee " << std::setw(2) << personId << " is preparing food." << std::endl;
                        Wait(Exponential(10)); // Food preparation time
                        foodPrepared = true;
                    } else {
                        // Checking tickets
                        logFile << "Employee " << std::setw(2) << personId << " is checking tickets." << std::endl;
                        // Assume checking tickets for every person (except employees) who boarded the train
                        for (int i = 1; i <= TOTAL_PEOPLE; ++i) {
                            if (i == personId && !isEmployee) {
                                Wait(Uniform(10, 120)); // Checking tickets takes 10 seconds to 2 minutes
                            }
                        }
                    }
                } else {
                    // Non-employee behaviors in the train
                    seatFinding = Exponential(5);
                    Wait(seatFinding); // Finding seat

                    // Check if the seat is reserved (5% chance)
                    if (Random() < 0.05) {
                        logFile << "Person " << std::setw(2) << personId << " needs to find another seat. Seat was reserved." << std::endl;
                        Wait(Exponential(2)); // Assume 2 minutes for finding another seat
                        seatFinding += Exponential(2);
                    }

                    if (Random() < 0.3 && !isInStation) {
                        // Go to the toilet
                        logFile << "Person " << std::setw(2) << personId << " is going to the toilet." << std::endl;
                        TrainToilet = true;
                        Wait(Exponential(10)); // Assume 5 minutes for using the toilet
                    }

                    if (Random() < 0.1 && !isInStation && foodPrepared) {
                        // Buying food (10% chance)
                        logFile << "Person " << std::setw(2) << personId << " is buying food." << std::endl;
                        TrainFood = true;
                        Wait(Exponential(5)); // Assume 5 minutes for buying food
                    }

                    if (isInStation) {
                        // Leaving the train
                        hasLeft = true;
                        logFile << "Person " << std::setw(2) << personId << " leaves the train." << std::endl;
                    }
                }
            }
        }
        Out();

        // Free the memory
        delete this;
    }

    void Out() {
        // Print("%d %d %d %d %d\n", personId, StationShop, StationToilet, TrainToilet, TrainFood);
        Print ("%d %f\n", personId, seatFinding);
    }
};

class Train : public Process {
public:
    int trainId;
    double delayTime;
    double transportTime;

    Train(int id) : trainId(id) {}

    void Behavior() override {
        transportTime = Exponential(30); // Exp(30 minutes) transportation time
        isLastStation = false;
        
        // Arrival at the station
        if (Random() < 0.05) {
            isLastStation = true;
            // 5% chance that this is the last station, and the train will stop for cleaning
            logFile << "Train " << std::setw(2) << trainId << " is in the last station. Cleaning process initiated." << std::endl;
            Wait(Exponential(60)); // Exp(1 hour) cleaning time
            logFile << "Train " << std::setw(2) << trainId << " is departing to the depot." << std::endl;

            delete this;
            return;
        } else {
            // Announce the ETA of the train
            logFile << "Announcement: Train arriving soon!" << std::endl;

            // 95% of trains will stop at the station and wait for departure again
            // Train arrives at the station
            logFile << "Train " << std::setw(2) << trainId << " arrives at the station." << std::endl;
            isInStation = true;
    
            // Boarding process
            double boardingTime = Exponential(2); // Exp(2 minutes) boarding time
            Wait(boardingTime); // Wait for boarding to complete
        }

        // Departure
        isInStation = false;
        logFile << "Train " << std::setw(2) << trainId << " departs from the station." << std::endl;
        if (Random() < 0.40) {
            // Train is late (40% chance)
            delayTime = Exponential(40); // Exp(40 minutes) delay
            logFile << "Train " << std::setw(2) << trainId << " is late by " << delayTime << " minutes." << std::endl;
            Wait(delayTime);
            Wait(transportTime);
        } else {
            // Train is on time
            logFile << "Train " << std::setw(2) << trainId << " is on time. Transportation time: " << transportTime << " minutes." << std::endl;
            Wait(transportTime);
        }
        // Out();

        // Repeat the process for the next train
        Train *nextTrain = new Train(trainId + 1);
        nextTrain->Activate();
        delete this;
    }

    // void Out() {
    //     Print("%d %9.4f %9.4f %9.4f\n", trainId, transportTime + delayTime, delayTime, transportTime);
    // }
};


int main() {
    // Open the log file for writing
    logFile.open("log.txt");

    // Check if the log file is successfully opened
    if (!logFile.is_open()) {
        cerr << "Error opening log file." << endl;
        return 1;
    }

    // Set the output file for simulation data
    SetOutput("main.dat");
    // Initialize the simulation
    Init(0, 1000);

    // Simulate the arrival of people at the station
    Print("----- People simulation -----\n");
    // Print("# PersonId   StationShop   StationToilet   TrainToilet   TrainFood\n");
    Print("# PersonId   SeatFindingTime\n");
    for (int i = 1; i <= TOTAL_PEOPLE; ++i) {
        // Decide whether the person is an employee (1% chance)
        bool isEmployee = (Random() < 0.01);

        // Create a person and schedule their arrival
        Person *person = new Person(i, isEmployee);
        person->Activate();
    }

    // Simulate the continuous arrival of trains
    // Print("----- Train transport simulation -----\n");
    // Print("# TrainId   TransportTime   DelayTime   TotalTime\n");
    Train *firstTrain = new Train(1);
    firstTrain->Activate();

    // Run the simulation
    Run();

    // Close the log file
    logFile.close();

    return 0;
}
