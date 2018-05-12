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
import StudentManager 1.1
import QtQuick.Dialogs 1.0
import ".."

FileDialog {
    id: selectFile

    signal process(var value)
    signal close()

    folder: "."
    title: qsTr("Choose a file to open")
    selectMultiple: false

    onAccepted: {
        console.log("onAccepted")
        selectFile.process(selectFile.fileUrls)
    }

    Component.onCompleted: visible = true

    onRejected: selectFile.close();
}
