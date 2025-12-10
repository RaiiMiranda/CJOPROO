import QtQuick
import QtQuick.Controls

// Container base
Item {
    // Tamanho do container (tela dentro da janela)
    width: 800
    height: 600

    // ------------------------------------------------------------------------------------------------------------------------------------------------
    // -- LINHA SUPERIOR COM BOTÃO DE SAIR
    // ------------------------------------------------------------------------------------------------------------------------------------------------
    Row {
        spacing: 10
        anchors.top: parent.top
        anchors.topMargin: 20
        anchors.horizontalCenter: parent.horizontalCenter

        // Botão de sair
        Rectangle {
            width: 80
            height: 25
            radius: 80
            color: "#76604b"

            // Texto do botão
            Text {
                anchors.centerIn: parent
                text: "SAIR"
                font.bold: true
                color: "white"
            }

            // Transforma o retângulo em um elemento que reage a cliques e outros eventos do mouse
            MouseArea {
                anchors.fill: parent
                onClicked:
                {
                    library.logout()
                    console.log("Logout solicitado")
                    stack.pop()
                }
                cursorShape: Qt.PointingHandCursor
            }
        }
    }

    // ------------------------------------------------------------------------------------------------------------------------------------------------
    // -- ÁREA PRINCIPAL
    // ------------------------------------------------------------------------------------------------------------------------------------------------
    Column {
        anchors.top: parent.top
        anchors.topMargin: 70
        anchors.horizontalCenter: parent.horizontalCenter
        spacing: 10

        // ------------------------------------------------------------------------------------------------------------------------------------------------
        // -- CAMPOS PARA ADIÇÃO DE LIVROS (visível somente para bibliotecários)
        // ------------------------------------------------------------------------------------------------------------------------------------------------
        Column {
            spacing: 10
            visible: library.isLibrarianLogged()

            // Campos para preencher sobre o livro
            TextField { id: bookTitleField; placeholderText: "Título do livro"; width: 300 }
            TextField { id: bookAuthorField; placeholderText: "Autor"; width: 300 }
            TextField { id: bookDescField; placeholderText: "Descrição"; width: 300 }

            // Botão Cadastrar Livro
            Rectangle {
                width: 120
                height: 30
                radius: 6
                color: "#697b4c"
                Text { anchors.centerIn: parent; text: "Cadastrar Livro"; color: "white"; font.bold: true }

                // Transforma o retângulo em um elemento que reage a cliques e outros eventos do mouse
                MouseArea {
                    anchors.fill: parent
                    cursorShape: Qt.PointingHandCursor
                    onClicked:
                    {
                        // Se tiver um bibliotecário logado
                        if(library.isLibrarianLogged())
                        {
                            // Então, para ele cadastrar um livro é necessário preencher todos os campos
                            if(bookTitleField.text === "" || bookAuthorField.text === "" || bookDescField.text === "")
                            {
                                console.log("Preencha todos os campos do livro!")
                                return
                            }

                            // Se tiver todos os dados, então atualiza a lista de livros
                            if(library.addBook(bookTitleField.text, bookAuthorField.text, bookDescField.text))
                            {
                                booksList.model = library.booksCount()
                                bookTitleField.text = ""
                                bookAuthorField.text = ""
                                bookDescField.text = ""
                            }
                        // Caso contrário, não permite o cadastro de livros
                        }
                        else
                        {
                            console.log("Você precisa estar logado como bibliotecário")
                        }
                    }
                }
            }
        }

        // ------------------------------------------------------------------------------------------------------------------------------------------------
        // -- LISTA DE LIVROS
        // ------------------------------------------------------------------------------------------------------------------------------------------------
        ListView {
            id: booksList
            width: 600
            height: 400
            spacing: 10
            clip: true                  // elementos devem ser recortados nos limites da área visível
            model: library.booksCount() // define quantos itens o ListView vai criar

            // Retângulo para exibir os dados do livro cadastrado
            // delegate: define como cada item é exibido na lista
            delegate: Rectangle {
                width: booksList.width
                height: columnContent.implicitHeight + 30  // +30 pra margin bottom
                radius: 8
                color: "#eeeeee"
                border.color: "#cccccc"

                Column {
                    id: columnContent
                    anchors.fill: parent
                    anchors.margins: 10
                    spacing: 8

                    // Título do Livro
                    Text {
                        textFormat: Text.RichText
                        text: {
                            var title = library.bookTitle(index)
                            if (!library.bookAvailable(index)) {
                                title += " <font color='#800101'>(Emprestado)</font>"
                            }
                            return title
                        }
                        font.bold: true
                        font.pointSize: 12
                        color: "black"
                    }

                    // Autor do Livro
                    Text {
                        text: {
                            if (library.booksCount() > index) {
                                return library.bookAuthor(index)
                            } else {
                                return ""
                            }
                        }
                        color: "#555"
                    }

                    // Descrição do Livro
                    Text {
                        id: bookDescText
                        text: {
                            if (library.booksCount() > index) {
                                return library.bookDescription(index)
                            } else {
                                return ""
                            }
                        }
                        width: parent.width
                        font.pixelSize: 12
                        color: "#777"
                        wrapMode: Text.WordWrap
                    }

                    // Linha com botões de ação
                    Row {
                        spacing: 10

                        // Botão de Emprestar
                        Rectangle {
                            width: 90; height: 30; radius: 6
                            color: "#697b4c"
                            Text { anchors.centerIn: parent; text: "Emprestar"; color: "white"; font.bold: true }
                            MouseArea { anchors.fill: parent; onClicked: library.borrowBook(index); cursorShape: Qt.PointingHandCursor }
                        }

                        // Botão de Devolver
                        Rectangle {
                            width: 90; height: 30; radius: 6
                            color: "#a8b197"
                            Text { anchors.centerIn: parent; text: "Devolver"; color: "white"; font.bold: true }
                            MouseArea { anchors.fill: parent; onClicked: library.returnBook(index); cursorShape: Qt.PointingHandCursor }
                        }

                        // Botão Remover (apenas para o bibliotecário)
                        Rectangle {
                            width: 90; height: 30; radius: 6
                            color: "#c74c4c"
                            visible: library.isLibrarianLogged()
                            Text { anchors.centerIn: parent; text: "Remover"; color: "white"; font.bold: true }

                            MouseArea {
                                anchors.fill: parent
                                onClicked: {
                                    if(library.removeBook(index)) {
                                        booksList.model = library.booksCount()
                                    }
                                }
                                cursorShape: Qt.PointingHandCursor
                            }
                        }
                    }
                }
            }
        }
    }
}
