#include <QGuiApplication>       // cria a aplicação gráfica
#include <QQmlApplicationEngine> // carrega e executa os arquivos QML
#include <QQmlContext>           // permite passar objetos C++ para usá-los no QML
#include "Library.h"             // importa interface da classe Library

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv); // Cria a aplicação Qt
    QQmlApplicationEngine engine;    // Cria o mecanismo QML
    Library library;                 // Cria uma instância da classe Library

    engine.rootContext()->setContextProperty("library", &library);        // Expõe o objeto C++ para o QML
    engine.load(QUrl(QStringLiteral("qrc:/qt/qml/Biblioteca/Main.qml"))); // Carrega o arquivo QML inicial

    // Se a interface não carregou
    if(engine.rootObjects().isEmpty())
        // Retorna erro
        return -1;

    // Entra no looping principal da aplicação
    return app.exec();
}
