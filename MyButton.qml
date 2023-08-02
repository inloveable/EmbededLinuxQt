import QtQuick 2.15
import QtQuick.Controls 2.0


Button {
    id: root
    text: qsTr("更新")

    property color hoverColor
    property color originColor;

    property alias fontColor:mainText.color
    background: Rectangle{
        id:background
        color: mouse.containsMouse?hoverColor:originColor
        radius: 7


        border.width: 2
        border.color: hoverColor

        MouseArea{
            id:mouse
            anchors.fill: parent
            hoverEnabled: true

            propagateComposedEvents: true
            onClicked:{
                root.clicked();
            }

            onDoubleClicked: {
                root.doubleClicked();
            }
        }

        Behavior on color{
            ColorAnimation {
               duration:500
            }
        }
    }

    contentItem:Text{
            id:mainText
            anchors.centerIn: parent
            text:root.text
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
            font.pointSize: 18
            font.bold: true
            color:"white"
        }

}
