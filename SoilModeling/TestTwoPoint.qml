import QtQuick 2.15
import QtQuick.Controls 2.15

Rectangle {
    id: rectangle1

     Rectangle {
         id: rectangle

         width: parent.width*0.1
         height: parent.height*0.2
         color: "antiquewhite"
         anchors.right: toolSeparator.left
         anchors.rightMargin: parent.width/2-width*3

         anchors.top: parent.top
         anchors.topMargin: 50

         radius:width

     }

     ToolSeparator {
         id: toolSeparator

         width: 25
         height: parent.height
         anchors.bottom: parent.bottom
         anchors.bottomMargin: 0
         anchors.horizontalCenter: parent.horizontalCenter
     }

     Rectangle {
         id: rectangleB

         width: parent.width*0.1
         height: parent.height*0.2
         color: "antiquewhite"
         anchors.left: toolSeparator.right
         anchors.top: rectangle.top
         anchors.leftMargin: (parent.width/2-width*3)
         radius:height

     }

     Text {
         id: text1
         x: 52
         width: 20
         height: 20
         color: "#fb0b0b"
         text: qsTr("A")
         anchors.verticalCenter: rectangle.verticalCenter
         anchors.right: rectangle.left
         font.pixelSize: 20
         horizontalAlignment: Text.AlignHCenter
         verticalAlignment: Text.AlignVCenter
         anchors.verticalCenterOffset: 0
         anchors.rightMargin: 10
         font.bold: true
     }

     Text {
         id: text2
         width: 20
         height: 20
         color: "#fb0b0b"
         text: qsTr("A")
         anchors.verticalCenter: rectangle.verticalCenter
         anchors.left: rectangle.right
         font.pixelSize: 20
         horizontalAlignment: Text.AlignHCenter
         verticalAlignment: Text.AlignVCenter
         anchors.leftMargin: 10
         font.bold: true
     }

     Button {
         id: button
         width: 100
         height: 30
         text: qsTr("测试")
         anchors.top: rectangle.bottom
         anchors.topMargin: 40
         anchors.horizontalCenter: rectangle.horizontalCenter
     }

     Text {
         id: text3
         y: 17
         width: 20
         height: 20
         color: "#fb0b0b"
         text: qsTr("B")
         anchors.bottom: rectangleB.top
         font.pixelSize: 20
         horizontalAlignment: Text.AlignHCenter
         verticalAlignment: Text.AlignVCenter
         anchors.bottomMargin: 10
         font.bold: true
         anchors.horizontalCenter: rectangleB.horizontalCenter
     }

     Text {
         id: text4
         width: 20
         height: 20
         color: "#fb0b0b"
         text: qsTr("B")
         anchors.top: rectangleB.bottom
         font.pixelSize: 20
         horizontalAlignment: Text.AlignHCenter
         verticalAlignment: Text.AlignVCenter
         anchors.topMargin: 10
         anchors.horizontalCenterOffset: 0
         font.bold: true
         anchors.horizontalCenter: rectangleB.horizontalCenter
     }

     Button {
         id: button1
         width: 100
         height: 30
         text: qsTr("测试")
         anchors.top: rectangleB.bottom
         anchors.topMargin: 40
         anchors.horizontalCenter: rectangleB.horizontalCenter
     }
}
