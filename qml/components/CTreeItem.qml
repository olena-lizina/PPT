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
import LecturesManager 1.1
import ".."

Row {
    id: root
    Rectangle{
        width: 20
        height: list.height
        opacity: 0
    }

    Column {
        id: list
        CLabel {
            height: 18.5 * settings.pixelDensity
            width: 0.25 * settings.windowWidth - modelData.nesting * 20 - 20

            text: modelData.text
            wrapMode: Text.Wrap

            MouseArea {
                id: mouseArea
                anchors.fill: parent
                acceptedButtons: Qt.LeftButton | Qt.RightButton
                onClicked: {
                    if (mouse.button == Qt.LeftButton)
                    {
                        if (modelData.idx === -1)
                        {
                            var insertParameters = {
                                title: qsTr("Insert new discipline"),
                                nameLabel: qsTr("Enter discipline name:")
                            }

                            var insertCallback = function(value)
                            {
                                console.log("Callback: " + value + ", idx: " + modelData.idx)
                                LecturesManager.insertDiscipline(value, modelData.idx)
                            }
                            dialog.open(dialog.types.lecture, insertParameters, insertCallback)
                        }
                        else
                            modelData.isOpen = !modelData.isOpen;
                    }
                    else
                    {
                        if (modelData.idx !== -1)
                            contextMenu.visible = !contextMenu.visible
                    }
                }

                onPressAndHold: tooltip.show(modelData.text)
            }
            CHorizontalSeparator {
                anchors.left: parent.left
                anchors.right: parent.right
                anchors.bottom: parent.bottom
            }
            Rectangle {
                anchors.fill: parent
                color: palette.button
                visible: mouseArea.pressed
                opacity: 0.5
            }

            Item {
                id: contextMenu
                visible: false

                property int delegateWidth: parent.width
                property int delegateHeight: 6 * settings.pixelDensity

                Column {
                    CContextMenuButton {
                        width: contextMenu.delegateWidth
                        height: contextMenu.delegateHeight
                        text: qsTr("Insert")
                        onClicked: {
                            root.contextMenuCallback(0)
                        }
                    }

                    CContextMenuButton {
                        text: qsTr("Edit")
                        width: contextMenu.delegateWidth
                        height: contextMenu.delegateHeight
                        onClicked: {
                            root.contextMenuCallback(1)
                        }
                    }

                    CContextMenuButton {
                        width: contextMenu.delegateWidth
                        height: contextMenu.delegateHeight
                        text: qsTr("Remove")
                        onClicked: {
                            root.contextMenuCallback(2)
                        }
                    }
                }
            }
        }

        Loader{ source: modelData.isOpen ? "CChildTreeList.qml" : "CTreeEmptyItem.qml"; }
    }

    function contextMenuCallback(idx)
    {
        contextMenu.visible = false;
        var label;
        var child;

        switch(modelData.nesting)
        {
        case 0:
            label = qsTr("discipline");
            child = qsTr("chapter");
            break;
        case 1:
            label = qsTr("chapter");
            child = qsTr("theme");
            break;
        case 2:
            label = qsTr("theme");
            child = qsTr("subtheme");
            break;
        case 3:
            label = qsTr("subtheme");
            break;
        }

        var dialogTitle;

        switch(idx)
        {
        case 0:
            var insertParameters = {
                title: qsTr("Insert new item"),
                label: qsTr("What would you like to insert?"),
                leftBtnText: label,
                rightBtnText: child
            }

            var insertCallback = function(value)
            {
                console.log("Callback: " + value + ", idx: " + modelData.idx)

                var textLab = value === 0 ? label : child;
                var ins = {
                    title: qsTr("Insert new ") + textLab,
                    nameLabel: qsTr("Enter new ") + textLab + qsTr(" name:")
                }

                var insCallback = function(val)
                {
                    console.log("Callback: " + val + ", idx: " + modelData.idx)

                    switch(modelData.nesting)
                    {
                    case 0:
                        if (value === 0)
                            LecturesManager.insertDiscipline(val, modelData.idx)
                        else
                            LecturesManager.appendChapter(val, modelData.idx)
                        break;
                    case 1:
                        if (value === 0)
                            LecturesManager.insertChapter(val, modelData.idx)
                        else
                            LecturesManager.appendTheme(val, modelData.idx)
                        break;
                    case 2:
                        if (value === 0)
                            LecturesManager.insertTheme(val, modelData.idx)
                        else
                            LecturesManager.appendSubtheme(val, modelData.idx)
                        break;
                    case 3:
                        LecturesManager.insertSubtheme(val, modelData.idx)
                        break;
                    }
                }

                dialog.open(dialog.types.lecture, ins, insCallback)
            }

            if (modelData.nesting === 3)
            {
                insertCallback(0);
            }
            else
            {
                dialog.open(dialog.types.insert, insertParameters, insertCallback)
            }
            break;
        case 1:
            var editParameters = {
                title: qsTr("Edit ") + label,
                nameLabel: qsTr("Enter new ") + label + qsTr(" name:"),
                name: modelData.text
            }

            var editCallback = function(value)
            {
                console.log("Callback: " + value + ", idx: " + modelData.idx)

                switch(modelData.nesting)
                {
                case 0:
                    LecturesManager.updateDiscipline(value, modelData.idx)
                    break;
                case 1:
                    LecturesManager.updateChapter(value, modelData.idx)
                    break;
                case 2:
                    LecturesManager.updateTheme(value, modelData.idx)
                    break;
                case 3:
                    LecturesManager.updateSubtheme(value, modelData.idx)
                    break;
                }
            }

            dialog.open(dialog.types.lecture, editParameters, editCallback)
            break;
        case 2:
            var removeParameters = {
                title: qsTr("Remove ") + label,
                text: qsTr("Press OK to delete ") + label + qsTr(": \"%1\"").arg(modelData.text)
            }

            var removeCallback = function(value)
            {
                console.log("Callback: " + value + ", idx: " + modelData.idx)

                if (!value)
                    return;

                switch(modelData.nesting)
                {
                case 0:
                    LecturesManager.removeDiscipline(modelData.idx)
                    break;
                case 1:
                    LecturesManager.removeChapter(modelData.idx)
                    break;
                case 2:
                    LecturesManager.removeThemes(modelData.idx)
                    break;
                case 3:
                    LecturesManager.removeSubtheme(modelData.idx)
                    break;
                }
            }

            dialog.open(dialog.types.confirmation, removeParameters, removeCallback)
            break;
        }
    }
}
