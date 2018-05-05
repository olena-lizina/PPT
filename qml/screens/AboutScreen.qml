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
import "../components"

BlankScreen {
    id: aboutScreen

    CToolBar {
        id: toolBar
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.top: parent.top

        CBackButton {
            anchors.fill: parent
            text: qsTr("About")
        }
    }

    CTextArea {
        anchors.top: toolBar.bottom
        anchors.bottom: parent.bottom
        anchors.left: parent.left
        anchors.right: parent.right

        text:  textStyle() +
               Qt.application.name + " " + Qt.application.version + "<br>
               Based on Qt Quick 2.4 (Qt 5.4.2)<br>
               Built on Dec 23 2017<br><br>
               Copyright (C) 2017-2018 <a href=\"https://www.linkedin.com/in/olena-lizina-177729b1/\">Olena Lizina</a><br>
               <a class=\"link\" href=\"mailto:elenalizina90@gmail.com\">elenalizina90@gmail.com</a><br><br>

               Pedagogical Programming Tool application is distributed under
               <a href=\"http://www.apache.org/licenses/LICENSE-2.0\">Apache Software License, Version 2</a>.<br><br>

               The source code is available at <a href=\"https://github.com/olena-lizina/PPT\">https://github.com/olena-lizina/PPT</a><br><br>

               Unless required by applicable law or agreed to in writing, software distributed under the License
               is distributed on an \"AS IS\" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.<br><br>

               Qt is a registered trademark of The Qt Company Ltd. and/or its subsidiaries."
    }
}
