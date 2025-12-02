// Prova: Raissa Pereira Miranda - CJ3028941
// Programa exercicio2.hpp
// Interface da Classe

#ifndef EXERCICIO2_HPP
#define EXERCICIO2_HPP

#include <string>

class Veiculo
{
    private: 
        std::string modelo;
        std::string cor;
        int anoFabricacao;
        float quilometragem;
        float consumo;

    public:
        //explicit Veiculo();
        explicit Veiculo(std::string modelo, std::string cor, int anoFabricacao, float quilometragem);
        void setModelo(std::string modelo);
        std::string getModelo();
        void setCor(std::string cor);
        std::string getCor();
        void setAnoFabricacao(int anoFabricacao);
        int getAnoFabricacao();
        void setQuilometragem(float quilometragem);
        float getQuilometragem();
        void setConsumo(float consumo);
        float getConsumo();
        void exibirDados();
        std::string calcularAutonomia(float totalCombustivel);
};


#endif

