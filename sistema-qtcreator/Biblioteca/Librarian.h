// Programa Librarian.h
// Interface da Classe Librarian

#ifndef LIBRARIAN_H
#define LIBRARIAN_H

// Classe Bibliotecário que herda de Usuário
class Librarian : public User
{
    public:
        // Método público
        Librarian(QString name, QString email, QString password) : User(name, email, password) {}
};

#endif // LIBRARIAN_H
