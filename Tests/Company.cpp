//
// Created by pedro on 01/12
#include "Company.h"
#include <string>
#include <iostream>
#include <iomanip>
#include <sstream>
#include <stack>
#include <queue>
#include <algorithm>
using namespace std;

int BinarySearch(vector<Flight> v, int el){
    int left = 0, right = v.size() - 1;
    while (left <= right)
    {
        int middle = (left + right) / 2;
        if (v[middle].getFlightNumber() < el) {
            left = middle + 1;
        }
        else if (el < v[middle].getFlightNumber()) {
            right = middle -1;
        }
        else{
            return middle;
        } // found
    }
    return 0;
}

void swap(Flight* a, Flight* b)
{
    Flight t = *a;
    *a = *b;
    *b = t;
}

// partition the array using last element as pivot
int partition (vector<Flight> &v, int low, int high)
{
    int pivot = v[high].getFlightNumber();    // pivot
    int i = (low - 1);

    for (int j = low; j <= high - 1 ; j++)
    {
        //if current element is smaller than pivot, increment the low element
        //swap elements at i and j
        if (v[j].getFlightNumber() <= pivot)
        {
            i++;    // increment index of smaller element
            swap(&v[i], &v[j]);
        }
    }
    swap(&v[i + 1], &v[high]);
    return (i + 1);
}

void sort(vector<Flight> &v, int low, int high)
{
    if (low < high)
    {
        //partition the array
        int pivot = partition(v, low, high);

        //sort the sub arrays independently
        sort(v, low , pivot - 1);
        sort(v, pivot + 1, high);
    }
}

void Company::update() {
    for (auto &p : planes) {
        for (auto f = p.getFlights().begin(); f != p.getFlights().end(); f++) {
            if (f->getDepartureDate().daysSince2020() < 0) {
                p.getFlights().remove(*f--);
            }
        }
        for (int i = 0; i < p.getToDoServ().size(); i++){
            if (p.getToDoServ().front().getDate().daysSince2020() < 0){
                p.getDoneServ().push_back(p.getToDoServ().front());
                p.getToDoServ().pop();
            }
        }
    }
}

Company::Company(ifstream &dataPl, ifstream &dataPs, ifstream &dataAir) {
    //read airports !
    int nAirports;
    string nAir;
    getline(dataAir, nAir);
    nAirports = stoi(nAir);
    for (int i = 0; i < nAirports; i++){
        Airport a;
        a.readFile(dataAir);
        airports.push_back(a);
    }

    // read planes !
    int nPlanes;
    string nPl;
    getline(dataPl,nPl);
    nPlanes = stoi(nPl);
    for (int i = 0; i < nPlanes; i++){
        list<Flight> fls;
        vector<Service> doneServ;
        queue<Service> toDoServ;
        string line, typeP, nFl, nDS, nTDS;
        int numberP, capP;
        string sep = " - ";
        getline(dataPl,line);
        stringstream ss(line);
        ss >> numberP >> sep >> typeP >> sep >> capP;
        getline(dataPl,nFl);
        int nFlights = stoi(nFl);
        for (int j = 0; j < nFlights; j++){
            string l, origin, dest, date;
            int numberF, durationF, ocPlaces;
            getline(dataPl,l);
            stringstream ss1(l);
            ss1 >> numberF >> sep >> date >> sep >> durationF >> sep >> origin >> sep >> dest >> sep >> ocPlaces;
            Date dateF(date);
            Flight f (numberF,durationF,dateF,origin,dest, capP, ocPlaces);
            fls.push_back(f);
        }

        getline(dataPl,nDS);
        int nDoneServ = stoi(nDS);
        for (int j = 0; j < nDoneServ; j++){
            string l, typeS, nameS, date;
            getline(dataPl,l);
            stringstream ss1(l);
            ss1 >> typeS >> sep >> date >> sep >> nameS;
            Date dateS(date);
            Service sv(typeS, dateS, nameS);        //contrutor tem de estar public !
            doneServ.push_back(sv);
        }

        getline(dataPl,nTDS);
        int nToDoServ = stoi(nTDS);
        for (int j = 0; j < nToDoServ; j++){
            string l, typeS, nameS, date;
            getline(dataPl,l);
            stringstream ss1(l);
            ss1 >> typeS >> sep >> date >> sep >> nameS;
            Date dateS(date);
            Service sv(typeS, dateS, nameS);
            toDoServ.push(sv);
        }
        Plane p(numberP, typeP, capP , fls, doneServ, toDoServ);
        planes.push_back(p);
    }
    // read passengers !
    string sep = " - ";
    string nP;
    getline (dataPs, nP); // Number of passengers (first line of the file)
    int nPs = stoi(nP);
    for (int i = 0; i < nPs; i++){
        string line;
        getline(dataPs, line); // Next passenger
        stringstream ss(line);
        vector<Ticket> tickets;
        int ssn, nTickets;
        string name;
        ss >> name >> sep >> ssn >> sep >> nTickets;
        for (int j = 0; j < nTickets; j++){
            int nF;
            int pck;
            string line1;
            getline(dataPs, line1);
            stringstream ss1(line1);
            ss1 >> pck >> sep >> nF;
            tickets.push_back(Ticket(pck, nF));
        }
        Passenger p(name, ssn, tickets);
        passengers.push_back(p);
    }

    //call update method !
    update();


}

bool Company::checkPassenger(Passenger &p){
    int ssn;
    cout << "Enter your SSN : "; cin >> ssn;
    for (auto &k : passengers)
        if (k.getSSN() == ssn) {
            p = k;
            return true;
        }
    cout << "You are not registered !\nRegister first\n";
    return false;
}
void Company::userMenu() {
    int userChoice;
    do{
        system("cls");
        cout << "User Menu\n\n";
        cout << "1 - Register\n";
        cout << "2 - Check-in\n";
        cout << "3 - Buy Ticket\n";
        cout << "0 - Return to Menu\n";
        cout << endl << "Option : ";
        cin >> userChoice;
        cout << endl;
        if (cin.fail()) {
            cout << "Invalid Input !" << endl;
            Sleep(400);
            cin.clear();
            cin.ignore(9999, '\n');
            userChoice = -1;
        }
        else {
            Passenger p;
            switch (userChoice) {
                case 1 :
                    cout << "Register Passenger\n\n";
                    addPassenger();
                    Sleep(500);
                    break;
                case 2 :
                    if (checkPassenger(p)) {
                        cout << "Welcome to Check-in\n";
                        checkIn(p);
                    }
                    Sleep(500);
                    break;
                case 3 :
                    if (checkPassenger(p))
                        buyTicket(p);

                    Sleep(500);
                    break;
                case 0 :
                    break;
                default:
                    cout << "Invalid Option! Try Again\n";
                    Sleep(400);
                    break;
            }
        }
    }while (userChoice != 0);
}

void Company::settingsMenu(){
    int setChoice;
    do{
        system("cls");
        cout << "Settings\n\n";
        cout << "1 - Show All Flights\n";
        cout << "2 - Add Flight\n";
        cout << "3 - Remove Flight\n";
        cout << "4 - Show All Passengers\n";
        cout << "5 - Add Passenger\n";
        cout << "6 - Remove Passenger\n";
        cout << "7 - Show All Planes\n";
        cout << "8 - Add Plane\n";
        cout << "9 - Remove Plane\n";
        cout << "10 - Show All Services\n";
        cout << "11 - Add Service\n";
        cout << "12 - Remove Service\n";
        cout << "0 - Return to Menu\n";
        cout << endl << "Option : ";
        cin >> setChoice;
        cout << endl;
        if (cin.fail()) {
            cout << "Invalid Input !" << endl;
            Sleep(400);
            cin.clear();
            cin.ignore(9999, '\n');
            setChoice = -1;
        }
        else {
            string name; int ssn;
            switch (setChoice) {        // falta addFlight, removeFlight, showAllPlanes, ...('shows')
                case 1 :
                    cout << "Flights : \n\n";
                    showAllFlights();
                    Sleep(999);
                    break;
                case 2 :
                    cout << "Adding Flight ...\n\n";
                    addFlight();
                    Sleep(500);
                    break;
                case 3 :
                    cout << "Removing Flight ...\n\n";
                    removeFlight();
                    Sleep(500);
                    break;
                case 4 :
                    cout << "Passengers : \n\n";
                    showAllPassengers();
                    Sleep(999);
                    break;
                case 5 :
                    cout << "Adding Passenger ...\n\n";
                    addPassenger();
                    Sleep(500);
                    break;
                case 6 :
                    cout << "Removing Passenger ...\n\n";
                    removePassenger();
                    Sleep(500);
                    break;
                case 7 :
                    cout << "Planes : \n\n";
                    showAllPlanes();
                    Sleep(999);
                    break;
                case 8 :
                    cout << "Adding Plane ...\n\n";
                    addPlane();
                    Sleep(500);
                    break;
                case 9 :
                    cout << "Removing Passenger ...\n\n";
                    removePlane();
                    Sleep(500);
                    break;
                case 10 :
                    cout << "Services : \n\n";
                    showAllServices();
                    Sleep(999);
                    break;
                case 11 :
                    cout << "Adding Service ...\n\n";
                    addService();
                    Sleep(500);
                    break;
                case 12 :
                    cout << "Removing Service ...\n\n";
                    removeService();
                    Sleep(500);
                    break;
                case 0 :
                    break;
                default:
                    cout << "Invalid Option! Try Again\n";
                    Sleep(400);
                    break;
            }
        }
    } while (setChoice != 0);
}


void Company::mainMenu(){
    int choice;
    do{
        system("cls");
        cout << "Main Menu\n\n";
        cout << "1 - User HelpDesk\n";
        cout << "2 - App Settings\n";
        cout << "0 - Exit App\n";
        cout << endl << "Option : ";
        cin >> choice;
        cout << endl;
        if (cin.fail()) {
            cout << "Invalid Input !" << endl; Sleep(400);
            cin.clear();
            cin.ignore(9999, '\n');
            choice = -1;
        }else {
            switch (choice) {
                case 1 :
                    userMenu();
                    break;
                case 2 :
                    settingsMenu();
                    break;
                case 0 :
                    break;
                default :
                    cout << "Invalid Option! Try Again\n";
                    Sleep(400);
                    break;
            }
        }
    }while (choice != 0);
}


void Company::showAllFlights() {
    cout << "     Flight Number     |     Flight Date     |     Flight Time     |     Origin     |     Destination     |     Avaiable Places" << endl;
    cout << "====================================================================================================================================" << endl;
    vector<Flight> fls;
    for (auto &k : planes)
        for (auto &i : k.getFlights())
            fls.push_back(i);

    sort(fls,0,fls.size() - 1);
    for (auto &k: fls)
        k.show();

}

void Company::showAllPassengers() {
    cout << "     Name     |     SSN     |      Package     |      Flight Number" << endl;
    cout << "======================================================================" << endl;
    for (auto &k : passengers) {
        k.show();
    }
}

void Company::showAllPlanes() {
    cout << "     Plate     |     Type     |     Capacity     " << endl;
    cout << "================================================" << endl;
    for (Plane &p : planes) {
        p.show();
    }
}

void Company::addPassenger() {
    int ssn; string name;
    do {
        cout << "Passenger SSN to add (xxxxxxxxx) : ";
        cin >> ssn;
        cin.ignore();
    }while (to_string(ssn).size() != 9);
    for (auto &k : passengers)
        if (k.getSSN() == ssn){
            cout << "SSN already in use !\n";
            return;
        }
    cout << "Passenger FirstName to add : "; getline(cin,name);
    vector<Ticket> t{};
    passengers.push_back(Passenger(name,ssn, t));
}

void Company::removePassenger() {
    int ssn;
    string name;
    cout << "Passenger SSN to remove : ";
    cin >> ssn;
    for (auto &k : passengers)
        if (k.getSSN() == ssn) {
            vector<Ticket> t;
            passengers.remove(Passenger(name, ssn, t));
            return;
        }
    cout << "Passenger not found !\n";
}


void Company::record(ofstream &dataPl, ofstream &dataPs, ofstream &dataAir) {
    string sep = " - ";

    //recording airports
    dataAir << airports.size() << endl;
    for (auto &a : airports) {
        dataAir << a.getName() << sep << a.getTransportsSize() << endl;
        for(auto &t : a.getTransports())
            dataAir << t.getType() << sep << t.getDistance() << endl;
    }

    //recording passengers
    dataPs << passengers.size() << endl;
    for (auto &p : passengers) {
        dataPs << p.getName() << sep << p.getSSN() << sep << p.getTickets().size() << endl;
        for (auto &t : p.getTickets())
            dataPs << int(t.getPackage()) << sep << t.getFlightNumber() << endl;
    }

    //recording planes
    dataPl << planes.size() << endl;
    for (auto &p : planes){
        dataPl << p.getPlate() << sep << p.getType() << sep << p.getCapacity() << endl;
        dataPl << p.getFlights().size() << endl;
        for (auto &f : p.getFlights())
            dataPl << f.getFlightNumber() << sep << f.getDepartureDate().show() << sep << f.getDuration() << sep << f.getOrigin() << sep << f.getDestination() << sep << f.getOccupiedPlaces() << endl;
        dataPl << p.getDoneServ().size() << endl;
        for (auto &ds : p.getDoneServ())
            dataPl << ds.getType() << sep << ds.getDate().show() << sep << ds.getEmployeeName() << endl;
        dataPl << p.getToDoServ().size() << endl;
        for (int i = 0; i < p.getToDoServ().size(); i++){
            dataPl << p.getToDoServ().front().getType() << sep << p.getToDoServ().front().getDate().show() << sep << p.getToDoServ().front().getEmployeeName() << endl;
            p.getToDoServ().pop();
        }
    }
}

void Company::addPlane() {
    int plate;
    list<Flight> fl{}; vector<Service> ds{}; queue<Service> tds{};
    cout << "Plane plate to add : "; cin >> plate; cout << endl;
    if (find(planes.begin(), planes.end(), Plane(plate, "", 0,  fl, ds, tds)) == planes.end()) {
        int cap;
        string type;
        cout << "Type : "; cin >> type; cout << endl;
        cout << "Capacity : "; cin >> cap; cout << endl;
        planes.push_back(Plane(plate, type, cap, fl, ds, tds));
        return;
    }
    cout << "Plane already exists !\n\n";
}

void Company::removePlane() {
    int plate;
    list<Flight> f{}; vector<Service> ds{}; queue<Service> tds{};
    cout << "Plane plate to remove : ";
    cin >> plate;
    planes.remove(Plane(plate, "", 0, f, ds, tds));
}

void Company::addFlight() {
    list<Flight> fl{}; vector<Service> ds{}; queue<Service> tds{};
    int number, duration, plate;
    string ori, dest, date;
    Date d;
    cout << "Plate number of the plane you want to add a Flight : "; cin >> plate; cout << endl;
    auto it = find(planes.begin(), planes.end(), Plane(plate, "", 0,  fl, ds, tds));
    if (it == planes.end()){
        cout << "Plane not found !\n\n";
        return;
    }
    cout << "Date (dd/mm/yyyy_hh:mm:ss) of the Flight : "; cin >> date; cout << endl;
    d = Date(date);
    for (auto &f: it->getFlights()) {
        if (f.getDepartureDate().daysBetweenDates(d) == 0) {
            cout << "Date not available !\n\n";
            return;
        }
    }
    int cnt;
    cout << "Flight number : "; cin >> number; cout << endl;
    for (auto &i : planes)
        if (find(i.getFlights().begin(), i.getFlights().end(), Flight(number, 0, Date(), "", "", 0, 0)) != i.getFlights().end()) {
            cnt ++;
        }
    if (cnt > 0) { cout << "Flight number already in use\n "; return; }
    cout << "Duration : "; cin >> duration; cout << endl;
    cout << "Origin : "; cin >> ori; cout << endl;
    cout << "Destination : "; cin >> dest; cout << endl;
    it->getFlights().push_back(Flight(number, duration, d, ori, dest, it->getCapacity(), 0));
}

void Company::removeFlight() {
    int fNumber;
    cout << "Flight Number to remove : "; cin >> fNumber;
    for (auto &p : planes){
        p.getFlights().remove(Flight(fNumber, 0, Date(), "", "", 0, 0));
    }
}



void Company::checkIn(Passenger &p) {
    vector<Flight> fls = getFlightsToCheckIn();
    vector <Ticket> readyCk;
    for (auto &t : p.getTickets())
        if(BinarySearch(fls,t.getFlightNumber()))
            readyCk.push_back(t);

    if (readyCk.size() > 0) {
        cout << endl;
        cout << "    Flight Number    " << endl;
        cout << "=====================" << endl;
        for (auto &t: readyCk) {
            cout << t.getFlightNumber() << endl;
            cout << "---------------------" << endl;
        }
        cout << endl;
        char opt;
        cout << "Yes (Y) to confirm CheckIn\nNO (N) to cancel CheckIn\n( Y / N ) : ";cin >> opt;
        cout << endl;
        if (toupper(opt) == 'Y'){
            readyCk.erase(readyCk.begin());
            list<Flight>::iterator it;
            for (auto &pl : planes) {
                it = find(pl.getFlights().begin(), pl.getFlights().end(), Flight(readyCk[0].getFlightNumber(),0,Date(), "", "", 0, 0));
            }
            cout << "Additional information about destination airport : \n";
            for (auto &a : airports) {
                if (a.getName() == it->getDestination()){
                    a.showTransports();
                    break;
                }
            }
            cout << endl << "Have a nice flight !\n\nEnter any char to continue!\n";
            char i; cin >> i;
        }
        // selecionar opçao do ticket , retira lo ao passenger e perguntar sobre bagage !
    }
    else {cout << "you have " << readyCk.size() << " flights available to Check-in !!\n\n";}


}


vector<Flight> Company::getFlightsToCheckIn() const {
    vector<Flight> flights;
    Date date = Date();
    for (auto pl : planes)
        for (auto &f : pl.getFlights())
            if (f.getDepartureDate().daysSince2020() <= 1 && f.getDepartureDate().daysSince2020() >= 0)
                flights.push_back(f);

    sort(flights,0,flights.size() - 1);
    return flights;
}

void Company::buyTicket(Passenger &p) {
    // Variables
    string numFlight, pack;
    bool validInput = true, avaiablePlaces = false, ynPackage;

    // Print a todos os voos disponiveis
    cout << endl;
    showAllFlights();
    cout << endl;

    // Perguntar qual é o número do voo que deseja
    cout << "What's the number of the flight you want to buy? ";
    do {
        cin >> numFlight; validInput = true;
        for (unsigned l : numFlight) {
            if (!isdigit(l)) { validInput = false; break;}
        }
        if (validInput) {
            validInput = false;
            for (auto &pl: planes) {
                auto f = find(pl.getFlights().begin(), pl.getFlights().end(), Flight(stoi(numFlight), 0, Date(), "", "", 0, 0));
                if (f != pl.getFlights().end() && f->getAvailablePlaces() > 0){
                    validInput = true; avaiablePlaces = true;
                    f->setOccupiedPlaces();
                    cout << "Do you want to take package ";
                    do {
                        cout << "(y or n) : ";
                        cin >> pack;
                    }while (pack != "y" && pack != "Y" && pack != "n" && pack != "N" && !(cin.fail()));
                    if (pack == "y" || pack == "Y") { ynPackage = true; }
                    else { ynPackage = false; }

                    // Create and setup the ticket
                    auto it = find(passengers.begin(), passengers.end(), p);
                    it->getTickets().push_back(Ticket(ynPackage, stoi(numFlight)));
                    return;
                }
            }
        }
        if (!validInput) { cout << "Invalid input! Please input another number for the flight: "; }
        else if (!avaiablePlaces) { cout << "The flight is full! Please choose another flight: "; }        //atençao aquiii !!
    } while (!validInput);


}

void Company::addService() {
    string nameService, dateService, employeeService;
    int plateNumber;
    list<Flight> fl{}; vector<Service> ds{}; queue<Service> tds{};
    cout << "Say the plate of the plane you wish to stop for maintenance or cleaning:" << endl; cin >> plateNumber;
    auto it = find(planes.begin(), planes.end(), Plane(plateNumber, "", 0,  fl, ds, tds));
    if (it == planes.end()){
        cout << "Plane not found !\n\n";
        return;
    }
    cout << "What's the type of service you want to add (maintenance or cleaning)? "<< endl; cin >> nameService;
    cout << "In which date you want to add that service? format(dd/mm/yyyy_hh:mm:ss)"<< endl; cin >> dateService;
    cout << "What´s the name of the employee responsible?" << endl; cin >> employeeService;
    Date b = Date(dateService);
    Service p = Service(nameService,b,employeeService);
    it->setToDoServ(p);
}

void Company::removeService() {
    int plateNumber, opt;
    list<Flight> fl{}; vector<Service> ds{}; queue<Service> tds{};
    cout << "Say the plate of the plane you wish to remove a service:" << endl; cin >> plateNumber;
    auto it = find(planes.begin(), planes.end(), Plane(plateNumber, "", 0,  fl, ds, tds));
    if (it == planes.end()){
        cout << "Plane not found !\n\n";
        return;

    }
    vector<Service> helper = {};
    while(!it->getToDoServ().empty()){
        helper.push_back(it->getToDoServ().front());
        it->getToDoServ().pop();
    }
    int m = 0;
    for(auto &i: helper){
        cout << m << " : " << i.getType() << " " << i.getDate().show() << " " << i.getEmployeeName() << endl;
        m++;
    }
    cout << "Which service number you want to remove?" << endl;cin >> opt;
    helper.erase(helper.begin()+opt);
    for (auto &h: helper){
        it->setToDoServ(h);
    }
}

void Company::showAllServices() {
    cout << "     Type     |        Date        |     Employee Name     " << endl;
    cout << "===========================================================" << endl;
    for (auto p : planes)
        for (int i = 0; i < p.getToDoServ().size(); i++){
            p.getToDoServ().front().show();
            p.getToDoServ().pop();
        }
}










