// Prova: Raissa Pereira Miranda - CJ3028941
// Interface da Classe

#ifndef RECTANGLE_HPP
#define RECTANGLE_HPP

class Rectangle 
{
    private:
        int largura;
        int altura;

    public:
        void setLargura(int largura);
        int getLargura();
        void setAltura(int altura);
        int getAltura();
        
        int calcularArea(int largura, int altura);
        int calcularPerimetro(int largura, int altura);
        void renderizarRetangulo(int largura, int altura);
};

#endif