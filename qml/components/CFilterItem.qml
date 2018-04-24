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

Item {
    id: cFilterItem

    anchors.left: parent.left
    anchors.right: parent.right
    implicitHeight: 18.5 * settings.pixelDensity

    property alias comboLabelText: comboLabel.text
    property alias groupValue: groupComboBox.currentText
    property alias textFieldLabelText: textLabel.text
    property alias textFieldText: textFieldIt.text

    CHorizontalSeparator {
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.bottom: parent.bottom
    }

    Row {
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.verticalCenter: parent.verticalCenter
        anchors.leftMargin: 5 * settings.pixelDensity
        anchors.rightMargin: 5 * settings.pixelDensity
        spacing: 0.5 * settings.pixelDensity

        CLabel {
            id: comboLabel
        }

        ComboBox {
            id: groupComboBox
            Layout.minimumWidth: 200
        }

        CLabel {
            id: textLabel
        }

        TextField {
            id: textFieldIt
            Layout.minimumWidth: 300
        }
    }
}
