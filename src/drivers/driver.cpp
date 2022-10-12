# include <drivers/driver.h>
using namespace ackeros::drivers;

Driver::Driver() {
}

Driver::~Driver() {
}


void Driver::Activate() {
}

void Driver::Deactivate() {
}

int Driver::Reset() {
    return 0;
}

DriverManager::DriverManager() {
    numDrivers = 0;
}

void DriverManager::AddDriver(Driver* drv) {
    drivers[numDrivers++] = drv;
}

void DriverManager::ActivateAll() {
    for (int i = 0; i < numDrivers; ++i) {
        drivers[i]->Activate();
    }
}
