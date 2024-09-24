import QtQuick 2.15
import QtCharts 2.15
import QtQuick.Controls 2.15
import QtQuick 2.15

ApplicationWindow {
    visible: true
    width: 640
    height: 480
    title: "Real-time Sensor Data"

    ChartView {
        id: chart
        anchors.fill: parent
        theme: ChartView.ChartThemeBrownSand
        antialiasing: true

        title: "Temperature and Humidity Statistics"

        // Temperature series
        LineSeries {
            id: temperatureSeries
            name: "Temperature"
            axisX: xAxis
            axisY: tempAxis
        }

        // Humidity series
        LineSeries {
            id: humiditySeries
            name: "Humidity"
            axisX: xAxis
            axisY: humidityAxis
        }

        // X Axis for time
        ValueAxis {
            id: xAxis
            min: 0
            max: 10
            tickCount: 10
            titleText: "Time (s)"
        }

        // Y Axis for temperature
        ValueAxis {
            id: tempAxis
            min: 20
            max: 40
            tickCount: 5
            titleText: "Temperature (°C)"
        }

        // Y Axis for humidity
        ValueAxis {
            id: humidityAxis
            min: 0
            max: 100
            tickCount: 5
            titleText: "Humidity (%)"
        }

        // Fetch data from Firebase periodically
        Timer {
            interval: 3000 // 3 seconds
            running: true
            repeat: true
            onTriggered: {
                fetchData();
            }
        }
    }

    // Fetch data from Firebase using XMLHttpRequest
    function fetchData() {
        var xhr = new XMLHttpRequest();
        var firebaseUrl = "https://syntem-esp32-qt-qml-default-rtdb.firebaseio.com/sensorData.json";
        xhr.open("GET", firebaseUrl);
        xhr.onreadystatechange = function() {
            if (xhr.readyState === XMLHttpRequest.DONE && xhr.status === 200) {
                var data = JSON.parse(xhr.responseText);
                log(data.temperature, data.humidity);
                updateChart(data.temperature, data.humidity);
            }
        };
        xhr.send();
    }


    function updateChart(temperature, humidity) {
        var timeCount = temperatureSeries.count;

        // Add new data points to the series
        temperatureSeries.append(timeCount, temperature);
        humiditySeries.append(timeCount, humidity);

        // Ensure X-axis adjusts to show the latest points
        if (temperatureSeries.count > 10) {
            xAxis.min = temperatureSeries.count - 10;
            xAxis.max = temperatureSeries.count;
        } else {
            xAxis.max = temperatureSeries.count + 1;
        }

        // Optionally update Y-axis scaling to fit the temperature and humidity values
        if (temperature > tempAxis.max) {
            tempAxis.max = temperature + 5; // Adjust the upper bound for temperature
        }
        if (temperature < tempAxis.min) {
            tempAxis.min = temperature - 5; // Adjust the lower bound for temperature
        }
        if (humidity > humidityAxis.max) {
            humidityAxis.max = humidity + 5; // Adjust the upper bound for humidity
        }
        if (humidity < humidityAxis.min) {
            humidityAxis.min = humidity - 5; // Adjust the lower bound for humidity
        }
    }

    function log(tem, hum) {
        console.log("GET: https://syntem-esp32-qt-qml-default-rtdb.firebaseio.com/sensorData.json");
        console.log("Temperature: " + tem)
        console.log("Humidity: " + hum)
    }
}
