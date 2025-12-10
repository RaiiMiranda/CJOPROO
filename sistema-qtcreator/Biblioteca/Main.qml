import QtQuick
import QtQuick.Controls

// Container principal que abre a janela
Window {
    width: 800
    height: 600
    visible: true
    title: qsTr("Biblioteca")

    // Para navegação
    StackView {
        id: stack
        anchors.fill: parent

        // Define LoginScreen como tela inicial
        initialItem: LoginScreen {
            // Quando obtém sucesso no login
            onLoginSuccess: {
                // Então troca para a tela principal
                stack.push(Qt.resolvedUrl("MainScreen.qml"))
            }
        }
    }
}
