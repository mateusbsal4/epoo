#include "gtest/gtest.h"
#include <string>
using namespace std;
#include <math.h>

#include <Bateria.hpp>
#include <Drone.hpp>
#include <S1000.hpp>

class BateriaTest : public ::testing::Test {
    public:
        Bateria *bateria;
        const int CAPACIDADE_MAH = 1000;
        const int TEMPO_CARGA = 10;
    protected:
        void SetUp() override {
            bateria = new Bateria(
                CAPACIDADE_MAH,
                TEMPO_CARGA);
        }
};

TEST_F(BateriaTest, Construtor){
    ASSERT_EQ(BateriaTest::CAPACIDADE_MAH, bateria->getMah());
    ASSERT_EQ(BateriaTest::TEMPO_CARGA, bateria->getTempoDeCarregamento());
    ASSERT_EQ(0, bateria->getCarga());
    ASSERT_EQ(0, bateria->calculaTempoDeVoo());
}

TEST_F(BateriaTest, Carregar){
    bool statusCarregamento = false;

    statusCarregamento = bateria->carregar(0);
    ASSERT_TRUE(statusCarregamento);
    ASSERT_EQ(0, bateria->getCarga());

    statusCarregamento = bateria->carregar(BateriaTest::TEMPO_CARGA/2);
    ASSERT_TRUE(statusCarregamento);
    ASSERT_EQ(BateriaTest::CAPACIDADE_MAH/2, bateria->getCarga());

    statusCarregamento = bateria->carregar(BateriaTest::TEMPO_CARGA/2);
    ASSERT_TRUE(statusCarregamento);
    ASSERT_EQ(BateriaTest::CAPACIDADE_MAH, bateria->getCarga());

    statusCarregamento = bateria->carregar(BateriaTest::TEMPO_CARGA/2);
    ASSERT_FALSE(statusCarregamento);
    ASSERT_EQ(BateriaTest::CAPACIDADE_MAH, bateria->getCarga());
}

TEST_F(BateriaTest, Usar){
    bool statusUso = false;

    statusUso = bateria->usar(0);
    ASSERT_FALSE(statusUso);

    bateria->carregar(BateriaTest::TEMPO_CARGA);
    statusUso = bateria->usar(0);
    ASSERT_TRUE(statusUso);
    ASSERT_EQ(BateriaTest::CAPACIDADE_MAH, bateria->getCarga());

    statusUso = bateria->usar(BateriaTest::TEMPO_CARGA/2);
    ASSERT_TRUE(statusUso);
    ASSERT_EQ(
        BateriaTest::CAPACIDADE_MAH/2,
        bateria->getCarga());

    statusUso = bateria->usar(BateriaTest::TEMPO_CARGA/2);
    ASSERT_TRUE(statusUso);
    ASSERT_EQ(0, bateria->getCarga());

    statusUso = bateria->usar(BateriaTest::TEMPO_CARGA/2);
    ASSERT_FALSE(statusUso);
    ASSERT_EQ(0, bateria->getCarga());
}

TEST_F(BateriaTest, CalculaTempoDeVoo) {
    ASSERT_EQ(0, bateria->calculaTempoDeVoo());

    bateria->carregar(0);
    ASSERT_EQ(0, bateria->calculaTempoDeVoo());

    bateria->carregar(BateriaTest::TEMPO_CARGA/2);
    ASSERT_EQ(BateriaTest::TEMPO_CARGA/2, bateria->calculaTempoDeVoo());

    bateria->carregar(BateriaTest::TEMPO_CARGA/2);
    ASSERT_EQ(BateriaTest::TEMPO_CARGA, bateria->calculaTempoDeVoo());

    bateria->usar(BateriaTest::TEMPO_CARGA/2);
    ASSERT_EQ(BateriaTest::TEMPO_CARGA/2, bateria->calculaTempoDeVoo());
}

class DroneTest : public ::testing::Test {
    public:
        Bateria *bateria;
        Drone *drone;
        const string NOME = "Robinho";
        const double POSICAO_INICIAL = 5.0;
        const int ALTURA_DECOLAGEM = 3;
        const int CAPACIDADE_MAH = 1000;
        const int TEMPO_CARGA = 10;

        int tempoUsado(double deltaX, double deltaY){
            return sqrt(pow(deltaX, 2) + pow(deltaY, 2))/60;
        }

        int cargaUsada(double tempoUsado){
            return (CAPACIDADE_MAH * tempoUsado) / TEMPO_CARGA;
        }
    protected:
        void SetUp() override {
            bateria = new Bateria(
                CAPACIDADE_MAH,
                TEMPO_CARGA);
            bateria->carregar(TEMPO_CARGA);
            drone = new Drone(
                NOME,
                bateria,
                POSICAO_INICIAL);
        }
    bool statusTakeoff = false;
    bool statusSetPosition = false;
};

TEST_F(DroneTest, Construtor){
    ASSERT_EQ(DroneTest::NOME, drone->getNome());
    ASSERT_EQ(DroneTest::POSICAO_INICIAL, drone->getPosicao());
}

TEST_F(DroneTest, TakeoffNormal){
    drone->takeoff(DroneTest::ALTURA_DECOLAGEM);
    ASSERT_EQ(DroneTest::ALTURA_DECOLAGEM, drone->getAltura());
    ASSERT_TRUE(drone->getDecolado());
    ASSERT_FALSE(bateria->getCarregavel());
}

TEST_F(DroneTest, TakeoffSemBateria){
    bateria->usar(DroneTest::TEMPO_CARGA);
    drone->takeoff(DroneTest::ALTURA_DECOLAGEM);
    ASSERT_EQ(0, drone->getAltura());
    ASSERT_FALSE(drone->getDecolado());
    ASSERT_TRUE(bateria->getCarregavel());
}

TEST_F(DroneTest, Land){
    drone->takeoff(DroneTest::ALTURA_DECOLAGEM);
    int cargaAntesDoLand = bateria->getCarga();
    drone->land();
    ASSERT_EQ(cargaAntesDoLand, bateria->getCarga());
    ASSERT_FALSE(drone->getDecolado());
    ASSERT_EQ(0, drone->getAltura());
    ASSERT_TRUE(bateria->getCarregavel());
}

TEST_F(DroneTest, SetPositionSemBateria){
    drone->takeoff(DroneTest::ALTURA_DECOLAGEM);
    bateria->carregar(DroneTest::TEMPO_CARGA);
    bateria->usar(DroneTest::TEMPO_CARGA);
    statusSetPosition = drone->setPosition(POSICAO_INICIAL+5,5);
    ASSERT_FALSE(statusSetPosition);
    ASSERT_EQ(DroneTest::POSICAO_INICIAL, drone->getPosicao());
    ASSERT_EQ(DroneTest::ALTURA_DECOLAGEM , drone->getAltura());
    ASSERT_EQ(0, bateria->getCarga());
}


TEST_F(DroneTest, SetPositionSemDecolar){
    statusSetPosition = drone->setPosition(POSICAO_INICIAL+5,5);
    ASSERT_FALSE(statusSetPosition);
    ASSERT_EQ(DroneTest::POSICAO_INICIAL, drone->getPosicao());
    ASSERT_EQ(0, drone->getAltura());
    ASSERT_EQ(DroneTest::CAPACIDADE_MAH, bateria->getCarga());
}

TEST_F(DroneTest, SetPositionNormal){
    drone->takeoff(DroneTest::ALTURA_DECOLAGEM);
    bateria->carregar(DroneTest::TEMPO_CARGA);
    statusSetPosition = drone->setPosition(POSICAO_INICIAL+100, DroneTest::ALTURA_DECOLAGEM+100);
    ASSERT_EQ(DroneTest::POSICAO_INICIAL + 100, drone->getPosicao());
    ASSERT_EQ(DroneTest::ALTURA_DECOLAGEM + 100, drone->getAltura());
    ASSERT_TRUE(statusSetPosition);
    ASSERT_EQ(
        DroneTest::CAPACIDADE_MAH - cargaUsada(tempoUsado(100, 100)),
        bateria->getCarga());
}

TEST_F(DroneTest, SetPositionAlturaNegativa){
    drone->takeoff(DroneTest::ALTURA_DECOLAGEM);
    bateria->carregar(DroneTest::TEMPO_CARGA);
    statusSetPosition = drone->setPosition(POSICAO_INICIAL+5,-5);
    ASSERT_EQ(DroneTest::POSICAO_INICIAL, drone->getPosicao());
    ASSERT_EQ(DroneTest::ALTURA_DECOLAGEM, drone->getAltura());
    ASSERT_FALSE(statusSetPosition);
    ASSERT_EQ(DroneTest::CAPACIDADE_MAH, bateria->getCarga());
}

TEST_F(DroneTest, SetPositionAlturaNula){
    drone->takeoff(DroneTest::ALTURA_DECOLAGEM);
    bateria->carregar(DroneTest::TEMPO_CARGA);
    statusSetPosition = drone->setPosition(POSICAO_INICIAL+5, 0);
    ASSERT_EQ(DroneTest::POSICAO_INICIAL, drone->getPosicao());
    ASSERT_EQ(DroneTest::ALTURA_DECOLAGEM, drone->getAltura());
    ASSERT_FALSE(statusSetPosition);
    ASSERT_EQ(DroneTest::CAPACIDADE_MAH, bateria->getCarga());
}

TEST_F(DroneTest, SetPositionPosicaoNegativa){
    drone->takeoff(DroneTest::ALTURA_DECOLAGEM);
    bateria->carregar(DroneTest::TEMPO_CARGA);
    statusSetPosition = drone->setPosition(POSICAO_INICIAL-100,DroneTest::ALTURA_DECOLAGEM+100);
    ASSERT_EQ(DroneTest::POSICAO_INICIAL - 100, drone->getPosicao());
    ASSERT_EQ(DroneTest::ALTURA_DECOLAGEM + 100, drone->getAltura());
    ASSERT_TRUE(statusSetPosition);
    ASSERT_EQ(
        DroneTest::CAPACIDADE_MAH - cargaUsada(tempoUsado(-100, 100)),
        bateria->getCarga());
}

TEST_F(DroneTest, Mapear){
    ASSERT_FALSE(drone->mapear());
}

class S1000Test : public ::testing::Test {
    public:
        S1000 *s1000;
        Bateria *batS1000;
        const int CAPACIDADE_MAH = 22000;
        const int TEMPO_CARGA = 40;
        const int POS_S1000 = 0;
        const int ALTURA_S1000 = 10;
        const string NOME_S1000 = "S1000";
    protected:
        void SetUp() override {
            batS1000 = new Bateria(
                CAPACIDADE_MAH,
                TEMPO_CARGA);
            s1000 = new S1000(
                NOME_S1000,
                batS1000,
                POS_S1000
            );
        }
};

TEST_F(S1000Test, Construtor){
    ASSERT_EQ("S1000", s1000->getNome());
    ASSERT_EQ(S1000Test::POS_S1000, s1000->getPosicao());
}

TEST_F(S1000Test, Mapear){
    bool statusS1000 = false;
    batS1000->carregar(S1000Test::TEMPO_CARGA);
    s1000->takeoff(S1000Test::ALTURA_S1000);
    statusS1000 = s1000->mapear();
    ASSERT_TRUE(statusS1000);
}

TEST_F(S1000Test, MapearSemBateria){
    bool statusS1000 = false;
    s1000->takeoff(S1000Test::ALTURA_S1000);
    statusS1000 = s1000->mapear();
    ASSERT_FALSE(statusS1000);

}

TEST_F(S1000Test, MapearSemTakeoff){
    bool statusS1000 = false;
    batS1000->carregar(S1000Test::TEMPO_CARGA);
    statusS1000 = s1000->mapear();
    ASSERT_FALSE(statusS1000);
}
