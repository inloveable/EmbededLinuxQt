import QtQuick 2.15

Item {


     property int currentIndex


    //一旦一个delegate有了required property.
    //则访问诸如index，modelData.model则需要将其也列为required


    property alias text:text1.text


    Rectangle{
        anchors.fill: parent
        opacity: 0.5

        border.width: 1
        border.color: "black"

        Text{
            id:text1
            anchors.fill: parent

            horizontalAlignment: Qt.AlignHCenter
            verticalAlignment:Qt.AlignVCenter

            font.pixelSize: 20
            font.bold: true
        }
    }


}
