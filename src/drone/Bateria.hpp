#ifndef BATERIA_H
#define BATERIA_H

class Bateria {
public:
    Bateria(int mah, int tempoDeCarregamento);
    ~Bateria();
    bool carregar(int tempo);
    bool usar(int tempo);
    int calculaTempoDeVoo();

    int getCarga();
    int getTempoDeCarregamento();
    int getMah();
    bool getUso();
    bool getCarregavel();

    void setUso(bool uso);
    void setCarregavel(bool carregavel);
    void status();
};

#endif
