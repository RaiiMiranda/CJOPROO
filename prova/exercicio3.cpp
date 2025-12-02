// Prova: Raissa Pereira Miranda - CJ3028941
// Programa exercicio3.hpp
// Implementação da Classe

#include <iostream>
#include "exercicio2.hpp"

using namespace std;

// Construtor
//Veiculo::Veiculo() {}
Veiculo::Veiculo(std::string modelo, std::string cor, int anoFabricacao, float quilometragem) : modelo(modelo), cor(cor), anoFabricacao(anoFabricacao), quilometragem(quilometragem) {}

// Getters e Setters
void Veiculo::setModelo(string modelo) {
    this->modelo = modelo;
}

string Veiculo::getModelo() {
    return modelo;
}

void Veiculo::setCor(string cor) {
    this->cor = cor;
}

string Veiculo::getCor() {
    return cor;
}

void Veiculo::setAnoFabricacao(int anoFabricacao) {
    this->anoFabricacao = anoFabricacao;
}

int Veiculo::getAnoFabricacao() {
    return anoFabricacao;
}

void Veiculo::setQuilometragem(float quilometragem) {
    this->quilometragem = quilometragem;
}

float Veiculo::getQuilometragem() {
    return quilometragem;
}

void Veiculo::setConsumo(float consumo) {
    this->consumo = consumo;
}

float Veiculo::getConsumo() {
    return consumo;
}

// Método para Exibir os Dados
void Veiculo::exibirDados() {
    cout << "Modelo ...........: " << getModelo() << endl;
    cout << "Cor ..............: " << getCor() << endl;
    cout << "Ano Fabricacao ...: " << getAnoFabricacao() << endl;
    cout << "Quilometragem ....: " << getQuilometragem() << endl;
    cout << endl;
}

// Método para Calcular e Retornar Autonomia
// Autonomia = quantos quilômetros o veículo consegue andar com uma certa quantidade de combustível
// Autonomia (em km) = consumo (km por litro) × quantidade de combustível (litros)
string Veiculo::calcularAutonomia(float totalCombustivel) {
    float autonomia;

    autonomia = totalCombustivel / quilometragem;
    
    if (autonomia <= 350)
        return "autonomia ok";
    return "autonomia precisa melhorar";

}


