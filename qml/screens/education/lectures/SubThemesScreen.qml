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
import LecturesManager 1.1
import "../../../components"
import "../.."

BlankScreen {
    id: subThemesScreen

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
                text: LecturesManager.selectedItem(LecturesManager.Themes)
            }

            CToolButton {
                Layout.fillHeight: true
                icon: "\uf067"
                tooltipText: qsTr("New sub theme")
                onClicked: {
                    var parameters = {
                        title: qsTr("New sub theme"),
                        label: qsTr("Enter sub theme name :"),
                        itemType: LecturesManager.SubThemes
                    }

                    var callback = function(value)
                    {
                        LecturesManager.addItem(value, LecturesManager.SubThemes)
                        listModel.model = LecturesManager.getListModel(LecturesManager.SubThemes)
                    }

                    dialog.open(dialog.types.newLecture, parameters, callback)
                }
            }

            CHomeButton {
                onClicked: stackView.push(Qt.resolvedUrl("../../MainMenuScreen.qml"))
            }
        }
    }

    CDragAndDropList {

        id: listModel

        selectedItem: LecturesManager.SubThemes

        anchors {
            top: toolBar.bottom
            right: parent.right
            left: parent.left
            bottom: parent.bottom
        }

        onClicked: {
            if (LecturesManager.itemHasFile(LecturesManager.selectedItem(LecturesManager.SubThemes), LecturesManager.SubThemes))
            {
                LecturesManager.selectFile()
                stackView.push(Qt.resolvedUrl("EditTextScreen.qml"))
            }
            else
            {
                LecturesManager.createFile()
                stackView.push(Qt.resolvedUrl("EditTextScreen.qml"))
            }
        }

        onEditClicked: {
            var parameters = {
                title: qsTr("Edit sub theme"),
                label: qsTr("Sub theme name :"),
                valueEdit: LecturesManager.selectedItem(LecturesManager.SubThemes),
                itemType: LecturesManager.SubThemes
            }

            var callback = function(value)
            {
                LecturesManager.editItem(value, LecturesManager.SubThemes)
                model = LecturesManager.getListModel(LecturesManager.SubThemes)
            }

            dialog.open(dialog.types.editLecture, parameters, callback)
        }

        onRemoveClicked: {
            var parameters = {
                title: qsTr("Delete the sub theme"),
                text: qsTr("Are you sure you want to delete \"%1\"?").arg(LecturesManager.selectedItem(LecturesManager.SubThemes))
            }

            var callback = function(value)
            {
                if (value)
                {
                    LecturesManager.deleteItem(LecturesManager.SubThemes)
                    model = LecturesManager.getListModel(LecturesManager.SubThemes)
                }
            }

            dialog.open(dialog.types.confirmation, parameters, callback)
        }
    }
}
