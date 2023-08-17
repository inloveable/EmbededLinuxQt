import QtQuick 2.15
import QtQuick.Controls 2.15
import CppCore 1.0
Rectangle {

    color:"transparent"

    width:parent.width


    function getCurrentIndex(){
        return listView.currentIndex();
    }


    ListView {
        id: listView
        anchors.fill: parent


        function columnWidth(index){
            return listView.header.columnWidthF(index)
        }




        header:ViewHeader{
            id:viewHeader
            width:parent.width
            widthRate:[1,3,4,4,3]
            height:40
            Component.onCompleted: {
                  console.log("width header:"+viewHeader.width+" view:"+listView.width)
            }



            itemInfo:ListModel{
                ListElement{
                     label:"编号"
                }
                ListElement{
                     label:"名称"
                }
                ListElement{
                     label:"建立时间"
                }
                ListElement{
                     label:"相关系数"
                }
                ListElement{
                     label:"来源"
                }
            }

        }
        headerPositioning: ListView.InlineHeader
        model: Service.getModelManagModel()




        function getWidth(cindex){
            let count=0;

            for(var i=0;i<widthRate.length;++i){
                count+=widthRate[i];
            }
            let totalWidth=listView.width
            let slice=totalWidth/count
            return slice*widthRate[cindex];
        }
        property var widthRate:[1,3,4,4,3];

        delegate: SwipeDelegate {
            id: swipeDelegate


            width: parent.width
            height:70
            background:Rectangle{
                color:!(index===listView.currentIndex) ?Qt.lighter("lightgreen",1.2):"lightgreen";
                border.width: 1;
                border.color: "white"

            }

            Connections{
                target:swipeDelegate
                function onClicked(){
                    listView.currentIndex=index
                }
            }

            contentItem:Row{
                //anchors.fill: parent
                id:row
                property int leftWidth:row.width*0.75
                //width/4*3/4--> 3/16



                //

                Rectangle{
                   height:parent.height
                   width:listView.getWidth(0)
                   color:"transparent"



                   Text{
                       anchors.fill:parent
                       text:model.index
                       verticalAlignment: Qt.AlignVCenter
                       horizontalAlignment: Qt.AlignHCenter
                   }

                }
                Rectangle{

                   height:parent.height
                   width:listView.getWidth(1)
                   color:"transparent"
                   Text{
                       anchors.fill:parent
                       text:model.name
                       verticalAlignment: Qt.AlignVCenter
                       horizontalAlignment: Qt.AlignHCenter
                   }
                }
                Rectangle{

                   height:parent.height
                   width:listView.getWidth(2)
                   color:"transparent"
                   Text{
                       anchors.fill:parent
                       text:model.time
                       verticalAlignment: Qt.AlignVCenter
                       horizontalAlignment: Qt.AlignHCenter
                   }
                }
                Rectangle{

                   height:parent.height
                   width:listView.getWidth(3)
                   color:"transparent"
                   Text{
                       anchors.fill:parent
                       text:model.rate
                       verticalAlignment: Qt.AlignVCenter
                       horizontalAlignment: Qt.AlignHCenter
                   }
                }
                Rectangle{

                   height:parent.height
                   width:listView.getWidth(4)
                   color:"transparent"
                   Text{
                       anchors.fill:parent
                       text:model.source
                       verticalAlignment: Qt.AlignVCenter
                        horizontalAlignment: Qt.AlignHCenter
                   }
                }
            }

            ListView.onRemove: SequentialAnimation {
                PropertyAction {
                    target: swipeDelegate
                    property: "ListView.delayRemove"
                    value: true
                }
                NumberAnimation {
                    target: swipeDelegate
                    property: "height"
                    to: 0
                    easing.type: Easing.InOutQuad
                }
                PropertyAction {
                    target: swipeDelegate
                    property: "ListView.delayRemove"
                    value: false
                }
            }

            swipe.right: Label {
                id: deleteLabel
                text: qsTr("删除")
                color: "white"
                verticalAlignment: Label.AlignVCenter
                padding: 12
                width:75
                height: parent.height
                anchors.right: parent.right

                SwipeDelegate.onClicked: {
                    console.log("removing model from list")
                    listView.model.removeModel(index)
                }



                background: Rectangle {
                    color: deleteLabel.SwipeDelegate.pressed ? Qt.darker("tomato", 1.1) : "tomato"
                }
            }
        }
    }

}
