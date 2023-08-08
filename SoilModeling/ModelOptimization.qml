import QtQuick 2.15
import QtQuick.Controls 2.15
import QtCharts 2.3

import "../"
import CppCore 1.0
Item {
    id: item1

    property string modelName:"ABC"
    state: "waterRate"


    Component.onCompleted: {
        //to init ChartView
        let type=state==="wetDensity"?0:1
        listView.model.updateSeries(testPointSeries,selectedSeries,type)
    }

    Connections{
        target:listView.model

        function onRequireSeiresSwap(pointIndex,reverse){
            let type=item1.state==="waterRate"?1:0
            if(!reverse){
                //swapper.swapPoint(point.x,point.y,line.series(1),line.series(2))
                listView.model.swapSeriesPoint(testPointSeries,selectedSeries,pointIndex,type)

            }else{
                listView.model.swapSeriesPoint(selectedSeries,testPointSeries,pointIndex,type)
            }
        }
        function onChartNeedsUpdate(){
            console.log("qml chart updating");
            let type=state==="wetDensity"?0:1
            listView.model.updateSeries(testPointSeries,selectedSeries,type)
        }
    }


    Rectangle{
        id:leftArea

        border.width: 1
        border.color: "black"

        width: parent.width/3*2
        height: parent.height-10
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 5
        anchors.left: parent.left
        anchors.leftMargin: 5

        ChartView {
            id: line

            theme:ChartView.ChartThemeBlueCerulean

            Connections{
                target:listView.model
                function onUpdateAxis(xmin,xmax,ymin,ymax){
                    xAxis.max=xmax
                    xAxis.min=xmin

                    //for(let i in selectedSeries.declarativeChildren)

                    yAxis.max=ymax
                    yAxis.min=ymin
                }
            }



            anchors.fill: parent

            ValueAxis{
                id:xAxis
                labelFormat: "%F"
                max:10
                min:-1

            }

            ValueAxis{
                id:yAxis
                labelFormat: "%F"
                max:10
            }



            LineSeries{
                id:fitLineSeries
                name:"拟合曲线"
                color:"blue"
                axisX: xAxis
                axisY:yAxis
            }

            ScatterSeries{
                id:selectedSeries
                axisX: xAxis
                axisY:yAxis

                color:"black"
                pointsVisible: true
                markerSize: 15
                markerShape: ScatterSeries.MarkerShapeCircle

                onClicked:function(point) {
                    let type=state==="wetDensity"?0:1
                    listView.model.setChecked(point,true,type);
                    line.update()
                }

            }

            ScatterSeries {
                id: testPointSeries
                name: "LineSeries"
                axisX: xAxis
                axisY:yAxis
                pointsVisible: true
                markerSize: 20
                onClicked:function(point) {           
                    let type=state==="wetDensity"?0:1
                    listView.model.setChecked(point,false,type);
                    line.update()
                }

                markerShape: ScatterSeries.MarkerShapeCircle
                color:"lightgreen"

                pointLabelsVisible: true



            }

        }


    }

    Rectangle {
        id: rectangle
        width: parent.width/3-20
        height: leftArea.height
        color: "#ffffff"
        anchors.left: leftArea.right
        anchors.top: leftArea.top
        anchors.leftMargin: 5
        anchors.topMargin: 0
        ListView {
            id: listView


            anchors.bottom: parent.bottom
            anchors.left: parent.left

            width:parent.width
            height:parent.height/2

            header:ViewHeader{
                width:parent.width
                height:50

                itemInfo: ListModel{
                    ListElement{
                        label:"选定"
                    }
                    ListElement{
                        label:"测点"
                    }
                }


                widthRate:[1,1]
            }

            model: Service.getTestPointModel()




            delegate: Item {


                width:parent.width
                height:50

                Row {
                    anchors.fill: parent
                    Rectangle{
                        height:parent.height
                        width:parent.width/2
                        CheckBox{

                            anchors.fill: parent
                            checked: model.isSelected

                            onCheckStateChanged: {
                                model.isSelected=checked;
                            }
                        }
                    }
                    Rectangle{
                        height:parent.height
                        width:parent.width/2
                        Text{
                            id: text3
                            anchors.fill: parent

                            text:model.index

                            font.pixelSize: 24
                            horizontalAlignment: Text.AlignHCenter
                            verticalAlignment: Text.AlignVCenter
                        }
                    }

                }
            }
        }

        Column{
            anchors.left: parent.left
            anchors.top: parent.top
            width:parent.width
            height:parent.height/2


            Text {
                id: text1

                width:parent.width
                height:50

                text:qsTr("模型名称:"+modelName)
                font.pixelSize: 25
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
                font.bold: true
            }

            Text {
                id: text2

                width: parent.width
                height: 50
                text: qsTr("参数:")

                font.pixelSize: 24
                horizontalAlignment: Text.AlignLeft
                verticalAlignment: Text.AlignVCenter
                font.bold: true

            }

            MyButton {
                id: button


                width:parent.width
                height:50
                text: qsTr("切换参数")

                onClicked:{
                    if(item1.state==="wetDensity"){
                        item1.state="waterRate"
                    }else if(item1.state==="waterRate"){
                        item1.state="wetDensity"
                    }
                    let type=item1.state==="wetDensity"?0:1
                    listView.model.updateSeries(testPointSeries,selectedSeries,type)
                }
                originColor:"#0C5917"
                hoverColor:Qt.lighter(originColor,1.5)
            }

            MyButton {
                id: button1

                width:parent.width
                height:50
                text: qsTr("拟合优化")

                originColor:"#0C5917"
                hoverColor:Qt.lighter(originColor,1.5)
                onClicked: {
                    let val=item1.state==="wetDensity"?0:1
                    listView.model.getFitSequence(fitLineSeries,val)
                }
            }
            MyButton {
                id: button2

                width:parent.width
                height:50
                text: qsTr("保存模型")

                originColor:"#0C5917"
                hoverColor:Qt.lighter(originColor,1.5)
                onClicked: {
                    console.log("saving model")
                }
            }
        }


    }
    states: [
        State {
            name: "waterRate"
            PropertyChanges {
                target: line
                x: 21
                y: 26
                width: 396
                height: 390
                title: "含水率"
            }
            PropertyChanges {
                name: "含水率"
                target: testPointSeries

            }
        },
        State {
            name: "wetDensity"

            PropertyChanges {
                target: line

                title: "湿密度"
            }

            PropertyChanges {
                name: "湿密度"
                target: testPointSeries

            }


        }
    ]
}
