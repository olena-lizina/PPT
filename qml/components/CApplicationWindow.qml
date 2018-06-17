/****************************************************************************
**
** Copyright (C) 2017-2018 Olena Lizina
**
** Licensed under the Apache License, Version 2.0 (the "License");
** you may not use this file except in compliance with the License.
** You may obtain a copy of the License at
**
** http://www.apache.org/licenses/LICENSE-2.0
**
** Unless required by applicable law or agreed to in writing, software
** distributed under the License is distributed on an "AS IS" BASIS,
** WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
** See the License for the specific language governing permissions and
** limitations under the License.
**
****************************************************************************/

import QtQuick 2.5
import QtQuick.Controls 1.4
import QtQuick.Controls.Styles 1.4
import QtQuick.Window 2.2
import Qt.labs.settings 1.0
import StudentManager 1.1
import "palettes"

ApplicationWindow {
    id: cApplicationWindow
    width: 800
    height: 640
    visible: true
    title: "ZPNP"
    visibility: settings.debugMode ? "FullScreen" : "Maximized"

    style: ApplicationWindowStyle {
        background: Rectangle {
            color: palette.background
        }
    }

    // Loading
    signal loaded()

    Component.onCompleted:
        loaded()

    onLoaded: {
        settings.pixelDensity = settings.debugMode ? 6.0 : Screen.logicalPixelDensity
    }

    // Settings
    QtObject {
        id: settings

        // configurable
        property string font: "Source Code Pro"
        property int fontSize: 40
        property string palette: "Cute"
        property int indentSize: 4
        property bool debugging: true

        // internal
        property bool debugMode: false
        property double pixelDensity
        property string previousVersion: "0.0.0"
        property bool desktopPlatform: Qt.platform.os === "windows" ||
                                       Qt.platform.os === "linux" ||
                                       Qt.platform.os === "osx" ||
                                       Qt.platform.os === "unix"

        property alias windowWidth: cApplicationWindow.width
        property alias windowHeight: cApplicationWindow.height
    }

    Settings {
        category: "Editor"
        property alias font: settings.font
        property alias fontSize: settings.fontSize
        property alias palette: settings.palette
        property alias indentSize: settings.indentSize
        property alias debugging: settings.debugging        
    }

    Settings {
        category: "Version"
        property alias previousVersion: settings.previousVersion
    }

    property alias settings: settings

    // Palettes

    PaletteLoader {
        id: paletteLoader
        name: settings.palette
    }

    property alias palette: paletteLoader.palette

    // Focus Management

    property Item focusItem: null
    signal backPressed()

    onActiveFocusItemChanged: {
        if (focusItem !== null && focusItem.Keys !== undefined)
            focusItem.Keys.onReleased.disconnect(onKeyReleased)

        if (activeFocusItem !== null)
        {
            activeFocusItem.Keys.onReleased.connect(onKeyReleased)
            focusItem = activeFocusItem
        }
    }

    function onKeyReleased(event) {
        if (event.key === Qt.Key_Back || event.key === Qt.Key_Escape) {
            if (Qt.inputMethod.visible)
                Qt.inputMethod.hide()
            else
                backPressed()

            event.accepted = true
        }
    }

    // Editor Fonts

    property ListModel editorFonts: ListModel {
        ListElement {
            name: "Ubuntu Mono"
            source: "../../resources/fonts/editor/ubuntumono.ttf"
        }

        ListElement {
            name: "DejaVu Sans Mono"
            source: "../../resources/fonts/editor/dejavusansmono.ttf"
        }

        ListElement {
            name: "Liberation Mono"
            source: "../../resources/fonts/editor/liberationmono.ttf"
        }

        ListElement {
            name: "Droid Sans Mono"
            source: "../../resources/fonts/editor/droidsansmono.ttf"
        }

        ListElement {
            name: "Fira Mono"
            source: "../../resources/fonts/editor/firamono.ttf"
        }

        ListElement {
            name: "Source Code Pro"
            source: "../../resources/fonts/editor/sourcecodepro.ttf"
        }

        function getCurrentIndex() {
            for (var i = 0; i < count; i++)
            {
                if (get(i).name === settings.font)
                    return i;
            }

            return -1;
        }
    }

    Repeater {
        model: editorFonts
        delegate: Loader {
            sourceComponent: FontLoader {
                source: model.source
            }
        }
    }

    // UI Fonts

    FontLoader {
        source: "../../resources/fonts/ui/robotoregular.ttf"
    }

    FontLoader {
        source: "../../resources/fonts/ui/robotoitalic.ttf"
    }

    FontLoader {
        source: "../../resources/fonts/ui/robotobold.ttf"
    }

    FontLoader {
        source: "../../resources/fonts/ui/robotobolditalic.ttf"
    }

    FontLoader {
        source: "../../resources/fonts/ui/fontawesome.ttf"
    }
}
