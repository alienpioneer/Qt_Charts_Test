import QtQuick 2.12
import QtQuick.Window 2.12
import QtQuick.Controls 2.12
import QtCharts 2.1

Window {
    id: window
    width: 1000
    height: 480
    visible: true
    color: "#404040"
    title: qsTr("Plotter")

    Connections {
        target: controller
        ignoreUnknownSignals: true
        onUpdateValue : console.log(controller.getValue)
    }

    ChartView {
        id: chartView
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.top: parent.top
        anchors.bottom: parent.bottom
        anchors.rightMargin: 30
        anchors.leftMargin: 230
        anchors.bottomMargin: 27
        anchors.topMargin: 27
        theme: ChartView.ChartThemeDark
        animationOptions: ChartView.NoAnimation

        ValueAxis {
            id: yAxis
            min: 0
            max: 0
        }

        ValueAxis {
            id: xAxis
            min: 0
            max: 0
        }

        LineSeries {
            id : dataSeries
            name: "LineSeries"
            axisX: xAxis
            axisY: yAxis

//            XYPoint {
//                x: 0
//                y: 2000
//            }

        }

        Component.onCompleted: {
//            yAxix.max = controller.getYMax
//            xAxix.max = controller.getXMax
            console.log(controller.getYMax)
            console.log(controller.getXMax)
        }
    }

    function addPoint(value){
        console.log(value)
        dataSeries.append(x,value)
    }
}
