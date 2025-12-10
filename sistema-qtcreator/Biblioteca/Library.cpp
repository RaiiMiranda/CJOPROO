// Programa Library.cpp
// Implementação da Classe Library

#include "Library.h"
#include <QDebug> // para qWarning, qDebug

// ------------------------------------------------------------------------------------------------------------
// -- Construtor
// ------------------------------------------------------------------------------------------------------------
Library::Library(QObject *parent) : QObject(parent)
{
    // Inicializando a biblioteca com alguns usuários e bibliotecário predefinidos
    users = QVector<User>{
        User("João", "joao@gmail.com", "1234"),
        User("Ana", "ana@gmail.com", "1234")
    };

    librarians = QVector<Librarian>{
        Librarian("Maria", "maria@gmail.com", "1234")
    };
}


// ------------------------------------------------------------------------------------------------------------
// -- Função para adicionar um livro (apenas bibliotecário)
// ------------------------------------------------------------------------------------------------------------
bool Library::addBook(QString title, QString author, QString desc)
{
    // Se não tiver bibliotecário logado
    if (loggedLibrarian == nullptr)
    {
        // Então não permite cadastrar livros
        qWarning() << "Permissão negada: apenas bibliotecários podem cadastrar livros.";
        return false; // livro não foi adicionado
    }

    // Padroniza o texto para nao cadastrar "Harry Potter" e "harry potter"
    // trimmed(): evita espaços a mais
    // toLowe(): evita diferença de maiúsculas/minúsculas
    QString newTitle = title.trimmed().toLower();

    // Loop que percorre todos os livros já cadastrados
    for (Book &b : books)
    {
        // Se já existe um livro com esse mesmo título
        if (b.getTitle().trimmed().toLower() == newTitle)
        {
            // Então não permite cadastrar o livro
            qWarning() << "Livro já cadastrado!";
            return false; // livro não foi adicionado
        }
    }

    // Cria um objeto Book com os dados
    Book book(title, author, desc);

    // Cadastra o livro na lista de livros
    books.append(book);

    // Mensagem no console
    qDebug() << "Livro adicionado:" << book.getTitle();
    return true; // livro foi adicionado
}


// ------------------------------------------------------------------------------------------------------------
// -- Função para remover o livro
// ------------------------------------------------------------------------------------------------------------
bool Library::removeBook(int indexBook)
{
    // Se não for um bibliotecário logado
    if (!isLibrarianLogged())
    {
        // Entõa não permite remover livros
        qWarning() << "Permissão negada: apenas bibliotecário pode remover livros.";
        return false;
    }

    // Se// Se o índice do livro não for válido
    if (indexBook < 0 || indexBook >= books.size())
    {
        // Então não permite remover o livro
        qWarning() << "Índice inválido!";
        return false;
    }

    // Se o livro não estiver disponível
    if (!books[indexBook].isAvailable())
    {
        // Então não permite remover o livro
        qWarning() << "Não é possível remover um livro que está emprestado!";
        return false;
    }

    // Se passar por todos os 'if' então permite remover o livro
    QString title = books[indexBook].getTitle();
    books.removeAt(indexBook);
    qDebug() << "Livro removido:" << title;
    return true;
}


// ------------------------------------------------------------------------------------------------------------
// -- Função para emprestar um livro
// ------------------------------------------------------------------------------------------------------------
void Library::borrowBook(int indexBook)
{
    // Se não tiver usuário logado
    if(!loggedUser)
    {
        // Então não permite o empréstimo do livro
        qWarning() << "Nenhum usuário logado!";
        return;
    }

    // Se o usuário logado não pode emprestar
    if (!loggedUser->canBorrow())
    {
        // Então não permite ele passar o limite de livros emprestados
        qWarning() << "Você atingiu o limite de 3 livros!";
        return;
    }

    // Se o índice do livro não for válido (não pode ser negativo
    // e não pode ser maior que a quantidade de livros cadastrados)
    if(indexBook < 0 || indexBook >= books.size())
    {
        // Então não permite o empréstimo do livro
        qWarning() << "Índice de livro inválido!";
        return;
    }

    // Se o livro não estiver disponível
    if(!books[indexBook].isAvailable())
    {
        // Então não permite o empréstimo do livro
        qWarning() << "Livro já emprestado!";
        return;
    }

    // Se passar por todos os 'if' entõa permite emprestar o livro
    books[indexBook].borrowBook(loggedUser);                        // Registra o usuário que pegou o livro
    loggedUser->incrementBorrowed();                                // Incrementa a quantidade de livros emprestados desse usuário
    qDebug() << "Livro emprestado:" << books[indexBook].getTitle(); // Mensagem no console
}


// ------------------------------------------------------------------------------------------------------------
// -- Função para devolver um livro
// ------------------------------------------------------------------------------------------------------------
void Library::returnBook(int indexBook)
{
    // Se não tiver usuário logado
    if(!loggedUser)
    {
        // Então não permite devolver livro
        qWarning() << "Nenhum usuário logado!";
        return;
    }

    // Se o índice do livro não for válido (não pode ser negativo
    // e não pode ser maior que a quantidade de livros cadastrados)
    if(indexBook < 0 || indexBook >= books.size())
    {
        // Então não permite devolver o livro
        qWarning() << "Índice de livro inválido!";
        return;
    }

    // Se o livro estiver disponível
    if(books[indexBook].isAvailable())
    {
        // Então não faz sentido devolver o livro
        qWarning() << "Este livro não está emprestado!";
        return;
    }

    // Se a pessoa que pegou o livro não for a mesma que está logada
    if(books[indexBook].getBorrowedBy() != loggedUser)
    {
        // Então não permite um usuário devolver um livro que não emprestou
        qWarning() << "Você não pode devolver este livro, pois não foi você quem o pegou!";
        return;
    }

    // Se passou por todas as verificações, então o livro pode ser devolvido
    // Marca o livro como disponível novamente
    books[indexBook].returnBook(loggedUser);

    // Diminui a quantidade de livros emprestados pelo usuário
    loggedUser->decrementBorrowed();

    // Mensagem no console
    qDebug() << "Livro devolvido:" << books[indexBook].getTitle();
}


// ------------------------------------------------------------------------------------------------------------
// -- Função para login do usuário comum
// ------------------------------------------------------------------------------------------------------------
bool Library::loginUser(QString name, QString email, QString password)
{
    // Se tiver usuário logado OU bibliotecário logado
    if (loggedUser != nullptr || loggedLibrarian != nullptr)
    {
        // Então não permite fazer login antes do logout
        qWarning() << "Já existe um usuário logado! Faça logout antes de logar outro.";
        return false; // não fez login
    }

    // Percorre todos os usuários cadastrados
    for(User &u : users)
    {
        // Se o nome e o email batem com algum usuário
        if(u.getName() == name && u.getEmail() == email)
        {
            loggedUser = &u;                        // marca ele como logado
            loggedLibrarian = nullptr;              // garante que nao tenha biblioteca logado
            qDebug() << "Usuário logado:" << name;  // mensagem de sucesso no console
            return true;                            // login realizado
        }
    }

    // Se não achou o usuário, então não permite o login
    qWarning() << "Usuário não encontrado!";
    return false; // não fez login
}


// ------------------------------------------------------------------------------------------------------------
// -- Função para login do bibliotecário
// ------------------------------------------------------------------------------------------------------------
bool Library::loginLibrarian(QString name, QString email, QString password)
{
    // Se tiver usuário logado OU bibliotecário logado
    if (loggedUser != nullptr || loggedLibrarian != nullptr)
    {
        // Então não permite fazer login antes do logout
        qWarning() << "Já existe um usuário logado! Faça logout antes de logar outro.";
        return false; // não fez login
    }

    // Percorre todos os bibliotecários cadastrados
    for(Librarian &l : librarians)
    {
        // Se o nome e o email batem com algum usuário
        if(l.getName() == name && l.getEmail() == email)
        {
            loggedLibrarian = &l;                        // marca ele como logado
            loggedUser = nullptr;                        // garante que nao tenha usuário comum logado
            qDebug() << "Bibliotecário logado:" << name; // mensagem de sucesso no console
            return true;                                 // login realizado
        }
    }

    // Se não achou o bibliotecário, então não permite o login
    qWarning() << "Bibliotecário não encontrado!";
    return false; // não fez login
}


// ------------------------------------------------------------------------------------------------------------
// -- Função para saber o usuário atual é o bibliotecário
// ------------------------------------------------------------------------------------------------------------
bool Library::isLibrarianLogged()
{
    // Retorna true se tiver bibliotecário logado
    return loggedLibrarian != nullptr;
}


// ------------------------------------------------------------------------------------------------------------
// -- Função para deslogar usuário ou bibliotecário
// ------------------------------------------------------------------------------------------------------------
void Library::logout()
{
    // Se não tiver usuário ou bibliotecário logado
    if(loggedUser == nullptr && loggedLibrarian == nullptr)
    {
        // Então não permite deslogar
        qWarning() << "Não há usuário para deslogar!";
        return;
    }

    // Se tiver usuário ou bibliotecário logado, então desloga ele
    loggedUser = nullptr;
    loggedLibrarian = nullptr;
    qDebug() << "Logout realizado";
}


// ------------------------------------------------------------------------------------------------------------
// -- Funções para desenhar os livros no .qml
// ------------------------------------------------------------------------------------------------------------
int Library::booksCount()
{
    // Retorna a quantidade total de livros cadastrados
    // para saber quantos livros exibir na interface
    return books.size();
}

QString Library::bookTitle(int index)
{
    // Retorna o título do livro que está em uma determinada posição (índice) da lista
    // para mostrar o nome do livro na interface
    return books[index].getTitle();
}

bool Library::bookAvailable(int index)
{
    // Retorna se o livro está disponível para empréstimo ou não
    // true  - está disponível
    // false - está emprestado
    return books[index].isAvailable();
}


// ------------------------------------------------------------------------------------------------------------
// -- Funções para autor e descrição do livro
// ------------------------------------------------------------------------------------------------------------
QString Library::bookAuthor(int index)
{
    // Retorna o autor do livro que está na posição 'index'
    return books[index].getAuthor();
}

QString Library::bookDescription(int index)
{
    // Retorna a descrição do livro que está na posição 'index'
    return books[index].getDescription();
}
