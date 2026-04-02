#pragma once

#include <string>
#include <vector>
#include <iostream>
#include <unordered_map>

class Car { // VEZI CA E CLASA ABSTRACTA ACUM !!
    protected:
        int capacity{0};

        double seat_price{10};

        std::vector<int> passengers;

    public:
        Car() = default;
        Car(int _capacity, double _price);
        //Car(const Car& other);
        
        //destructor virtual
        virtual ~Car() = default;

        // AFISARE
        virtual void print(std::ostream& out) const;

        //asta e friend, deci nu poate fi virtuala si folosita de alte clase pt ca nu apartine clasei!
        friend std::ostream& operator<<(std::ostream& out, const Car& c); 
        
        virtual double getPrice() const = 0; //se schimba la fiecare tip de vagon

        //FUNCTIE VIRTUALA PURA
        virtual std::string type() = 0;

        //FUNCTII UNIVERSALE PASAGERI
        void addPassenger(int _id, int _seat);
        void printAllPassengers();
        int occupyEmptySeat(int id);
};

class CouchetCar : public Car {
    int bed_nr{4};
    double bedsheet_price{15};

    public:
        CouchetCar() = default;
        CouchetCar(int _capacity, double _price, int _bed, double _bedsheet);
        CouchetCar(const CouchetCar& other);

        void print(std::ostream& out) const override;

        CouchetCar& operator=(const CouchetCar& other);
        //noua functie getprice pentru cuseta
        double getPrice() const override;

        std::string type() override;
};

class StandardCar : public Car {
    int car_class{2}, bike_seats{5};

    public:
        StandardCar() = default;
        StandardCar(int _capacity, double _price, int _class, int _bike);
        StandardCar(const StandardCar& other);

        StandardCar& operator=(const StandardCar& other);

        double getPrice() const override;

        std::string type() override;
};

//am nevoie de numarul vagonului si numarul locului
struct carInfo {
    int car_number = 0;
    int seat_number = 0;
};


class Train {
    int departure_time{0};
    int time_to_station{0};
    int cars_nr{0};
    
    std::vector<Car*> traincars{};
    std::vector<std::string> carOptions{};

    public:
        Train() = default;
        Train(int _dep, int _time);
        Train(const Train& other);

        ~Train();

        Train& operator=(const Train& other);

        void pushCar(Car* cart);
        void removeCars();

        void printTraincars();

        bool operator<(const Train& other); //pentru sortare

        int getDeparture() const { return departure_time; }
        int getSpeed() const { return time_to_station; }

        void mapCars(); //pune intr un vector toate tipurile de vagon disponibile
        void printOptions();
        std::string getCarOption(int option);

        carInfo occupyEmptySeat(std::string type, int id);
        bool isFull();

        double getPrice(std::string type) const;
};

class Route {
    private:
        std::vector<std::string> stations;
        std::vector<Train*> trains;

    public:
        Route() = default;
        Route(const Route& other);
        ~Route();

        //void textToStations(std::string text);
        void pushStation(std::string string);
        void printStations();

        void pushTrain(Train* train);
    
        std::string getRouteName() const;
        int hasStation(std::string station) const;

        const std::vector<std::string>& getStations() const { return stations; } //CONST REFERENCE E MAI EFICIENT!
        const std::vector<Train*>& getTrains() const { return trains; }

        void sortTrains();
        //void printTrains();

};

class Trip {
    static int trip_counter;
    int trip_id;

    std::string departure_time, arrival_time;
    std::string routeName;

    std::string start, end;

    //date de vagon
    int seat_nr;
    int car_nr;
    std::string car_type;

    double price;

    public:
        Trip(std::string dep, std::string arr, std::string routeName, std::string start, std::string end, int seat, int car, std::string type, double _price);
        std::string getDep() const { return departure_time; }
        std::string getArr() const { return arrival_time; }
        double getPrice() const { return price; }

        void printInfo() const;
};


class Ticket {
    static int ticket_counter;
    protected:
        std::vector<Trip*> trips;

        // DATE FURNIZATE DE CLIENT(TREBUIE CONSTRUITE)
        std::string name;
        std::string start, destination;

        // date create automat
        int ticket_id;
        double price{0};

    public:
        Ticket();
        Ticket(std::string _name);

        virtual ~Ticket();
        //nu am de ce sa fac copy constructor si assignment operator! cred

        virtual double getFinalPrice(double initial) = 0;


        void appendTrip(Trip* trip);
        void printTrips () const;

        virtual void printInfo () const = 0;

        int getId() const { return ticket_id; }
        std::string getName() const { return name; }

        void setTravel();
        void setPrice();

        static int ticketCount();
};

class DiscountTicket : virtual public Ticket {
    protected: double discount{0};
};

class FullPriceTicket : virtual public Ticket {
    protected: int id_card;

    public:
        FullPriceTicket() = default;
        FullPriceTicket(std::string _name, int _id);
        double getFinalPrice(double initial) override;

        void printInfo() const override;
};

class YouthTicket : public DiscountTicket {
    int student_id;
    public:
        YouthTicket(std::string _name, int _id);
        double getFinalPrice(double initial) override;

        void printInfo() const override;
};

class ElderlyTicket : public DiscountTicket {
    int health_card;

    public:
        ElderlyTicket(std::string _name, int _id);
        double getFinalPrice(double initial) override;

        void printInfo() const override;
};

class MembershipTicket : public FullPriceTicket, public DiscountTicket {
    std::string account_name;

    public:
        MembershipTicket(std::string _name, int _id, std::string _account);
        double getFinalPrice(double initial) override;

        void printInfo() const override;
};

class RouteTable {
    std::vector<Route*> routes;

    public:
        ~RouteTable();

        void appendRoute(Route* route);
        void printRoutes();

        const Route* findRoute(std::string name);
        const std::vector<Route*>& getRoutes() { return routes; }
};


/*
⠀⠀⠀⠀⠘⣿⡇⢀⢉⣙⣿⣿⣿⡇⠀⠀⠀⠀⠀⠀⠀⢠⠀⠀⠀⠀⠀⠀⢸⣿⣿⣿⣿⣛⣭⣭⣷⣶⣶⠌⠀⠀⠀⠀⠀⢀⣤⠎⠀⠀⠀⠀
⣄⣀⣀⣀⣀⣹⡿⠉⠉⠀⢩⣿⣿⡇⠀⠀⡇⠀⠀⠀⢸⡜⠀⠀⠀⠀⠀⠀⢸⣿⣿⣿⣿⣿⣿⣿⣿⣿⣏⠀⠀⠀⠀⠀⠀⡽⠁⠀⠀⠀⠀⠀
⣿⣿⣿⣿⣿⣿⠀⠀⠀⠀⡀⢸⣿⡇⠀⠀⡇⠀⠀⠀⡞⡇⠀⠀⠀⠀⠀⠀⢸⣿⣿⣿⣿⣿⣿⣿⣿⣿⠇⠀⠀⠀⠀⣀⠞⠀⠀⠀⠀⠀⠀⠀
⣿⣿⣿⣿⣿⣿⠀⠀⠀⠙⡦⣸⣭⢤⡉⠑⡧⠀⠀⠀⢧⠀⠀⠀⠀⠀⠀⢀⣾⣿⣿⣿⣿⣿⣿⣿⡟⠄⠀⠀⠀⠀⣼⢯⠀⠀⠀⠢⡀⠀⢠⠀
⣿⣿⣿⣿⣿⣿⠀⠀⠀⠀⠈⢣⣙⠦⣈⠢⡉⠀⠀⢠⢸⠖⢀⡄⣀⡤⠚⠁⣿⣿⣿⣿⣿⣿⣿⣿⡍⠀⠀⠀⢀⣸⠃⠈⠀⠀⠀⠀⠱⠀⢸⣿
⠿⣿⣿⣿⣿⣿⠁⠀⠀⠀⠢⢸⣿⡟⠗⢿⠙⠀⠀⢸⣾⡾⢏⠻⡌⠳⣆⠀⣿⣿⣿⣿⣿⣿⣿⡟⠀⠀⠀⠀⠢⡇⠀⠀⠀⠀⠀⠀⠀⠀⢸⠉
⠶⢿⣿⣿⣿⣿⠀⠀⠀⠀⠀⢸⣿⡇⠀⠘⡠⠀⢀⡀⣿⠧⠤⠷⠬⠦⣈⣳⣿⣿⣿⣿⣿⣿⡻⠁⠀⠀⢠⡲⠛⠀⠀⠀⠀⠀⠀⢀⡜⠀⣸⣀
⣸⡿⢿⣿⣿⣿⠀⠀⠄⠀⠀⢸⣿⣇⣠⡤⠿⠭⠤⢤⣀⠀⠀⠀⠀⠀⠀⠀⠀⠉⠙⠛⠿⠏⠀⠀⠀⢠⢳⣋⣉⣉⣉⣉⣉⢉⠩⠍⡉⣉⢉⡛
⣿⣧⣾⣿⣿⣿⣤⣤⣄⣄⣀⣜⡩⠋⠁⠀⠀⠀⠀⠀⠉⠀⠀⠀⢰⠞⠛⠋⠉⠙⠒⠢⢤⡀⠢⣀⢠⢯⠆⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠈⣿⡇
⢿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣤⣤⢤⣀⣀⣀⣀⡀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠈⠳⡀⠁⠸⡀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢀⣿⡇
⠈⠉⠉⠈⢉⢻⠛⠛⢛⣻⡏⢸⣿⣿⠀⠀⠀⠀⠉⡽⠁⠀⣶⣶⣦⣤⢄⣀⣀⣀⠀⠀⠀⠀⠀⠀⠀⠀⠱⡄⠀⠀⠀⠀⠀⠀⠀⠀⠀⢈⣿⡇
⣀⣀⣐⣐⣒⣸⠂⣤⣤⣩⠣⡸⣿⣿⡄⠀⢀⣠⠞⠀⠀⠀⢻⣿⣿⣿⡇⡈⠠⠭⠽⠙⠐⠒⠲⣤⠄⠀⠀⢡⠀⠀⠀⠀⠀⠀⠀⠀⠀⢸⣿⡇
⠉⠉⠉⠉⠉⢹⣿⣿⣿⢿⡀⠈⠉⠉⠓⠊⠉⠁⠀⠀⠀⠀⠈⠻⣿⣿⣷⠀⠀⠀⠀⠀⠀⢀⡰⠃⠀⠀⠀⢸⠀⠀⠀⠀⠀⠀⠀⠀⠀⢸⣿⡇
⢤⣀⣀⣠⣤⣼⣿⣿⣿⠀⡇⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠈⠉⠛⠧⠤⠤⠤⠤⠖⠋⠀⠀⠀⠀⠀⢸⠀⠀⠀⠀⠀⠀⠀⠀⠀⢸⣿⡇
⣿⣿⣿⣿⣿⣿⣿⣿⣿⠀⠸⡀⠀⠀⠀⣀⣀⣀⡀⠀⠀⠀⠀⢀⣀⡀⠀⢀⣀⣀⣀⡀⠠⢤⣴⠖⠀⠀⠀⡆⠀⠀⠀⠀⠀⠀⠀⠀⠀⢸⣿⡇
⣿⣿⣿⣿⣿⣿⣿⣟⣿⣶⣄⣱⣄⠀⠀⠙⠲⣄⠈⡏⠉⠉⠉⠁⢸⠈⠉⠉⠀⡘⠀⠀⢀⣼⡏⠀⠀⢀⣼⣁⣀⣀⣀⣀⣀⣀⣀⣀⣠⣼⣿⣷
⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣷⣄⠀⠀⠀⠉⠓⠤⢤⣀⣀⣠⣀⣀⣀⣤⣧⠴⠞⠋⠋⠀⠀⣠⣾⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿
⣿⣿⣿⣿⣿⣿⣿⡿⠿⠛⠛⠛⠛⠛⠉⠛⠢⢄⣀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⣀⡴⠚⠉⠀⠉⠉⠉⠉⠉⠉⠉⠉⠋⠉⠉⠉⠉⠁
⣩⣽⣿⣿⣿⣿⣿⣶⣶⣶⣶⣶⣶⣶⣶⣶⡶⠶⠿⠿⢿⣖⠠⣤⣄⣤⣤⣤⣤⣤⣴⣚⣋⣥⣤⣤⣤⣤⣤⣄⣀⣀⣀⣀⠤⠄⠠⠤⠤⢤⣤⣄
⠀⠂⠒⠒⠒⠒⠒⠒⠒⠒⠒⠒⠒⠚⠛⠉⠉⠉⠉⠉⠉⢼⠀⠀⠃⡌⠉⠙⠛⠛⠛⠭⠭⣍⣉⡉⠉⠉⠉⠉⠉⠉⠉⠙⠛⠛⠛⠛⠒⠒⠒⠒
⠓⠂⠤⢄⣀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢸⡏⠀⠀⠀⡇⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠈⠉⠁⠒⠒⠀⠤⠤⣤⣀⡤⠤⠤⠤⠤⠤
⠀⠀⠀⠀⠀⠈⠛⠒⠀⠀⠠⢤⣀⠀⠀⠀⠀⠀⠀⠒⡞⠁⠀⠀⠀⡗⠒⠒⠒⠛⠛⠉⠉⠉⠉⠉⠉⠉⠉⠁⠀⠀⠀⠀⠀⠉⠉⠓⠒⠂⠠⠤
⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠈⠉⠒⠢⠤⢀⣀⢇⠀⠀⠀⢸⠁⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀
⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢀⡠⢤⠖⠈⣩⠟⠊⠞⠀⠀⠀⠘⠆⣀⣀⡀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀
⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢀⠖⠁⡰⠁⢀⡾⠁⠀⠀⠀⠀⠀⠀⣠⠖⠁⠀⣩⠥⢦⡤⠤⠤⠤⠤⠐⠒⠒⠒⠒⠒⠒⠒⠀⠀⠀⠀⠉⠉⠉⠉
⠀⠀⠤⠤⠤⠖⠒⠒⢒⡲⠣⠂⡰⠃⢀⡾⠀⠀⠀⠀⠀⠀⠀⠞⠁⠀⢀⠞⠁⠀⠀⠈⠑⢶⣦⣤⣀⣀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀
*/

//clase folosite pentru aplicarea BFS printre rutele date

//queue ul meu
struct Node {
    Node* next = nullptr;
    std::string value{};
};

class Queue {
    Node* head = nullptr;
    Node* tail = nullptr;

    public:
        ~Queue();
        bool isEmpty();
        void push(std::string val);
        void pop();
        std::string front();
};

// pentru ca vreau si ruta de unde provine, nu doar statia la care ajunge
struct RouteEdge {
    std::vector<Route*> routes{};
    std::string toStation{"empty_station"};

    RouteEdge() = default;
    RouteEdge(std::string toStation);

};

class RouteGraph {
    // creez un graf din rutele date si memorez intr-o LISTA DE ADIACENTA
    // folosesc tipul de date UNORDERED MAP
    // fiecare muchie este un struct care memoreaza si ruta de pe care vine, intr un pointer
    // pentru a gasi ruta cea mai scurta, implementez algoritmul BFS
    // apoi recreez ruta si o afisez pentru client
    std::unordered_map<std::string, std::vector<RouteEdge>> adjList{};

    public:
        // te rog... SA REZOLVI problema cu trenurile si orele compatibile.
        // maine: bfs, recreerea solutiei, BILETE...
        void addRoute(Route& route);
        void addEdge(std::string st1, std::string st2, Route& route);
        void printGraph();

        int keyHasEdge(std::string key, std::string station);
        std::vector<RouteEdge> bfs(std::string start, std::string end);

        bool checkForNode(std::string node);

};


//obiectele 'raw' folosite in algoritm
struct rawTrack {
    int start = 0;
    int end = 0;
    std::string name = ""; //sa NU pui const daca vrei sa sortezi


    rawTrack() = default;
    rawTrack(int _start, int _end, std::string _name) : start(_start), end(_end), name(_name) {}

    bool operator<(const rawTrack& other) { //PENTRU SORT
        return start < other.start;
    }
};

class TicketManager {
    static TicketManager* instance;
    TicketManager();

    bool initialized = false;

    //salvate in memorie
    RouteGraph* graph = nullptr;
    RouteTable* table = nullptr;
    std::unordered_map<int, Ticket*> tickets;

    Train* highSpeedTrain();
    Train* regionalTrain();
    Car* manualAddCar();
    Train* manualAddTrain();

    Ticket* buyFullPrice();
    Ticket* buyYouth();
    Ticket* buyElderly();
    Ticket* buyMembership();
    
    std::vector<Train*> options(const std::vector<RouteEdge>& solution, const std::vector<rawTrack>& vec);

    public:
        static TicketManager* getInstance() {
            if(!instance) instance = new TicketManager();
            return instance;
        }
        ~TicketManager();
        
        void populateTable();
        void printRouteGraph();

        void routeSearch(Ticket* ticket);

        void buyTicket();

        bool checkForNode(std::string node);

        void StartMenu();
        void RoutesMenu();
        void deleteGraph();
        void TicketsInfo();
        void printGraph();
};



