#ifndef CARMANAGER_H
#define CARMANAGER_H

#include "Car.h"
#include <fstream>

class CarManager {
private:
    // --- STATIC REQUIREMENT: Fixed Array ---
    static const int MAX_CARS = 10;
    Car* cars[MAX_CARS];
    int carCount;

public:
    CarManager() {
        carCount = 0;
        for(int i = 0; i < MAX_CARS; i++) cars[i] = nullptr;
        loadFromFile();
    }

    ~CarManager() {
        for(int i = 0; i < carCount; i++) if(cars[i]) delete cars[i];
    }

    // Returns true if added, false if full
    bool addCar(string make, string model, double price, string engineType) {
        if (carCount >= MAX_CARS) return false;

        cars[carCount] = new Car(make, model, price, engineType);
        carCount++;
        saveToFile();
        return true;
    }

    void deleteCar(int index) {
        if (index < 0 || index >= carCount) return;

        delete cars[index];
        // Shift array logic
        for (int i = index; i < carCount - 1; i++) {
            cars[i] = cars[i + 1];
        }
        cars[carCount - 1] = nullptr;
        carCount--;
        saveToFile();
    }

    int getCount() const { return carCount; }

    Car* getCar(int index) const {
        if(index >= 0 && index < carCount) return cars[index];
        return nullptr;
    }

    void loadFromFile() {
        ifstream inFile("cars.txt");
        if (!inFile) return;

        string make, model, engineType;
        double price;

        for(int i=0; i<carCount; i++) delete cars[i];
        carCount = 0;

        while (carCount < MAX_CARS && getline(inFile, make)) {
            if(make.empty()) continue;
            getline(inFile, model);
            inFile >> price;
            inFile.ignore();
            getline(inFile, engineType); // Read Engine

            cars[carCount] = new Car(make, model, price, engineType);
            carCount++;
        }
        inFile.close();
    }

    void saveToFile() {
        ofstream outFile("cars.txt");
        for (int i=0; i<carCount; i++) {
            outFile << cars[i]->getMake() << endl;
            outFile << cars[i]->getModel() << endl;
            outFile << cars[i]->getPrice() << endl;
            outFile << cars[i]->getEngineType() << endl; // Save Engine
        }
        outFile.close();
    }
};

#endif
