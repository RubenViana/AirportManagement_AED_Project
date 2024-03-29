#include <iostream>
#include "Tests/Company.h"
#include <istream>
#include <string>
#include "Tests/Date.h"
#include <ctime>

using namespace std;


int main(){


    ifstream planes, passengers, airports;
    planes.open("PLANES.txt"); // Just need the name of the file to open!
    passengers.open("PASSENGERS.txt");
    airports.open("AIRPORT.txt");
    if (!planes.is_open() || !passengers.is_open() || !airports.is_open()) {
        cout << "Files not Found\n";
        return 1;
    }

    Company comp(planes, passengers, airports);

    planes.close();
    passengers.close();
    airports.close();

    comp.mainMenu();     // start the program

    ofstream planesR ("PLANES.txt");
    ofstream passengersR ("PASSENGERS.txt");
    ofstream airportsR ("AIRPORT.txt");

    comp.record(planesR, passengersR, airportsR);      // load the changes commit in the .txt file

    planesR.close();
    passengersR.close();
    airportsR.close();



    return 0;
}