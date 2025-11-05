// Prova: Raissa Pereira Miranda - CJ3028941
// Implementação do Programa Principal

// CMD: g++ prova-raissa.cpp exercicio3.cpp -o prova-raissa

#include <iostream>
#include "exercicio2.hpp"

using namespace std;

int main(void)
{
    float totalCombustivel;

    //Veiculo veiculo1("Modelo1", "Branco", 2000, 10000);
    //Veiculo veiculo2("Modelo2", "Rosa", 2000, 10000);
    //Veiculo veiculo3("Modelo3", "Vermelho", 2000, 10000);
    //Veiculo veiculo4("Modelo4", "Prata", 2000, 10000);
    //Veiculo veiculo5("Modelo5", "Preto", 2000, 10000);
    Veiculo veiculo1;
    Veiculo veiculo2;
    Veiculo veiculo3;
    Veiculo veiculo4;
    Veiculo veiculo5;

    veiculo1.setModelo("Modelo1");
    veiculo1.setCor("Cor1");
    veiculo1.setAnoFabricacao(1900);
    veiculo1.setQuilometragem(10000);

    veiculo2.setModelo("Modelo2");
    veiculo2.setCor("Cor2");
    veiculo2.setAnoFabricacao(1900);
    veiculo2.setQuilometragem(10000);

    veiculo3.setModelo("Modelo3");
    veiculo3.setCor("Cor3");
    veiculo3.setAnoFabricacao(1900);
    veiculo3.setQuilometragem(10000);

    veiculo4.setModelo("Modelo4");
    veiculo4.setCor("Cor4");
    veiculo4.setAnoFabricacao(1900);
    veiculo4.setQuilometragem(10000);

    veiculo5.setModelo("Modelo5");
    veiculo5.setCor("Cor5");
    veiculo5.setAnoFabricacao(1900);
    veiculo5.setQuilometragem(10000);

    Veiculo vetorVeiculos[5];
    vetorVeiculos[0] = veiculo1;
    vetorVeiculos[1] = veiculo2;
    vetorVeiculos[2] = veiculo3;
    vetorVeiculos[3] = veiculo4;
    vetorVeiculos[4] = veiculo5;

    // Demonstração Utilização das Funcionalidades
    cout << "\n-----------------------------------------------------------------------------------------------------" << endl;
    cout << "EXIBICAO DOS DADOS INICIAIS\n";
    cout << "-----------------------------------------------------------------------------------------------------\n" << endl;
    
    veiculo1.exibirDados();
    veiculo2.exibirDados();
    veiculo3.exibirDados();
    veiculo4.exibirDados();
    veiculo5.exibirDados();

    cout << "\n-----------------------------------------------------------------------------------------------------" << endl;
    cout << "ALTERANDO INFORMACOES\n";
    cout << "-----------------------------------------------------------------------------------------------------\n" << endl;
    veiculo1.setModelo("Corsa");
    cout << "Modelo veiculo 1 ...........: " << veiculo1.getModelo() << endl;

    veiculo2.setCor("Roxo");
    cout << "Cor veiculo 2 ..............: " << veiculo2.getCor() << endl;

    veiculo3.setAnoFabricacao(1900);
    cout << "Ano fabricacao veiculo 3 ...: " << veiculo3.getAnoFabricacao() << endl;

    veiculo4.setQuilometragem(10.000);
    cout << "Quilometragem veiculo 4 ....: " << veiculo4.getQuilometragem() << endl;

    cout << "\n-----------------------------------------------------------------------------------------------------" << endl;
    cout << "APRESENTANDO A AUTONOMIA\n";
    cout << "-----------------------------------------------------------------------------------------------------\n" << endl;

    cout << "Veiculo 1: " << veiculo1.calcularAutonomia(30) << endl;
    cout << "Veiculo 2: " << veiculo2.calcularAutonomia(30) << endl;
    cout << "Veiculo 3: " << veiculo3.calcularAutonomia(30) << endl;
    cout << "Veiculo 4: " << veiculo4.calcularAutonomia(30) << endl;
    cout << "Veiculo 5: " << veiculo5.calcularAutonomia(30) << endl;
    
    cout << "\nTecle <Enter> para encerrar...";
    cin.get();
    return 0;
}
