import QtQuick
import QtQuick.Controls

// Container base
Item {
    // Sinal que será emitido quando o login for bem-sucedido
    signal loginSuccess()

    // Tamanho do container (tela dentro da janela)
    width: 800
    height: 600

    // Container principal com os elementos empilhados verticalmente
    Column {
        anchors.centerIn: parent
        spacing: 10
        anchors.horizontalCenter: parent.horizontalCenter

        // ---------------------------------------------------------------------------------------------------------
        // -- TÍTULOS
        // ---------------------------------------------------------------------------------------------------------
        // Título da Biblioteca
        Text {
            text: "ECO-READS LIBRARY"
            font.bold: true
            font.pointSize: 28
            color: "#333"
            horizontalAlignment: Text.AlignHCenter
            anchors.horizontalCenter: parent.horizontalCenter
        }

        // Subtítulo Login
        Text {
            text: "Login"
            font.bold: true
            font.pointSize: 20
            color: "#555"
            horizontalAlignment: Text.AlignHCenter
            anchors.horizontalCenter: parent.horizontalCenter
        }

        // ---------------------------------------------------------------------------------------------------------
        // -- CAMPOS DE LOGIN
        // ---------------------------------------------------------------------------------------------------------
        Column {
            spacing: 10
            anchors.horizontalCenter: parent.horizontalCenter

            // Campos de login
            TextField { id: loginName; placeholderText: "Nome"; width: 200 }
            TextField { id: loginEmail; placeholderText: "Email"; width: 200 }
            TextField { id: loginPassword; placeholderText: "Senha"; width: 200 }

            // Linha contendo os botões
            Row {
                spacing: 10
                anchors.horizontalCenter: parent.horizontalCenter

                // Botão de login
                Rectangle {
                    width: 80
                    height: 25
                    radius: 80
                    color: "#697b4c"

                    // Texto dentro do botão
                    Text {
                        anchors.centerIn: parent
                        text: "LOGIN"
                        font.bold: true
                        color: "white"
                    }

                    // Área que detecta o clique do mouse
                    MouseArea {
                        anchors.fill: parent
                        onClicked:
                        {
                            // Tentativa de login como usuário
                            if (library.loginUser(loginName.text, loginEmail.text, loginPassword.text))
                            {
                                console.log("Usuário logado com sucesso")
                                loginSuccess()
                                return
                            }

                            // Tentativa de login como bibliotecário
                            if (library.loginLibrarian(loginName.text, loginEmail.text, loginPassword.text))
                            {
                                console.log("Bibliotecário logado com sucesso")
                                loginSuccess()
                                return
                            }

                            // Nenhum login deu certo
                            console.log("Falha no login (nenhum usuário encontrado)")
                        }

                        // Muda o cursor para pointer, quando passa o mouse por cima do botão
                        cursorShape: Qt.PointingHandCursor
                    }
                }
            }
        }
    }
}
