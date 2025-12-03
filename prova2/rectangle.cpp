// Prova: Raissa Pereira Miranda - CJ3028941
// Implementação da Classe

#include <iostream>
#include "rectangle.hpp"

using namespace std;

void Rectangle::setLargura(int largura) {
    this->largura = largura;
}

int Rectangle::getLargura() {
    return largura;
}

void Rectangle::setAltura(int altura) {
    this->altura = altura;
}

int Rectangle::getAltura() {
    return altura;
}

int Rectangle::calcularArea(int largura, int altura) 
{
    int area = largura * altura;
    return area;
}

int Rectangle::calcularPerimetro(int largura, int altura) 
{
    int perimetro = largura * 2 + altura * 2;
    return perimetro;
}

void Rectangle::renderizarRetangulo(int largura, int altura) 
{
    for (int i = 1; i <= altura; i++)
    {
        for(int j = 1; j <= largura; j++)
        {
            // se for a primeira ou ultima linha, entao desenha a linha toda com *
            if (i == 1 || i == altura)
                cout << " * ";
            // se nao for a primeira ou ultima linha
            // se j for 1 ou j for o tamanho da largura
            else if (j == 1 || j == largura)
                cout << " * ";
            else
                cout << "   ";
        }
            
        cout << endl;
    }
}