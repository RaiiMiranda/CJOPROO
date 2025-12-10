// Programa Library.h
// Interface da Classe Library

#ifndef LIBRARY_H
#define LIBRARY_H

#include <QObject>
#include <QVector>
#include <QDebug>
#include "User.h"
#include "Book.h"
#include "Librarian.h"

// Classe Biblioteca que herda de QObject
class Library : public QObject // para conseguir usar Q_INVOKABLE
{
    Q_OBJECT

    private:
        // Atributos privados
        QVector<Book> books;
        QVector<User> users;
        QVector<Librarian> librarians;
        User* loggedUser = nullptr;
        Librarian* loggedLibrarian = nullptr;

    public:
        // Métodos públicos
        explicit Library(QObject *parent = nullptr); // parent serve para gerenciar memória automaticamente

        // Métodos que o QML pode chamar diretamente
        Q_INVOKABLE bool addBook(QString title, QString author, QString desc);
        Q_INVOKABLE bool removeBook(int indexBook);
        Q_INVOKABLE void borrowBook(int indexBook);
        Q_INVOKABLE void returnBook(int indexBook);

        Q_INVOKABLE bool loginUser(QString name, QString email, QString password);
        Q_INVOKABLE bool loginLibrarian(QString name, QString email, QString password);
        Q_INVOKABLE bool isLibrarianLogged();
        Q_INVOKABLE void logout();

        Q_INVOKABLE int booksCount();
        Q_INVOKABLE QString bookTitle(int index);
        Q_INVOKABLE bool bookAvailable(int index);

        Q_INVOKABLE QString bookAuthor(int index);
        Q_INVOKABLE QString bookDescription(int index);
};

#endif // LIBRARY_H
