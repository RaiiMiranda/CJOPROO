// Programa User.cpp
// Implementação da Classe User

#include "User.h"

// Construtor
User::User(QString n, QString e, QString p) : name(n), email(e), password(p) {}

// Métodos Getters
QString User::getName() { return name; }
QString User::getEmail() { return email; }

// Função que retorna quantidade atual
int User::getBorrowedBooks() { return borrowedBooks; }

// Função que verifica se pode pegar mais livros
bool User::canBorrow() { return borrowedBooks < 3; }

// Função para quando pega um livro
void User::incrementBorrowed() { borrowedBooks++; }

// Função para quando devolve um livro
void User::decrementBorrowed() { if(borrowedBooks>0) borrowedBooks--; }
