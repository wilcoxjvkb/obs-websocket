import QtQuick 2.0
import QtQuick.Controls 1.2
import QtQuick.Dialogs 1.2
import QtQuick.Layouts 1.3

Dialog {
    id: dialog
    width: 407
    height: 195
    property alias dialogTitle: dialog.title
    title: qsTr("OBSWebsocket.Settings.DialogTitle")
    visible: true
}




