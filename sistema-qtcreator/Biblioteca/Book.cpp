// Programa Book.cpp
// Implementação da Classe Book

#include "Book.h"
#include <QDebug> // para qWarning, qDebug

// Construtor
Book::Book(QString title, QString author, QString description) : title(title), author(author), description(description), status(true), borrowedBy(nullptr) {}

// Métodos Getters
QString Book::getTitle() { return title; }
QString Book::getAuthor() { return author; }
QString Book::getDescription() { return description; }

// Função para saber se o livro está disponível
bool Book::isAvailable() { return status; }

// Função para emprestar o livro
void Book::borrowBook(User* user)
{
    // Se está disponível
    if(status)
    {
        // Muda o status para emprestado e salva o usuário que emprestou
        status = false;
        borrowedBy = user;
    }
    // Se não está disponível
    else
    {
        qWarning() << "Livro já emprestado!";
    }
}

// Função para devolver o livro
void Book::returnBook(User* user)
{
    // Só o usuário que pegou pode devolver
    if(!status && borrowedBy == user)
    {
        status = true;
        borrowedBy = nullptr;
    }
    else
    {
        qWarning() << "Você não pode devolver este livro!";
    }
}

// Função para saber qual usuário emprestou o livro
User* Book::getBorrowedBy() { return borrowedBy; }
