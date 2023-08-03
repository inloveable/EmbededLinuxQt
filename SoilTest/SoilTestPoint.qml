import QtQuick 2.15
import QtQuick.Controls 2.15

Rectangle {
   id:root

   width:1024
   height:600

   color:"#ccccd6"

   signal returnSignal()

   property string projectName

   Component.onCompleted: {
       console.log("item project name:["+projectName+"]")
   }

   ListView {
       id: listView
       y: 62
       width: parent.width/4*3
       height: 257

       anchors.horizontalCenter: parent.horizontalCenter
       model: ListModel {
           ListElement {
               name: "Grey"
               colorCode: "grey"
           }

           ListElement {
               name: "Red"
               colorCode: "red"
           }

           ListElement {
               name: "Blue"
               colorCode: "blue"
           }

           ListElement {
               name: "Green"
               colorCode: "green"
           }
       }
       delegate: Item {
           x: 5
           width: 80
           height: 40
           Row {
               id: row1
               Rectangle {
                   width: 40
                   height: 40
                   color: colorCode
               }

               Text {
                   text: name
                   anchors.verticalCenter: parent.verticalCenter
                   font.bold: true
               }
               spacing: 10
           }
       }
   }

   Rectangle {
       id: rectangle
       width: listView.width/4*3
       height: 206
       color: "transparent"
       radius: 10
       border.color: "#51f50c"
       border.width: 4
       anchors.top: listView.bottom
       anchors.horizontalCenterOffset: 0
       anchors.topMargin: 18
       anchors.horizontalCenter: parent.horizontalCenter

       RoundButton {
           id: roundButton
           x: 103
           width: 75
           height: 75
           text: "+"
           anchors.verticalCenter: parent.verticalCenter
           anchors.right: toolSeparator.left
           anchors.rightMargin:width
       }

       RoundButton {
           id: roundButton1
           text: "+"
           anchors.verticalCenter: parent.verticalCenter
           anchors.left: toolSeparator.right
           anchors.leftMargin: width
           width: 75
           height: 75
       }

       ToolSeparator {
           id: toolSeparator
           height:parent.height
           anchors.top: parent.top
           anchors.topMargin: 0
           anchors.horizontalCenter: parent.horizontalCenter
       }
   }

   RoundButton {
       id: roundButton2

       width: 100
       height: 100
       text: "返回"
       anchors.right: parent.right
       anchors.bottom: parent.bottom
       font.bold: true
       font.pointSize: 17
       anchors.bottomMargin: 40
       anchors.rightMargin: 40

       onClicked:{
           returnSignal()
       }
   }
}
