#include "cfr.h"
#include <string>
#include <vector>
#include <iostream>

int main() {
    
    TicketManager* menu = TicketManager::getInstance();
    menu->StartMenu();

    return 0;
}