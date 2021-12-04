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
    title: qsTr("Data Plotter")

    Connections {
        target: controller
        ignoreUnknownSignals: true
        onUpdateValue : addPoint(controller.value)
        onUpdateYMax : yAxis.max = controller.yAxisMax
    }

    TextField {
        id: maxYtextField
        x: 87
        y: 27
        width: 70
        height: 28
        selectByMouse: true
        text: "500"

        Label {
            id: maxYLabel
            x: -72
            y: 0
            width: 53
            height: 28
            color: "#000000"
            text: qsTr("yMax")
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
            font.pointSize: 8
        }

        onAccepted: yAxis.max = text.valueOf()
    }

    TextField {
        id: maxXtextField
        x: 87
        y: 74
        width: 70
        height: 28
        selectByMouse: true
        text: "100"

        Label {
            id: maxXLabel
            x: -72
            y: 0
            width: 53
            height: 28
            color: "#000000"
            text: qsTr("xMax")
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
            font.pointSize: 8
        }

        onAccepted: xAxis.max = text.valueOf()
    }

    TextField {
        id: sPeriodTextField
        x: 87
        y: 119
        width: 70
        height: 28
        selectByMouse: true
        text: "2"

        Label {
            id: sPeriodLabel
            x: -72
            y: 0
            width: 53
            height: 28
            color: "#000000"
            text: qsTr("sPeriod")
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
            font.pointSize: 8
        }

    }

    Rectangle {
        id: plotRectangle
        x: 0
        y: 0
        width: 740
        height: 426
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.top: parent.top
        anchors.bottom: parent.bottom
        anchors.rightMargin: 30
        anchors.leftMargin: 193
        anchors.bottomMargin: 27
        anchors.topMargin: 27
        color: "#6e6e6e"

        property int maxThreshold: (parseInt(maxXtextField.text) * 0.90)|0

        ChartView {
            id: plotView
            anchors.fill: parent
            theme: ChartView.ChartThemeDark
            animationOptions: ChartView.NoAnimation

            ValueAxis {
                id: yAxis
                min: 0
                max: parseInt(maxYtextField.text)
            }

            ValueAxis {
                id: xAxis
                min: 0
                max: parseInt(maxXtextField.text)
            }

            LineSeries {
                id : dataSeries
                name: "LineSeries"
                axisX: xAxis
                axisY: yAxis
            }
        }
    }

    function addPoint(value)
    {
        var xVal = 0
        if(dataSeries.count > 0)
            xVal = ( dataSeries.at(dataSeries.count-1).x + parseInt(sPeriodTextField.text) )

        dataSeries.append(xVal, value)
        console.log("Plotting Value", xVal, value)

        if ( xVal > plotRectangle.maxThreshold )
        {
            var newXAxisMin = (xAxis.max * 0.25)|0
            var newXAxisMax = newXAxisMin + parseInt(maxXtextField.text)

            console.log("New axis", newXAxisMin, newXAxisMax)

            var samplePeriod =  parseInt(sPeriodTextField.text)
            var removePointsCount = (newXAxisMin/samplePeriod)|0

            xAxis.max = newXAxisMax
            xAxis.min = newXAxisMin
            dataSeries.removePoints(0, removePointsCount)

            plotRectangle.maxThreshold = (newXAxisMax * 0.90)|0
            console.log("New maxThreshold", plotRectangle.maxThreshold)
        }
    }
}
