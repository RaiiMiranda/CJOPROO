// Programa User.h
// Interface da Classe User

#ifndef USER_H
#define USER_H

#include <QString>

// Classe Usuário
class User
{
    private:
        // Atributos privados
        int id;
        int borrowedBooks = 0; // livros emprestados
        QString name;
        QString email;
        QString password;

    public:
        // Métodos públicos
        User(QString name, QString email, QString password);

        QString getName();
        QString getEmail();

        int getBorrowedBooks();   // retorna quantidade atual
        bool canBorrow();         // verifica se pode pegar mais livros
        void incrementBorrowed(); // quando pega um livro
        void decrementBorrowed(); // quando devolve um livro
};

#endif // USER_H
