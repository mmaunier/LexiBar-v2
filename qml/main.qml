import QtQuick
import QtQuick.Controls
import QtQuick.Window

Window {
    id: mainWindow
    visible: true
    title: qsTr("LexiBar v2")
    width: 378
    height: isWayland ? 29 : 29
    
    // Utiliser des flags qui préservent la barre de titre
    flags: Qt.Window | Qt.WindowStaysOnTopHint

    // Détection dynamique de Wayland vs X11
    readonly property bool isWayland: Qt.platform.pluginName.toLowerCase().includes("wayland")

    // Timer pour vérifier périodiquement si la fenêtre est active 
    // et la ramener au premier plan (solution pour Wayland)
    Timer {
        id: activationTimer
        interval: 1000 // Vérifier toutes les secondes
        running: isWayland && visible
        repeat: true
        onTriggered: {
            if (!mainWindow.active) {
                // Technique 1: Activer la fenêtre
                mainWindow.requestActivate();
                
                // Technique 2: Forcer la visibilité
                if (!mainWindow.visible) {
                    mainWindow.visible = true;
                }
            }
        }
    }

    Component.onCompleted: {
        console.log("Environnement détecté: " + Qt.platform.pluginName);
        console.log("Utilisation de la hauteur: " + height + "px");
        
        // Positionner en haut de l'écran
        x = (Screen.width - width) / 2;
        y = 0;
        
        // // Sous Wayland, positionner dans un coin spécifique peut aider
        // if (isWayland) {
        //     // Position en haut à droite
        //     x = Screen.width - width;
        //     y = 0;
        // }
    }

    // Couleur de fond
    color: "#f0f0f0"

    // Empêcher le redimensionnement
    minimumWidth: width
    maximumWidth: width
    minimumHeight: height
    maximumHeight: height

    // Rangée de boutons
    Row {
        id: characterRow
        // Positionnement exact sans utiliser anchors.fill
        x: 0
        y: 0
        width: parent.width
        height: 20 // Hauteur exacte des boutons
        spacing: 0 // Pas d'espace entre les boutons

        Repeater {
            model: characterModel

            Rectangle {
                id: characterButton
                width: 27
                height: 29
                color: "#e0e0e0"
                border.color: "#c0c0c0"
                border.width: 1

                Text {
                    anchors.centerIn: parent
                    text: model.character
                    color: model.color
                    font.bold: true
                    font.pixelSize: 15
                }

                MouseArea {
                    id: mouseArea
                    anchors.fill: parent
                    acceptedButtons: Qt.LeftButton

                    property point dragStart

                    onClicked: {
                        lexibar.copyToClipboard(model.character)
                        animation.start()
                    }

                    onPressed: function(mouse) {
                        characterButton.color = "#d0d0d0"
                        dragStart = Qt.point(mouse.x, mouse.y)
                    }

                    onReleased: function() {
                        characterButton.color = "#e0e0e0"
                    }

                    onPositionChanged: function(mouse) {
                        if (pressed && (Math.abs(mouse.x - dragStart.x) > 10 || Math.abs(mouse.y - dragStart.y) > 10)) {
                            // Utiliser le système de Drag de Qt
                            characterButton.Drag.active = true
                            characterButton.Drag.hotSpot.x = mouse.x
                            characterButton.Drag.hotSpot.y = mouse.y

                            // Copier le texte dans le presse-papier
                            lexibar.copyToClipboard(model.character)
                        }
                    }
                }

                // Configuration du drag & drop
                Drag.active: mouseArea.drag.active
                Drag.dragType: Drag.Automatic
                Drag.supportedActions: Qt.CopyAction
                Drag.mimeData: { "text/plain": model.character }

                SequentialAnimation {
                    id: animation
                    ColorAnimation {
                        target: characterButton
                        property: "color"
                        to: "#a0e0a0"
                        duration: 100
                    }
                    ColorAnimation {
                        target: characterButton
                        property: "color"
                        to: "#e0e0e0"
                        duration: 100
                    }
                }
            }
        }
    }

    ListModel {
        id: characterModel

        ListElement { character: "¿"; color: "blue" }
        ListElement { character: "¡"; color: "blue" }

        ListElement { character: "ñ"; color: "green" }
        ListElement { character: "á"; color: "green" }
        ListElement { character: "é"; color: "green" }
        ListElement { character: "í"; color: "green" }
        ListElement { character: "ó"; color: "green" }
        ListElement { character: "ú"; color: "green" }

        ListElement { character: "Ñ"; color: "darkorange" }
        ListElement { character: "Á"; color: "darkorange" }
        ListElement { character: "É"; color: "darkorange" }
        ListElement { character: "Í"; color: "darkorange" }
        ListElement { character: "Ó"; color: "darkorange" }
        ListElement { character: "Ú"; color: "darkorange" }
    }
}