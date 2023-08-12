import QtQuick 2.15
import QtQuick.Controls 2.15
import "../keyboard/content/"
import CppCore 1.0
Rectangle {
   id:root

   width:1024
   height:600

   color:"#ccccd6"

   signal returnSignal()

   property string projectId

   Component.onCompleted: {

   }

   ListView {
       id: listView
       y: 62
       width: parent.width/4*3
       height: 257

       anchors.horizontalCenter: parent.horizontalCenter
       model: Service.getTestPointModel_Project(projectId)
       delegate: SoilTestPointDelegate{
           width:parent.width
           height:50
       }

       Connections{
           target:keyboard1
           function onKeyPressed(textR){
               if(textR!=="."&&isNaN(Number(textR))){
                   return;
               }

               if(listView.currentItem.densityFocus){
                   //all these effert are for the absence of virtualkeyboard
                   let text=listView.currentItem.densityText
                   listView.currentItem.densityText=text+textR

               }
               else if(listView.currentItem.waterRateFocus){
                   let text=listView.currentItem.waterRateText
                   listView.currentItem.waterRateText=text+textR

               }else{
                   return;
               }
           }
           function onDeleteSignal(){
              let active;
              if(listView.currentItem.densityFocus){
                  //all these effert are for the absence of virtualkeyboard
                  let text=listView.currentItem.densityText
                  listView.currentItem.densityText=
                          text.substring(0, text.length - 1)
              }
              else if(listView.currentItem.waterRateFocus){
                  let text=listView.currentItem.waterRateText
                  listView.currentItem.waterRateText=
                          text.substring(0, text.length - 1)
              }else{
                  return;
              }
           }
       }
   }

   Screen01{
       id:keyboard1
       z:99
       width:parent.width
       height:parent.height/2

       opacity:0.3

       screenHeight: parent.height

       anchors.left: parent.left

       onHostHidden: {

              listView.currentItem.disableEdit()
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
