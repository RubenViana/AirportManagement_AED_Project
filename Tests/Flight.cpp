//
// Created by ruben on 24/11/2021.
//

#include "Flight.h"

Flight::Flight(unsigned int number,unsigned int time,Date date,string ori,string des){
    flightNumber = number;
    flightTime = time;
    departureDate = date;
    origin = ori;
    destination = des;
    occupiedPlaces = 0;
}

unsigned int Flight::getAvailablePlaces(string &plate) const{
    return  0;//Plane.getCapacity() - occupiedPlaces;
}

unsigned int Flight::getFlightNumber() const{
    return flightNumber;
}

unsigned int Flight::getDuration() const{
    return flightTime;
}

unsigned int Flight::getOccupiedPlaces() const{
    return occupiedPlaces;
}

string Flight::getDestination() const{
    return destination;
}

string Flight::getOrigin() const{
    return origin;
}

Date Flight::getDepartureDate() const{
    return departureDate;
}

void Flight::setFlightNumber( unsigned int flightNumber )           //passar por referencia !!
{
    this->flightNumber = flightNumber;
}

void Flight::setDestination(string destination)
{
    this->destination = destination;
}

void Flight::setOrigin(string origin)
{
    this->origin = origin;
}

void Flight::setFlightTime(unsigned int flightTime)
{
    this->flightTime = flightTime;
}

void Flight::setDate(Date departureDate)
{
    this->departureDate = departureDate;
}

void Flight::setOccupiedPlaces() {
    occupiedPlaces++;
}


