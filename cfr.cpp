#include "cfr.h"
#include <string>
#include <vector>
#include <iostream>
#include <algorithm>
#include <sstream>
#include <limits>

//exceptiile mele
//DE ADAUGAT: exceptie pentru tipuri gresite de date

class OptionError : public std::runtime_error {
    int option;
    
    public:
        OptionError(int opt) : std::runtime_error("Optiune invalida: " + std::to_string(opt)) {}
};

class ObjectError : public std::runtime_error {
    std::string message;

    public:
        ObjectError(std::string object) : std::runtime_error(object + " nu exista!") {}
};

class ValueError : public std::runtime_error {
    std::string message;

    public:
        ValueError(std::string value) : std::runtime_error(value + " nu este valid/a in acest context.") {}
        ValueError(int value) : std::runtime_error(std::to_string(value) + " nu este valid/a in acest context.") {}
};

class MiscError : public std::runtime_error {
    std::string message;
    public:
        MiscError(std::string value) : std::runtime_error(value) {}
};




std::string clockConvert(int time) {
    std::string output = "";
    int hours = (time / 60) % 24;
    int minutes = time % 60;
    output = std::to_string(hours) + ":" + std::to_string(minutes);

    return output;
}

// CLASA DE BAZA CAR
Car::Car(int _capacity, double _price) : 
    capacity(_capacity), 
    seat_price(_price) {
        passengers.resize(_capacity, -1);
}

void Car::print(std::ostream& out) const {
    out << "Vagon(locuri: " << capacity << ", pret: " << seat_price << ")\n";
}

std::ostream& operator<<(std::ostream& out, const Car& c) {
    c.print(out);
    return out;
}

// UNIVERSALE
void Car::addPassenger(int _id, int _seat) {
    passengers[_seat] = _id;
    std::cout << "(mesaj intern): Pasager cu ID " << _id << " adaugat la locul " << _seat << "\n";
}

void Car::printAllPassengers() {
    std::cout << "\n(USA)\n";
    int i = 0;
    for(const int& elem : passengers)
        std::cout << i++ + 1 << ": " << elem << "\n";

    std::cout << "[USA]\n";
}

int Car::occupyEmptySeat(int id) {
    for(int seat : passengers) {
        if(seat == -1) {
            seat = id;
            return seat;
        }
    }
    
    return -1; //nu exista loc gol     
}

//CLASA CUSETA COUCHETCAR---------------------------------------------------------------------------------

CouchetCar::CouchetCar(int _capacity, double _price, int _bed, double _bedsheet) : 
            Car(_capacity, _price),
            bed_nr(_bed),
            bedsheet_price(_bedsheet) {}

CouchetCar::CouchetCar(const CouchetCar& other) : 
            Car(other), 
            bed_nr(other.bed_nr),
            bedsheet_price(other.bedsheet_price) {}

CouchetCar& CouchetCar::operator=(const CouchetCar& other) {
    if (this == &other)
        return *this;

    capacity = other.capacity;
    seat_price = other.seat_price;

    //Car::operator=(other);
    bed_nr = other.bed_nr;
    bedsheet_price = other.bedsheet_price;

    return *this;
}
void CouchetCar::print(std::ostream& out) const {
    Car::print(out);
    out << "Cuseta(numar paturi: " << bed_nr << ", pret lenjerie: " << bedsheet_price << ")\n";
}

double CouchetCar::getPrice() const {
    return seat_price + bedsheet_price;
}

std::string CouchetCar::type() {
    return "Vagon Cuseta";
}



//------------------------------------------------STANDARD CAR---------------------------------------------
StandardCar::StandardCar(int _capacity, double _price, int _class, int _bike) :
        Car(_capacity, _price),
        car_class(_class),
        bike_seats(_bike) {}

StandardCar::StandardCar(const StandardCar& other) :
        Car(other),
        car_class(other.car_class),
        bike_seats(other.bike_seats) {}

StandardCar& StandardCar::operator=(const StandardCar& other) {
    if(&other == this) 
        return *this;
    
    Car::operator=(other);
    car_class = other.car_class;
    bike_seats = other.bike_seats;

    return *this;
}

double StandardCar::getPrice() const {
    return seat_price + (2 - car_class) * 50 / 100 * seat_price;
}

std::string StandardCar::type() {
    if(car_class == 1)
        return "Vagon Clasa Intai";
    else return "Vagon Clasa a Doua";
}
//-----------------------------------------------------------------------------------------------------
//--------------------------------------------------TREN----------------------------------------------
//---------------------------------------------------------------------------------------------------

Train::Train(int _dep, int _time) : departure_time(_dep), time_to_station(_time) {
    cars_nr = 0;
    //std::cout << "train initialized\n";
}

Train::Train(const Train& other) {
    departure_time = other.departure_time;
    time_to_station = other.time_to_station;
    cars_nr = other.cars_nr;
    traincars = other.traincars;

    std::cout << "train copy constructor";
}

Train::~Train() {
    for(Car* traincar : traincars)
        delete traincar;

    std::cout << "train emptied and erased\n";
}

Train& Train::operator=(const Train& other) {
    if(&other == this)
        return *this;
    
    departure_time = other.departure_time;
    time_to_station = other.time_to_station;
    cars_nr = other.cars_nr;
    traincars = other.traincars;

    return *this;
}

bool Train::operator<(const Train& other) {
    return departure_time < other.departure_time;
}

void Train::pushCar(Car *cart) {
    traincars.push_back(cart);
    cars_nr++;
}

void Train::removeCars() {
    for(Car* traincar : traincars)
        delete traincar;
    traincars.clear();
    cars_nr = 0;
}


void Train::printTraincars() {
    int i = 0;
    for(Car* car : traincars)
        std::cout << ++i << ": " << car->type() << "\n";
}

void Train::mapCars() {
    for(auto it : traincars) {
        if(std::count(carOptions.begin(), carOptions.end(), it->type()) == 0)
            carOptions.push_back(it->type());
    }
}
void Train::printOptions() {
    int i = 0;
    for(auto it : carOptions) {
        std::cout << ++i << ": " << it << "\n";
    }
}
std::string Train::getCarOption(int option) {
    return carOptions[option - 1];
}

carInfo Train::occupyEmptySeat(std::string type, int id) {
    int i = 0;
    int seat = 0;
    
    carInfo info;

    for(auto car : traincars) {
        seat = car->occupyEmptySeat(id);
        if(seat != -1) {
            info.seat_number = i + 1;
            info.car_number = seat;
        }

        i++;
    }

    return info;
}

bool Train::isFull() {
    int seat = 0;
    for(auto car : traincars) {
        seat = car->occupyEmptySeat(0); //REVEZI: SA NU EXISTE ID 0
        if(seat != -1)
            return false;
    }

    return true;
}

double Train::getPrice(std::string type) const {
    for(auto car : traincars) {
        if(car->type() == type)
            return car->getPrice();
    }

    return 999999999; //nu se va intampla vreodata
}



//--------------------------------------------------------------------------------------------------------
//-------------------------------------------------RUTA--------------------------------------------------
//------------------------------------------------------------------------------------------------------

Route::Route(const Route& other) {
    stations = other.stations;
    trains = other.trains;
}

Route::~Route() {
    for(auto train : trains)
        delete train;
}

void Route::pushStation(std::string station) {
    stations.push_back(station);
}

void Route::pushTrain(Train* train) {
    trains.push_back(train);   
}

void Route::printStations() {
    int i=0;
    for(const auto& it : stations) 
        std::cout << ++i << ": " << it << "\n";
}

std::string Route::getRouteName() const {
    return stations.front() + " - " + stations.back();
}

int Route::hasStation(std::string station) const {
    int index = 0;

    for(auto& it : stations) {
        if(it == station) return index;
        index++;
    }
    return -1;
}

void Route::sortTrains() {
    std::sort(trains.begin(), trains.end(), [](Train* &t1, Train* &t2) {
        return *t1 < *t2;
    });
}
//------------------------------------------------------------------------------------------------------
//-------------------------------------------BILET-----------------------------------------------------
//----------------------------------------------------------------------------------------------------
int Trip::trip_counter = 0;

Trip::Trip(std::string dep, std::string arr, std::string routeName, std::string start, std::string end, int seat, int car, std::string type, double _price) :
    departure_time(dep), arrival_time(arr), routeName(routeName), start(start), end(end), seat_nr(seat), car_nr(car),
    car_type(type), price(_price)  {
        trip_id = ++trip_counter;
    }

void Trip::printInfo() const {
    std::cout << departure_time << "-" << arrival_time <<" " << start << "-" << end << " (" << routeName << ")\n";
    std::cout << "\t" << car_type << " / Numar vagon: " << car_nr << " / Loc: " << seat_nr << "\n"; 
}

int Ticket::ticket_counter = 0;
int Ticket::ticketCount() {
    return ticket_counter;
}

Ticket::~Ticket() {
    for(auto trip : trips)
        delete trip;
}

Ticket::Ticket() {
    ticket_counter++;
    ticket_id = ticket_counter;
}

Ticket::Ticket(std::string _name) : 
        name(_name) {
            ticket_counter++;
            ticket_id = ticket_counter;
        }

void Ticket::setTravel() {
    if(trips.size() == 0)
        return;

    start = trips.at(0)->getDep();
    destination = trips.at(trips.size() - 1)->getArr();
}
void Ticket::appendTrip(Trip* trip) {
    trips.push_back(trip);
}

void Ticket::printTrips() const {
    for(auto trip : trips) {
        trip->printInfo();
    }
}

void Ticket::setPrice() {
    double price = 0;
    for(auto it : trips)
        price+=it->getPrice();

    this->price = getFinalPrice(price);
}

FullPriceTicket::FullPriceTicket(std::string _name, int _id) :
        Ticket(_name), id_card(_id) {}
double FullPriceTicket::getFinalPrice(double initial) {
    return initial;
}
void FullPriceTicket::printInfo() const{
    std::cout << "Nume: "<< name << "\nID: " << id_card << "\nPret: " << price << " lei\n\n";
    printTrips();
}

YouthTicket::YouthTicket(std::string _name, int _id) :
        Ticket(_name), student_id(_id) {
            this->discount = 90;
        }
double YouthTicket::getFinalPrice(double initial) {
    return (100 - discount) * initial / 100;
}
void YouthTicket::printInfo() const{
    std::cout << "Nume: "<< name << "\nID student/elev: " << student_id << "\nPret: " << price <<  " lei\n\n";
    printTrips();
}

ElderlyTicket::ElderlyTicket(std::string _name, int _id) :
        Ticket(_name), health_card(_id) {
            discount = 70;
        }
double ElderlyTicket::getFinalPrice(double initial) {
    return (100 - discount) * initial / 100;
}
void ElderlyTicket::printInfo() const {
    std::cout << "Nume: "<< name << "\nID: " << health_card << "\nPret: " << price << " lei\n\n";
    printTrips();
}

MembershipTicket::MembershipTicket(std::string _name, int _id, std::string _account) :
        Ticket(_name), 
        FullPriceTicket(_name, _id),
        account_name(_account) {
            this->discount = 30;
        }
double MembershipTicket::getFinalPrice(double initial) {
    return (100 - discount) * initial / 100;
}
void MembershipTicket::printInfo() const{
    std::cout << "Nume: "<< name << "\nID: " << id_card << "\nNUME UTILIZATOR: " << account_name << "\nPret: " << price << " lei\n\n";
    printTrips();
}

//-------------------------------------------------------------------------------------------------------
//------------------------------------ROUTE TABLE--------------------------------------------------------
//-------------------------------------------------------------------------------------------------------
RouteTable::~RouteTable() {
    for(auto it : routes)
        delete it;
}

void RouteTable::appendRoute(Route* route) {
    routes.push_back(route);
}

void RouteTable::printRoutes() {
    int i = 0;
    for(auto& route : routes)
        std::cout << ++i << ": " << route->getRouteName() << "\n";
}

const Route* RouteTable::findRoute(std::string name) {
    for(auto& route : routes)
        if(route->getRouteName() == name)
            return route;
    
    return nullptr;
    //REVEZI: exceptie nu exista!
}

//-----------------------------------------------------------------------------------------------
//--------------------------------------------MENIUL-----------------------------------------------
//--------------------------------------------------------------------------------------------------

TicketManager* TicketManager::instance = nullptr;

TicketManager::TicketManager() {
    graph = new RouteGraph();
    table = new RouteTable();
}

Train* TicketManager::highSpeedTrain() {
    int dep_time;                                        
    std::cout << "Introduceti ora plecarii: ";
    std::cin >> dep_time;
    if(dep_time < 0 || dep_time >= 1440)
        throw ValueError(std::to_string(dep_time));
    
    std::cout << "\n";

    Train* hst = new Train(dep_time, 20);

    Car* auxcar;
    for(int i = 0; i < 6; i++) {
        auxcar = new StandardCar(50, 30, 2, 6);
        hst->pushCar(auxcar);
    }

    for(int i = 0; i < 6; i++) {
        auxcar = new StandardCar(50, 50, 1, 10);
        hst->pushCar(auxcar);
    }

    if(dep_time > 1230) {
        for(int i = 0; i < 4; i++) {
            auxcar = new CouchetCar(30, 40, 6, 15);
            hst->pushCar(auxcar);
        }
    }

    hst->mapCars();
    return hst;
}

Train* TicketManager::regionalTrain() {
    int dep_time;                                  
    std::cout << "Introduceti ora plecarii: ";
    std::cin >> dep_time;
    if(dep_time < 0 || dep_time >= 1440)
        throw ValueError(std::to_string(dep_time));
    std::cout << "\n";

    Train* rt = new Train(dep_time, 30);

    Car* auxcar;
    for(int i = 0; i < 4; i++) {
        auxcar = new StandardCar(40, 25, 2, 4);
        rt->pushCar(auxcar);
    }

    for(int i = 0; i < 4; i++) {
        auxcar = new StandardCar(40, 45, 1, 8);
        rt->pushCar(auxcar);
    }

    if(dep_time > 1230) {
        for(int i = 0; i < 3; i++) {
            auxcar = new CouchetCar(20, 40, 4, 10);
            rt->pushCar(auxcar);
        }
    }

    rt->mapCars();
    return rt;
}

Car* TicketManager::manualAddCar() {
    int capacity;
    double price;
    std::cout << "Introduceti capacitatea vagonului: ";
    std::cin >> capacity;
    if(capacity <= 0 || capacity > 200)
        throw ValueError(std::to_string(capacity));

    std::cout << "Introduceti pretul per loc: ";
    std::cin >> price;
    if(price <= 0)
        throw ValueError(std::to_string(price));
        
    // 1 - CouchetCar, 2 - StandardCar
    std::cout << "Ce fel de vagon doriti?\n1 = Vagon cuseta\n2 = Vagon Standard\n>";
    int type;
    std::cin >> type;

    Car* car;

    switch(type) 
    {
        case 2:
            int car_class, bike_seats;
            std::cout << "Introduceti clasa vagonului: ";
            std::cin >> car_class;
            if(car_class > 2 || car_class < 1)
                throw ValueError(std::to_string(car_class));

            std::cout << "Introduceti numarul de locuri pentru biciclete: ";
            std::cin >> bike_seats;
            if(bike_seats <= 0)
                throw ValueError(std::to_string(bike_seats));

            car = new StandardCar(capacity, price, car_class, bike_seats);
            break;
        case 1:
            int bed_count;
            std::cout << "Introduceti numarul de paturi per compartiment: ";
            std::cin >> bed_count;
            if(bed_count <= 0)
                throw ValueError(std::to_string(bed_count));

            double sheet_price;
            std::cout << "Introduceti pretul pentru lenjerie: ";
            std::cin >> sheet_price;
            if(sheet_price <= 0)
                throw ValueError(std::to_string(sheet_price));

            car = new CouchetCar(capacity, price, bed_count, sheet_price);
            break;
        default:
            throw OptionError(type);
    }
    return car;
}

Train* TicketManager::manualAddTrain() {
    std::cout << "---- INTRODUCEREA MANUALA A UNUI TREN IN BAZA DE DATE ----\n";
    
    std::cout << "Introduceti timpul de plecare: ";
    int departure_time;
    std::cin >> departure_time;
    if(departure_time < 0 || departure_time >= 1440)
        throw ValueError(std::to_string(departure_time));

    std::cout << "\nIntroduceti timpul mediu petrecut intre statii: ";
    int avg_time;
    std::cin >> avg_time;
    if(avg_time < 5 || avg_time >= 1440)
        throw ValueError(std::to_string(avg_time));

    Train* tren = new Train(departure_time, avg_time);

    //vagoanele
    std::cout << "\nIntroduceti numarul de vagoane: ";
    int car_count;
    std::cin >> car_count;
    if(car_count < 1 || car_count >= 200)
        throw ValueError(std::to_string(car_count));

    bool tryAgain = true;
    Car* aux;
    for(int i = 0; i < car_count; i++) {

        while(tryAgain) {
            try {
                aux = manualAddCar();
                tryAgain = false;   
            }
            catch(const OptionError &e){
                std::cout << "Exceptie: " << e.what() << "\n";
                tryAgain = true;
            }
            catch(const ValueError &e1) {
                std::cout << "Exceptie: " << e1.what() << "\n";
                tryAgain = true;
            }
        }

        tren->pushCar(aux);
    }

    tren->mapCars();
    return tren;
}

void TicketManager::populateTable() {
    initialized = true;

    Route* routepoint = nullptr;
    
    int num = 0;
    std::string statie;
    std::cout << "Introduceti numarul de rute de adaugat: ";
    std::cin >> num;
    if(num < 1) 
            throw ValueError(num);

    std::cout << "Introduceti statiile. \nDupa ultima statie din ruta, introduceti '/'.\n";
    std::cout << "Exemplu: Bucuresti Titu Targoviste /";

    for(int i = 0; i < num; i++) {
        std::cout << "\nruta " + std::to_string(i + 1) + ":\n";
        routepoint = new Route();

        //1.
        //se dau statiile
        std::cin >> statie;

        while(statie != "/") {
            //ASTA E TOLOWER........am cautat pe net
            for (char &c : statie) 
                c = std::tolower((unsigned char)c);

            routepoint->pushStation(statie);
            std::cin >> statie;
        }

        //1.5
        //se adauga ruta in graf
        graph->addRoute(*routepoint);

        //2.
        //se dau trenurile -> manual sau cu presetari
        int train_count;


        std::cout << "Introduceti numarul de trenuri dorite: ";
        std::cin >> train_count;
        if(train_count < 1) 
            throw ValueError(train_count);

        std::cout << "Ce tip de tren doriti sa creati?\n";

        int ttype = 0;
        for(int i = 0; i < train_count; i++) {
            bool tryAgain = true;

            while(tryAgain) //AICI SUNT PRINSE TOATE ERORILE DE LA CREAREA TRENULUI
            try {
                    std::cout << "Trenul " << std::to_string(i + 1) << ":\n";
                    std::cout << "\n1 - Tren de Inalta Viteza \t(Presetare - Va fi creat automat)\n"\
                    << "2 - Tren Regional \t\t(Presetare - Va fi creat automat)\n"\
                    << "3 - Introducere Manuala\n>";
                    std::cin >> ttype;

                    switch (ttype) {
                    case 1:
                        //tren preset
                        routepoint->pushTrain(highSpeedTrain());
                        tryAgain = false;
                        break;
                    case 2:
                        routepoint->pushTrain(regionalTrain());
                        tryAgain = false;
                        break;
                    case 3:
                        routepoint->pushTrain(manualAddTrain());
                        tryAgain = false;
                        break;
                    default:
                        throw OptionError(ttype); 
                }
            }
            catch(const OptionError &e) {
                std::cout << "Exceptie: " << e.what() << "\n";
                tryAgain = true;
            }
            catch(const ValueError &e1) {
                std::cout << "Exceptie: " << e1.what() << "\n";
                tryAgain = true;
            }
            



        }
        routepoint->sortTrains();
        //3.
        //a terminat de adaugat trenuri; adaugi ruta in tabel
        table->appendRoute(routepoint);
    }
}

TicketManager::~TicketManager() {
    delete graph;
    delete table;
}

void TicketManager::printRouteGraph() {
    graph->printGraph();
}

void TicketManager::buyTicket() {
    std::cout << "Ce tip de bilet doriti sa achizitionati?\n";

    bool tryAgain = true;
    int choice;
    Ticket* ticket;

    while(tryAgain) {
        try {
            std::cout << "1 - Bilet Intreg\n";
            std::cout << "2 - Bilet Tineret (Elev/Student)\n";
            std::cout << "3 - Bilet Pensionar\n";
            std::cout << "4 - Bilet CFR+\n";
            std::cout << ">";
            std::cin >> choice;

            switch(choice) {
                case 1:
                    ticket = buyFullPrice();
                    tryAgain = false;
                    break;
                case 2:
                    ticket = buyYouth();
                    tryAgain = false;
                    break;
                case 3:
                    ticket = buyElderly();
                    tryAgain = false;
                    break;
                case 4:
                    ticket = buyMembership();
                    tryAgain = false;
                    break;
                default:
                    throw OptionError(choice);
            }
        }
        catch(const OptionError& e) {
            std::cout << "Exceptie: " << e.what() << "\n";
        }
    }
    
    try {
        routeSearch(ticket);
    }
    catch(const MiscError& e) {
        std::cout << "Exceptie: " << e.what() << "\n";
    }
    
}

Ticket* TicketManager::buyFullPrice() {
    std::string name;
    std::cout << "Introduceti-va numele complet:\n";
    std::cin.ignore (std::numeric_limits<std::streamsize>::max(), '\n');
    std::getline(std::cin, name);

    int id;
    std::cout << "Introduceti CNP-ul: ";
    std::cin >> id;
    if(id < 1) throw ValueError(id);
    
    Ticket* ticket = new FullPriceTicket(name, id);
    return ticket;
}
Ticket* TicketManager::buyYouth() {
    std::string name;
    std::cout << "Introduceti-va numele complet:\n";
    std::cin.ignore (std::numeric_limits<std::streamsize>::max(), '\n');
    std::getline(std::cin, name);

    int id;
    std::cout << "Introduceti numarul matricol (student)/CNP-ul (elev): ";
    std::cin >> id;
    if(id < 1) throw ValueError(id);
    
    Ticket* ticket = new YouthTicket(name, id);
    return ticket;
}
Ticket* TicketManager::buyElderly() {
    std::string name;
    std::cout << "Introduceti-va numele complet:\n";
    std::cin.ignore (std::numeric_limits<std::streamsize>::max(), '\n');
    std::getline(std::cin, name);

    int id;
    std::cout << "Introduceti codul cardului de sanatate: ";
    std::cin >> id;
    if(id < 1) throw ValueError(id);
    
    Ticket* ticket = new ElderlyTicket(name, id);
    return ticket;
}
Ticket* TicketManager::buyMembership() {
    std::string name, username;
    std::cout << "Introduceti-va numele complet:\n";
    std::cin.ignore (std::numeric_limits<std::streamsize>::max(), '\n');
    std::getline(std::cin, name);

    int id;
    std::cout << "Introduceti CNP-ul: ";
    std::cin >> id;
    if(id < 1) throw ValueError(id);

    std::cout <<"Introduceti numele dvs. de utilizator (FARA SPATII): ";
    std::cin >> username;
    
    Ticket* ticket = new MembershipTicket(name, id, username);
    return ticket;
}

bool TicketManager::checkForNode(std::string node) {
    return graph->checkForNode(node);
}

/*
1 - Tren de Inalta Viteza   (Presetare - Va fi creat automat)
2 - Tren Regional           (Presetare - Va fi creat automat)
*/

/*
-----------------------------------------------------------------------------------------------------
------------------------------------------ALGORITMUL DE CAUTARE--------------------------------------
-----------------------------------------------------------------------------------------------------
*/

bool RouteGraph::checkForNode(std::string node) {
    return adjList.find(node) != adjList.end();
}


void RouteGraph::addEdge(std::string st1, std::string st2, Route& route) {
    //aceste linii daca exista deja statia
    int index = RouteGraph::keyHasEdge(st1, st2);

    if(index != -1) {
        bool found = false;
        for(auto& it : adjList[st1][index].routes)
            if(it == &route)
                found = true;

        if(!found)
            adjList[st1][index].routes.push_back(&route);
        //std::cout << "Am ajuns aici\n";
    }
    else { //daca nu este gasita
        RouteEdge edge;
        edge.toStation = st2;
        edge.routes = {&route};
        adjList[st1].push_back(edge);
    }
        
}

void RouteGraph::addRoute(Route& route) {
    const std::vector<std::string>& routep = route.getStations();
    int n = routep.size();
    for(int i = 0; i < n - 1; i++) {
        //adaugi ambele legaturi pentru ca este graf neorientat!
        addEdge((routep)[i], (routep)[i + 1], route);

        addEdge((routep)[i + 1], (routep)[i], route);
    }
}

void RouteGraph::printGraph() {
    int n = 0;
    for(auto& it : adjList) {
        std::cout << it.first << ": ";

        n = it.second.size();
        for(int i = 0; i < n; i++) {
            //std::cout << it.second[i].toStation << " (" << it.second[i].routeOrigin->getRouteName() << ")";
            std::cout << it.second[i].toStation << " ";
            for(auto& route : it.second[i].routes)
                std::cout << "(" << route->getRouteName() << ")";
            if(i < n - 1) std::cout << ", "; 
        }
        std::cout << "\n";
    }
}

//functie ca sa cauti in lista de adiacenta dupa numele statiei, nu dupa RouteEdge
//returneaza indexul muchiei in vector daca este gasita
int RouteGraph::keyHasEdge(std::string key, std::string station) {
    int pos = 0;
    for(auto& it : adjList[key]){
        if(it.toStation == station)
            return pos;
        
        pos++;
    }

    return -1;
}

//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
//::::::::::::::::::::::::::::::::::::::::::::::::QUEUE:::::::::::::::::::::::::::::::::::::::::::::::::
bool Queue::isEmpty() {
    return head == nullptr;
}
void Queue::push(std::string val) {
    Node* p = new Node();
    p->value = val;
    if(isEmpty())
        head = tail = p;
    else {
        tail->next = p;
        tail = p;
    }
}
void Queue::pop() {
    if(isEmpty()) {
        std::cout << "Queue already empty before popping!\n"; //REVEZI EXCEPTIE
        return;
    }
    Node* p = head;
    head = head->next;
    if(head == nullptr)
        tail = nullptr; //in cazul in care coada mai avea doar 1 element

    delete p;
}
std::string Queue::front() {
    if(!isEmpty()) return head->value;
    return nullptr;
}
Queue::~Queue() {
    Node* p = head, *aux;
    while(p != nullptr) {
        aux = p;
        p = p->next;
        delete aux;
    }
}
//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::


//ALGORITM CARE CREAZA UN TRASEU CU CAT MAI PUTINE SCHIMBARI DE TREN POSIBIL,
//FOLOSIND VECTORUL CU MUCHII FACUT DE BFS  
std::vector<rawTrack> algoritmulMeu(std::vector<RouteEdge>* raw) { //aici puteai folosi si const reference, 
    std::vector<rawTrack> tracks = {};
    std::vector<rawTrack> solution = {};
    rawTrack aux;
    
    //creezi vectorul
    int pos = 1;
    auto iter = raw->begin();
    iter++;
    for(auto edge = iter; edge != raw->end(); edge++) {
        for(auto& route : edge->routes) {
            auto it = tracks.begin();
            while(it != tracks.end() && it->name != route->getRouteName())
                it++;

            if(it == tracks.end()) {
                tracks.push_back(rawTrack(pos - 1, pos, route->getRouteName()));
            }
            else {
                it->end++;
            }
        }
        pos++;   
    }
    
    //sortezi crescator in functie de start
    std::sort(tracks.begin(), tracks.end());

    int end = 0; //primul nod
    auto startingPos = tracks.begin();
    /*
    for(const auto& it : tracks) 
        std::cout << (*raw)[it.start].toStation << " " << (*raw)[it.end].toStation << " " << it.name << "\n";
    std::cout << raw->size()<< "\n";
    */
   
    //cauti rutele cele mai bune ()
    while(end < static_cast<int>(raw->size() - 1)) {
        auto it = startingPos;
        auto itmax = it;
        it++;
        while(it != tracks.end() && it->start <= end) {
            if(it->end - it->start > itmax->end - itmax->start)
                itmax = it;
            
            it++;
        }

        solution.push_back(rawTrack(end, itmax->end, itmax->name));
        std::cout << (*raw)[end].toStation << " " << (*raw)[itmax->end].toStation << " (" << itmax->name << ")\n"; 

        end = itmax->end;
        if(it != tracks.end())
            startingPos = it;
    }

    return solution;
}

std::vector<Train*> TicketManager::options(const std::vector<RouteEdge>& solution, const std::vector<rawTrack>& tracks) {
    std::vector<std::vector<Train*>> trains_options = {};
    std::vector<Train*> trains_to_buy = {};
    
    bool ok = true; //verifica daca solutia curenta este inca valida

    int arrival = 0; //este ora la care ajunge trenul in statie(timpul la care pleaca urm tren trebuie sa fie mai mare)
    int departure = 0;
    int firstDeparture = 0;
    auto it = tracks.begin();
    int sense = 1;
    std::stringstream output; //CA SA FACA EFICIENT output in terminal
    int count = 0;
    
    const Route* main_route = table->findRoute(it->name);

    //ca sa stabilesti timpul de plecare si de sosire al unui tren
    //se presupune ca pleaca din ambele capete la aceeasi ora
    int endIndex = main_route->hasStation(solution[it->end].toStation);
    int startIndex = main_route->hasStation(solution[it->start].toStation);
    int routeSize = (int)main_route->getStations().size();
    if(endIndex < startIndex)
            sense = 0;

    for(auto& train : main_route->getTrains()) {
        if(train->isFull() == false) {


        output.str(""); //sa devina goala
        output.clear(); // :)

        trains_to_buy.clear();
        trains_to_buy.push_back(train);

        it = tracks.begin();
        ok = true;

        if(!sense) {
            arrival = train->getDeparture() + train->getSpeed() * (routeSize - endIndex - 1); //(indexul e de la 0)
            departure = train->getDeparture() + train->getSpeed() * (routeSize - startIndex - 1);
        }
        else {
            arrival = train->getDeparture() + train->getSpeed() * endIndex;
            departure = train->getDeparture() + train->getSpeed() * startIndex;
        }
        firstDeparture = departure;
        output << clockConvert(departure) << "-" << clockConvert(arrival) << " ";
        output << solution[it->start].toStation << "-" << solution[it->end].toStation << " ";
        output << "(" << it->name << ")\n";
        //std::cout << clockConvert(departure) << "-" << clockConvert(arrival) << " ";

        it++;
        while(it != tracks.end() && ok) {
            Train* save = nullptr;
            const Route* aux_route = table->findRoute(it->name);

            int _sense = 1;
            int _endIndex = aux_route->hasStation(solution[it->end].toStation);
            int _startIndex = aux_route->hasStation(solution[it->start].toStation);
            int _routeSize = (int)aux_route->getStations().size();

            if(_endIndex < _startIndex)
                    _sense = 0;

            for(auto& train : aux_route->getTrains()) {
                if(train->isFull() == false) {

                    if(!_sense) {
                        departure = train->getDeparture() + train->getSpeed() * (_routeSize - _startIndex - 1);
                    }
                    else {
                        departure = train->getDeparture() + train->getSpeed() * _startIndex;
                    }

                    if(departure > arrival) {
                        save = train;
                        break;
                    }
                }
            }

            if(save != nullptr) {
                if(!_sense) {
                    arrival = save->getDeparture() + save->getSpeed() * (_routeSize - _endIndex - 1);
                }
                else {
                    arrival = save->getDeparture() + save->getSpeed() * _endIndex;
                }

                output << clockConvert(departure) << "-" << clockConvert(arrival) << " ";
                output << solution[it->start].toStation << "-" << solution[it->end].toStation << " ";
                output << "(" << it->name << ")\n";
                //std::cout << clockConvert(departure) << "-" << clockConvert(arrival) << " ";
                trains_to_buy.push_back(save);
            }
            else {
                std::cout << "NU AM GASIT!\n"; //REVEZI EXCEPTIE
                ok = false;
            }
            it++;
        }
        

        if(ok) {
            std::cout << "Optiunea " << std::to_string(count++ + 1) << ": ";
            std::cout << clockConvert(firstDeparture) << "-" << clockConvert(arrival)<< "\n\t";
            std::cout << output.str();

            trains_options.push_back(trains_to_buy);
        }
    }
    }

    int choice;
    bool tryAgain = true;
    while(tryAgain) {
        try {
            std::cout << "Introduceti optiunea dorita: ";
            std::cin >> choice;
            if(choice < 1 || choice > (int)trains_options.size())
                throw OptionError(choice);

            tryAgain = false;
        }
        catch(const OptionError &e) {
            std::cout << "Exceptie: " << e.what() << "\n";
        }
    }
    
    return trains_options[choice - 1];
}


void TicketManager::routeSearch(Ticket* ticket) {
    std::string start;
    std::string end;
    bool tryAgain = true;

    //introducerea statiilor de plecare, sosire
    while(tryAgain) {
        try {
            std::cout << "Introduceti statia de plecare: ";
            std::cin >> start;

            for (char &c : start) 
                c = std::tolower((unsigned char)c);

            std::cout << "Introduceti statia destinatie: ";
            std::cin >> end;

            for (char &c : end) 
                c = std::tolower((unsigned char)c);
            
            //verifica daca se gasesc in graf
            if(!checkForNode(start))
                throw MiscError("Statia " + start + "nu se afla in retea.");

            if(!checkForNode(end))
                throw MiscError("Statia " + end + "nu se afla in retea.");

            tryAgain = false;
        }
        catch(const MiscError& e) {
            std::cout << "Exceptie: " << e.what() << "\n";
        }
    }

    std::vector<RouteEdge> solution;
    std::vector<rawTrack> suggestion_tracks;

    solution = graph->bfs(start, end); //BFS in graful RouteGraf

    //afisare
    std::cout << start << "\n";
    for(auto& edge : solution) {
        std::cout << edge.toStation << ", cu ruta:\t";
        for(auto& route : edge.routes)
            std::cout << "(" << route->getRouteName() << ")";
        std::cout << "\n";
    }
    solution.insert(solution.begin(), RouteEdge(start));
    std::cout << "Solutia sugerata:\n";

    suggestion_tracks = algoritmulMeu(&solution);
    std::cout << "\n";

    std::vector<Train*> to_buy = options(solution, suggestion_tracks);

    // PENTRU CITITORI: CE INSEAMNA TOATE ASTEA DE FAPT?
    
    // SOLUTION:
    // vector cu statiile(noduri, chiar daca sunt de tip muchie) vizitate pentru a ajunge de la start la end
    // elementele contin numele statiei, si un vector cu rute posibile pentru a ajunge la statia respectiva
    // trebuie prelucrat mai departe pentru a putea stabili un traseu pentru client
    
    // SUGGESTION_TRACKS:
    // aplic algoritmulMeu peste solution si primesc un traseu posibil (eficient)
    // este un vector care contine rute pe care le poate folosi un calator ca sa ajunga din start in end
    // fiecare element este format din:
    //      indexul statiei din solution de unde pleaca
    //      indexul statiei din solution unde ajunge
    //      numele rutei folosite

    // TO_BUY:
    // dupa ce am creat suggestion_tracks, trebuie gasite trenurile pentru a parcurge traseul
    // in functia options(), clientul va alege ce serie de trenuri va lua pentru a ajunge la destinatie
    // to_buy este vectorul cu pointeri la trenurile alese
    // are atatea elemente cate 'rute' sunt in suggestion_tracks

    int choice;
    
    int i = 0;
    for(auto it = to_buy.begin(); it != to_buy.end(); it++) {
        std::cout << "Pentru trenul: " << solution[suggestion_tracks[i].start].toStation << "-" << solution[suggestion_tracks[i].end].toStation << "\n";
        
        // EXPLICATIE:
        // pentru fiecare tren din to_buy, se creaza o calatorie care va fi adaugata la bilet
        // se creeaza un element de tip Trip, si pe rand ii sunt asamblate toate componentele necesare
        // **vezi Trip

        //clientul alege vagonul, si i se atribuie automat un loc in el
        std::string car_type;
        tryAgain = true;
        std::cout << "Alegeti tipul de vagon:\n";
        
        while(tryAgain) {
            (*it)->printOptions();
            std::cin >> choice;

            try {
                car_type = (*it)->getCarOption(choice);
                tryAgain = false;
            }
            catch(const std::out_of_range& e) {
                std::cout << "Exceptie: Optiune invalida!\n";
            }
        }
        int depraw, arrraw;
        std::string departure, arrival;
        std::string routeName = suggestion_tracks[i].name;

        std::string start, end;
        start = solution[suggestion_tracks[i].start].toStation;
        end = solution[suggestion_tracks[i].end].toStation;
        
        int seat, car;
        carInfo info = (*it)->occupyEmptySeat(car_type, ticket->getId());
        seat = info.seat_number; car = info.car_number;
        double price = (*it)->getPrice(car_type);
               

        //trebuie sa verific sensul mai intai                                                                   aici !
        int startIndex = table->findRoute(suggestion_tracks[i].name)->hasStation(solution[suggestion_tracks[i].start].toStation);
        int endIndex = table->findRoute(suggestion_tracks[i].name)->hasStation(solution[suggestion_tracks[i].end].toStation);
        int routeSize = (int)table->findRoute(suggestion_tracks[i].name)->getStations().size();
        
        if(startIndex > endIndex) { //daca sensul e invers
            depraw = (*it)->getDeparture() + (*it)->getSpeed() * (routeSize - startIndex - 1);
            arrraw = (*it)->getDeparture() + (*it)->getSpeed() * (routeSize - endIndex - 1);
        }
        else {
            depraw = (*it)->getDeparture() + (*it)->getSpeed() * (startIndex);
            arrraw = (*it)->getDeparture() + (*it)->getSpeed() * (endIndex);
        }
        
        departure = clockConvert(depraw);
        arrival = clockConvert(arrraw);

        //ACUM AI TOTUL PENTRU TRIP
        Trip* trip = new Trip(departure, arrival, routeName, start, end, seat, car, car_type, price);
        
        ticket->appendTrip(trip);
        //care face un unordered map cu tipurile de vagoane si indexul unde se afla.
        
        //afisezi tipul de vagon si alegi de care doresti
        //primesti pretul
        //loc RANDOM in vagon pentru ca deja este prea mult
        //alegi tipul de bilet si introduci detalii
        //la final vreau info cu totul despre client, eventual salvate intr un vector in TicketManager
        //pot fi afisate cu o functie din ticketManager
        
        //exceptii si gata! GATA
        i++;
    }

    ticket->setTravel();
    ticket->setPrice();
    tickets[ticket->getId()] = ticket;
    std::cout << "Biletul dvs, cu codul " << ticket->getId() << ", a fost cumparat si inregistrat cu succes!\n";
}  



RouteEdge::RouteEdge(std::string _toStation) : toStation(_toStation) {}

std::vector<RouteEdge> RouteGraph::bfs(std::string start, std::string end) {
    //pentru fiecare nod vizitat, memorez in visited statia de unde am venit, si ruta folosita 
    //(folosesc routeEdge pt asta)
    std::unordered_map<std::string, RouteEdge> visited;
    Queue q;
    RouteEdge aux1;

    //pun vecinii lui start pe queue
    visited[start] = RouteEdge();
    for(auto& it : adjList[start]) {
        //sstd::cout << it.toStation << "\n";
        q.push(it.toStation);
        
        aux1.routes = it.routes;
        aux1.toStation = start;
        visited[it.toStation] = aux1;
        //visited[it.toStation] = {it.routes, start};
    }

    if(q.isEmpty()) {
        //revezi: exceptie nod izolat (nu ar trebui sa se intample)
        std::cout << "NOD IZOLAT????\n";
        throw MiscError("Graful contine noduri izolate. Nu se poate realiza cautarea.\n");
        return {};
    }
    
    std::string aux; //pentru queue
    while(!q.isEmpty()) {
        aux = q.front();
        q.pop();

        if(adjList[aux].size() > 1) //DOAR DACA NODUL NU E FRUNZA!!
            for(auto& it : adjList[aux]) { //te uiti printre vecini
                if(visited.find(it.toStation) == visited.end()) { //DOAR DACA NU A FOST VIZITAT DEJA
                    //std::cout << it.toStation << "\n";
                    q.push(it.toStation);

                    aux1.routes = it.routes;
                    aux1.toStation = aux;
                    visited[it.toStation] = aux1;
                }   
            }
    }

    //recrearea solutiei
    std::vector<RouteEdge> solution;
    RouteEdge *auxpointer = &visited[end];
    aux1.toStation = end;

    while(auxpointer->toStation != "empty_station") {
        aux1.routes = auxpointer->routes;
        //solution.insert(solution.begin(), aux1); //pun in solutie LA INCEPUT routeEdge cu statia unde merge, si ruta folosita
        solution.push_back(aux1);

        aux1.toStation = auxpointer->toStation; //pun ruta de unde vin, ca sa o folosesc iar
        auxpointer = &visited[auxpointer->toStation];
    }
    std::reverse(solution.begin(), solution.end()); //este mai rapid sa dai reverse la final!
    return solution;
}


//---------------------------------------------------------------------------------------------------------
//------------------------------------------------MENIUL/INTERFATA---------------------------------------
//-----------------------------------------------------------------------------------------------------

void TicketManager::StartMenu() {
    bool tryAgain = true;
    int option;
    bool menuRunning = true;

    while(menuRunning) {
        tryAgain = true;
        while(tryAgain) {
            try {
                std::cout << "CFR - SISTEM DE CUMPARARE BILETE\n\n";

                std::cout<<"1 - Reteaua de Rute\n2 - Cumparare Bilet\n3 - Informatii Bilete\n";
                std::cout << "0 - Inchide\n";
                std::cout << ">";

                std::cin >> option;

                switch(option) {
                    case 0:
                        std::cout << "Se inchide...\n\n";
                        tryAgain = false;
                        menuRunning = false;
                        break;
                    case 1:
                        RoutesMenu();
                        tryAgain = false;
                        break;
                    case 2:
                        if(initialized)
                            buyTicket();
                        else
                            std::cout << "Nu exista rute in sistem.\n";
                        tryAgain = false;
                        break;
                    case 3:
                        TicketsInfo();
                        tryAgain = false;
                        break;
                    default:
                        throw OptionError(option);
                }
            }
            catch(const OptionError &e) {
                std::cout << "Exceptie: " << e.what() << "\n";
            }
        }
    }
}

void TicketManager::RoutesMenu() {
    bool running = true;
    while(running) {
        std::cout<<"1 - Reteaua de Rute\n";
        std::cout << "\t1 - Initializare Automata\n\t2 - Stergere Totala\n\t3 - Adaugare Manuala\n";
        std::cout << "\t4 - Afisare Graf Rute(lista de adiacenta)\n\t0 - Inapoi\n";

        int option;
        std::cout << ">";
        std::cin >> option;
        switch(option) {
            case 0:
                running = false;
                break;
            case 1:
                //FUNCTIE DE INITIALIZARE
                std::cout << "Momentan nu exista aceasta optiune!\n";
                //o sa o fac pana luni
                break;
            case 2:
                deleteGraph();
                break;
            case 3:
                populateTable();
                break;
            case 4:
                printGraph();
                std::cout << "\n";
                break;
            default:
                throw OptionError(option);
                running = false;
        }
    }
}

void TicketManager::deleteGraph() {
    if(initialized) {
        delete table;
        delete graph;

        table = new RouteTable();
        graph = new RouteGraph();
    }
    else
        std::cout << "Nu exista nimic de sters!\n";

}

void TicketManager::printGraph() {
    if(initialized) {
        graph->printGraph();
    }
    else
        std::cout << "Nu exista nimic de afisat!\n";
}

void TicketManager::TicketsInfo() {
    if(!initialized) {
        std::cout << "Nu exista rute in sistem.\n";
        return;
    }

    bool running = true;

    std::cout << "\n" << Ticket::ticketCount() << " bilete inregistrate in sistem.\n";
    for(auto it : tickets) {
        std::cout << it.first << "\t" << it.second->getName();
    }
    std::cout << "\n";

    while(running) {
        int id;
        std::cout << "Selectati un ID pentru a vedea informatii despre bilet.\n0 - Inapoi la meniul principal\n>";
        std::cin >> id;

        if(tickets.find(id) == tickets.end()) {
            throw OptionError(id);
            running = false;
        }
            
        if(id == 0)
            running = false;
        
        tickets[id]->printInfo();
    }
}