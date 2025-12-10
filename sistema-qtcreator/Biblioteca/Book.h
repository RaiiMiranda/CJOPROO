// Programa Livro.h
// Interface da Classe Book

#ifndef BOOK_H
#define BOOK_H

#include <QString>
#include "User.h"

// Classe Livro
class Book
{
    private:
        // Atributos privados
        int id;
        QString title;
        QString author;
        QString description;
        bool status;                // true = disponível
        User* borrowedBy = nullptr; // atributo para saber qual usuário pegou o livro

    public:
        // Métodos públicos
        Book(QString title, QString author, QString description);

        QString getTitle();
        QString getAuthor();
        QString getDescription();
        bool isAvailable();

        void borrowBook(User* user);
        void returnBook(User* user);

        User* getBorrowedBy();
};

#endif // BOOK_H
