#ifndef DRONE_H
#define DRONE_H

#include "Bateria.hpp"
#include <string>
using namespace std;

class Drone {
public:
    Drone(string nome, Bateria* bateria, double posicao);
    virtual ~Drone();

    void takeoff(int altura);
    bool setPosition(double x, double y);
    void land();
    virtual bool mapear();
    void status();

    string getNome();
    Bateria* getBateria();
    double getPosicao();
    double getAltura();
    bool getDecolado();
};

#endif
