import QtQuick 2.15
import QtQuick.Controls 2.0
Button {
    id: root
    text: qsTr("更新")

    property color hoverColor
    property color originColor;
    background: Rectangle{
        id:background
        color: mouse.containsMouse?hoverColor:originColor
        radius: 7

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
               duration:200
            }
        }
    }





    contentItem:Text{
            anchors.centerIn: parent
            text:button.text
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
            font.pointSize: 18
            font.bold: true
        }


    onClicked:{
       Service.getTime();
    }
}
