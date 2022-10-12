#ifndef ACKEROS_DRIVERS_DRIVER_H_
#define ACKEROS_DRIVERS_DRIVER_H_

namespace ackeros {
    namespace drivers {
        class Driver {
            public:
                Driver();
                ~Driver();

                virtual void Activate();
                virtual int Reset();
                virtual void Deactivate();
        };

        class DriverManager {
           public:
                Driver* drivers[265];
                int numDrivers;

            public:
                DriverManager();
                void AddDriver(Driver*);
                void ActivateAll();
        };
    }
}

#endif
