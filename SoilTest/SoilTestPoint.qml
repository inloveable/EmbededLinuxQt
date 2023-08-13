import QtQuick 2.15
import QtQuick.Controls 2.15
import "../keyboard/content/"
import"../"
import"../SoilModeling/"
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

   property color buttonOriginColor:"#4b1e2f"
   property color buttonHoverColor:"#2b1216"



   gradient: Gradient {
               GradientStop { position: 0.0; color: "#8e0e00" }

               GradientStop { position: 1.0; color: "#1f1c18" }
               //start: Qt.point(0, 0)
               //end: Qt.point(width, height)
               orientation: Qt.Horizontal

           }

   MyButton{
       id:saveProjectButton

       width:100
       height:50
       anchors.top: removePointButton.bottom
       anchors.topMargin: 10

       anchors.right: viewArea.left
       anchors.rightMargin: 10

       text:qsTr("保存")

       originColor: buttonOriginColor
       hoverColor:buttonHoverColor

       onClicked: {
           Service.projectInfoSave();
       }
   }

   MyButton{
       id:createNewButton

       width:100
       height:50


       anchors.top: viewArea.top

       anchors.right: viewArea.left
       anchors.rightMargin: 10

       originColor: buttonOriginColor
       hoverColor:buttonHoverColor


       text:qsTr("新建")

       onClicked: {
           Service.addProjectTestPoint()
       }
   }
   MyButton{
       id:removePointButton

       width:100
       height:50
       anchors.top: createNewButton.bottom
       anchors.topMargin: 10

       anchors.right: viewArea.left
       anchors.rightMargin: 10

       originColor: buttonOriginColor
       hoverColor:buttonHoverColor

       text:"删除"

       onClicked: {
           Service.removeProjectTestPoint(listView.count-1)
       }
   }

   MyButton{
       id:refreshButton

       width:100
       height:50
       anchors.top: saveProjectButton.bottom
       anchors.topMargin: 10

       anchors.right: viewArea.left
       anchors.rightMargin: 10

       text:qsTr("刷新")

       originColor: buttonOriginColor
       hoverColor:buttonHoverColor

       onClicked: {
           Service.refreshTestPointDataWithModel()
       }
   }


   Rectangle{
       id:viewArea

       anchors.top: parent.top
       anchors.topMargin: 20

       width: parent.width/4*3
       height: parent.height/2
       anchors.horizontalCenter: parent.horizontalCenter

       color:"transparent"

       border.width: 1
       border.color: "black"
       ListView {
           id: listView

           Component.onCompleted: {
               Service.requestTinyModelInfo()
           }

           Connections{
               target:Service

               function onTinyModelInfoUpdated(){
                   console.log("front end recv tiny modelinfo")
                   listView.tinyModelInfo=Service.getTinyModelInfos()
               }

           }
           property var tinyModelInfo;

           anchors.fill: parent

           model: Service.getTestPointModel_Project(projectId)




           delegate: SoilTestPointDelegate{
               width:parent.width
               height:50

               indexText: model.index
               densityText: model.density
               waterRateText: model.waterRate
               ampText:model.ampitude//error spelling
               phaseAngleText:model.phaseAngle

               comboBoxModel:listView.tinyModelInfo

               widthRate:[1,3,3,2,2,2,2]

               onDensityInputActive:function (active) {

                   if(active===true){
                       keyboard1.state="invoked"
                   }
                   listView.currentIndex=index
               }
               onWaterRateInputActive: function(active){

                   if(active===true){
                       keyboard1.state="invoked"
                   }
                   listView.currentIndex=index
               }
               onSolidityInputActive: function(active){
                   if(active===true){
                       keyboard1.state="invoked"
                   }
                   listView.currentIndex=index
               }

               onComboBoxIndexChanged:function(itemIndex,modelIndex){
                   console.log("test point:"+index+" ‘s model changed to:"+modelIndex);
                   Service.setPointToModel(index,modelIndex);
               }
           }

           header:ViewHeader{
               width:parent.width
               height:50

               widthRate:[1,3,3,2,2,2,2]

               itemInfo:ListModel{
                   ListElement{
                       label:"测点"
                   }
                   ListElement{
                       label:"土壤模型"
                   }
                   ListElement{
                       label:"湿密度(g/cm^3)"
                   }
                   ListElement{
                       label:"含水率(%)"
                   }
                   ListElement{
                       label:"相角"
                   }
                   ListElement{
                       label:"幅值"
                   }
                   ListElement{
                       label:"压实度"
                   }

               }
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

                   }
                   else if(listView.currentItem.solidtiyFocus){
                       let text=listView.currentItem.solidityText
                       listView.currentItem.solidityText=text+textR

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
                  }else if(listView.currentItem.solidtiyFocus){
                      let text=listView.currentItem.solidityText
                      listView.currentItem.solidityText=
                              text.substring(0, text.length - 1)
                  }else{
                      return;
                  }
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
       anchors.top: viewArea.bottom
       anchors.horizontalCenterOffset: 0
       anchors.topMargin: 18
       anchors.horizontalCenter: parent.horizontalCenter

       RoundButton {
           id: roundButton
           x: 103
           width: 75
           height: 75
           text: "测试A"
           anchors.verticalCenter: parent.verticalCenter
           anchors.right: toolSeparator.left
           anchors.rightMargin:width

           onClicked: {
               Service.requestPointTest(listView.count-1)
           }
       }

       RoundButton {
           id: roundButton1
           text: "测试B"
           anchors.verticalCenter: parent.verticalCenter
           anchors.left: toolSeparator.right
           anchors.leftMargin: width
           width: 75
           height: 75
           onClicked: {
               Service.requestPointTest(listView.count-1)
           }
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

       width: 75
       height: 75
       opacity:0.5
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
