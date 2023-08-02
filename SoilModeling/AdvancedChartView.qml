import QtQuick 2.15
import QtCharts 2.13
import CppCore 1.0
ChartView {
    id: line


    SeriesPointSwaper{
        id:swapper
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
            swapper.swapPoint(point.x,point.y,line.series(1),line.series(2))
        }
        XYPoint{
            x:10000
            y:0
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
            console.log("point clicked:"+point.x+","+point.y)
            console.log()

            swapper.swapPoint(point.x,point.y,line.series(2),line.series(1))
            line.update()
            //line.selectPoint(point)
        }

        markerShape: ScatterSeries.MarkerShapeCircle
        color:"lightgreen"

        pointLabelsVisible: true


        XYPoint {
            x: 0
            y: 2
        }

        XYPoint {
            x: 1
            y: 1.2
        }

        XYPoint {
            x: 0.5
            y: 0.5
        }


        XYPoint {
            x: 2
            y: 3.3
        }

        XYPoint {
            x: 5
            y: 2.1
        }
        XYPoint {
            x: 6
            y: 3.7
        }

        XYPoint{//点击最后一个点会产生崩溃，人工构造一个无限远的点，注意在计算时移除
            x:10000
            y:0
        }
    }

}
