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
import QtQuick.Layouts 1.2
import "../components"
import LecturesManager 1.1
import ScreenContextBuffer 1.1

BlankScreen {
    id: educationalMaterialsScreen

    CToolBar {
        id: toolBar
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.top: parent.top

        RowLayout {
            anchors.fill: parent
            spacing: 0

            CBackButton {
                Layout.fillWidth: true
                Layout.fillHeight: true
                text: qsTr("Educational materials")
            }

            CHomeButton {
                onClicked: stackView.push(Qt.resolvedUrl("MainMenuScreen.qml"))
            }
        }
    }

    ListView {
        id: leftMenu

        model: LecturesManager.labsTree
        delegate: CTreeItem {}

        width: 0.25 * settings.windowWidth
        height: settings.windowHeight - toolBar.height

        anchors {
            left: parent.left
            top: toolBar.bottom
        }


    }

    Rectangle {
        id: separator
        anchors.top: toolBar.bottom
        anchors.bottom: parent.bottom
        anchors.left: leftMenu.right
        anchors.leftMargin: 3
        width: Math.max(1, Math.round(0.8 * settings.pixelDensity))
        color: palette.toolBarStripe
        z: 2
    }

    Rectangle {
        id: rightRect

        width: 0.75 * settings.windowWidth
        height: settings.windowHeight - toolBar.height

        anchors {
            left: separator.right
            top: toolBar.bottom
            bottom: parent.bottom
        }

        Component.onCompleted: {
            ScreenContextBuffer.screenType = LecturesManager.LectureFile;
            ScreenContextBuffer.setNestingAndIndex(0, 1)
            handleItemChanged()
        }
    }

    Connections {
        target: ScreenContextBuffer
        onLoaderSourceChanged: handleItemChanged()
        onItemChanged: handleItemChanged()
    }

    Loader {
        id: rectLoader
        anchors.fill: rightRect
    }

    function handleItemChanged()
    {
        rectLoader.source = ""

        if (ScreenContextBuffer.screenType == LecturesManager.LiteratureListFile ||
                ScreenContextBuffer.screenType == LecturesManager.EducationPlanFile ||
                ScreenContextBuffer.screenType == LecturesManager.EducationProgramFile)
        {
            if (LecturesManager.fileExist(ScreenContextBuffer.screenType, ScreenContextBuffer.selectedIdx, ScreenContextBuffer.nesting))
                rectLoader.source = "education/lectures/EditTextScreen.qml"
            else
                rectLoader.source = "education/lectures/AddDisciplineFilesScreen.qml"
            return;
        }

        if (ScreenContextBuffer.nesting == 0)
            rectLoader.source = "education/lectures/AddDisciplineFilesScreen.qml"
        else if (ScreenContextBuffer.nesting == 1)
            rectLoader.source = "education/lectures/DummyScreen.qml"
        else
        {
            if (LecturesManager.fileExist(ScreenContextBuffer.screenType, ScreenContextBuffer.selectedIdx, ScreenContextBuffer.nesting))
                rectLoader.source = "education/lectures/EditTextScreen.qml"
            else
                rectLoader.source = "education/lectures/AddFileScreen.qml"

        }
    }
}
