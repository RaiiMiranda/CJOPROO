// Prova: Raissa Pereira Miranda - CJ3028941
// Implementação do Programa Principal

// CMD: g++ prova-raissa.cpp rectangle.cpp -o prova-raissa

#include <iostream>
//#include <locale>
#include "rectangle.hpp"

using namespace std;

int main(void)
{
    //setlocale(LC_ALL, "Portuguese_Brazil.1252");

    Rectangle retangulo;

    int largura = 5, altura = 1;

    retangulo.setLargura(largura);
    retangulo.setAltura(altura);

    cout << ">> Prova P3" << endl;

    cout << "\n1. Dados do retangulo original:" << endl;
    cout << " - Largura: " << retangulo.getLargura() << endl;
    cout << " - Altura: " << retangulo.getAltura() << endl;
    cout << " - Area: " << retangulo.calcularArea(largura, altura) << endl;
    cout << " - Perimetro: " << retangulo.calcularPerimetro(largura, altura) << endl;

    cout << "\n# Retangulo renderizado:\n\n";
    retangulo.renderizarRetangulo(largura, altura);

    cout << "\n2. Informe as novas dimensoes:" << endl;
    cout << " - Largura: "; cin >> largura;
    cout << " - Altura: "; cin >> altura;
    cin.get();

    retangulo.setLargura(largura);
    retangulo.setAltura(altura);

    cout << "\n3. Dados do retangulo modificado:" << endl;
    cout << " - Largura: " << retangulo.getLargura() << endl;
    cout << " - Altura: " << retangulo.getAltura() << endl;
    cout << " - Area: " << retangulo.calcularArea(largura, altura) << endl;
    cout << " - Perimetro: " << retangulo.calcularPerimetro(largura, altura) << endl;

    cout << "\n. Retangulo renderizado:\n\n";
    retangulo.renderizarRetangulo(largura, altura);

    cout << "\nO retangulo foi deletado!";
    cin.get();
    return 0;
}
