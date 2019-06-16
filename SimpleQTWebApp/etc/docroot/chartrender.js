window.chartColors = {
    red: 'rgb(255, 99, 132)',
    orange: 'rgb(255, 159, 64)',
    yellow: 'rgb(255, 205, 86)',
    green: 'rgb(75, 192, 192)',
    blue: 'rgb(54, 162, 235)',
    purple: 'rgb(153, 102, 255)',
    grey: 'rgb(201, 203, 207)'
};


Chart.defaults.global.animation = false;


var points = 150;
var labels = [];

var data1 = {
    label: 'Channel1',
    fill: false,
    //lineTension:0,
    backgroundColor: window.chartColors.red,
    borderColor: window.chartColors.red,
    data: [],
};
var data2 = {
    label: 'Channel2',
    fill: false,
    //lineTension:0,
    backgroundColor: window.chartColors.blue,
    borderColor: window.chartColors.blue,
    data: [],
};
var data3 = {
    label: 'Channel3',
    fill: false,
    //lineTension:0,
    backgroundColor: window.chartColors.yellow,
    borderColor: window.chartColors.yellow,
    data: [],
};
var data4 = {
    label: 'Channel4',
    fill: false,
    //lineTension:0,
    backgroundColor: window.chartColors.green,
    borderColor: window.chartColors.green,
    data: [],
};
var data5 = {
    label: 'Channel5',
    fill: false,
    //lineTension:0,
    backgroundColor: window.chartColors.yellow,
    borderColor: window.chartColors.yellow,
    data: [],
};
var data6 = {
    label: 'Channel6',
    fill: false,
    //lineTension:0,
    backgroundColor: window.chartColors.blue,
    borderColor: window.chartColors.blue,
    data: [],
};
var data7 = {
    label: 'Channel7',
    fill: false,
    //lineTension:0,
    backgroundColor: window.chartColors.red,
    borderColor: window.chartColors.red,
    data: [],
};
var data8 = {
    label: 'Channel8',
    fill: false,
    //lineTension:0,
    backgroundColor: window.chartColors.purple,
    borderColor: window.chartColors.purple,
    data: [],
};

function clearData() {

    // while (labels.length > 0)
    //     labels.pop();
    // while (data1.data.length > 0)
    //     data1.data.pop();
    // while (data2.data.length > 0)
    //     data2.data.pop();
    // while (data3.data.length > 0)
    //     data3.data.pop();
    // while (data4.data.length > 0)
    //     data4.data.pop();
    // while (data5.data.length > 0)
    //     data5.data.pop();
    // while (data6.data.length > 0)
    //     data6.data.pop();
    // while (data7.data.length > 0)
    //     data7.data.pop();
    // while (data8.data.length > 0)
    //     data8.data.pop();

    labels.length = 0;
    data1.data.length = 0;
    data2.data.length = 0;
    data3.data.length = 0;
    data4.data.length = 0;
    data5.data.length = 0;
    data6.data.length = 0;
    data7.data.length = 0;
    data8.data.length = 0;

    for (var i = 0; i < points; i++) {
        labels.push("");
        data1.data.push(0);
        data2.data.push(0);
        data3.data.push(0);
        data4.data.push(0);
        data5.data.push(0);
        data6.data.push(0);
        data7.data.push(0);
        data8.data.push(0);
    }
}

clearData();


var channelMapping = {};
channelMapping["0"] = 0;
channelMapping["1"] = 1;
channelMapping["2"] = 2;
channelMapping["3"] = 3;
channelMapping["4"] = 4;
channelMapping["5"] = 5;
channelMapping["6"] = 6;
channelMapping["7"] = 7;

//var MONTHS = ['January', 'February', 'March', 'April', 'May', 'June', 'July', 'August', 'September', 'October', 'November', 'December'];
var config = {
    type: 'line',
    data: {
        labels: labels,
        datasets: [data1, data2, data3, data4, data5, data6, data7, data8]
    },
    options: {
        responsive: true,
        title: {
            display: true,
            text: ''
        },
        tooltips: {
            mode: 'index',
            intersect: false,
            enabled: false,
        },
        hover: {
            mode: 'nearest',
            intersect: false
        },
        scales: {
            xAxes: [{
                display: true,
                scaleLabel: {
                    display: true,
                    labelString: 'Time'
                }
            }],
            yAxes: [{
                display: true,
                scaleLabel: {
                    display: true,
                    labelString: 'Value'
                }
            }]
        },
        // Container for pan options
        pan: {
            // Boolean to enable panning
            enabled: true,

            // Panning directions. Remove the appropriate direction to disable
            // Eg. 'y' would only allow panning in the y direction
            mode: 'y'
        },

        // Container for zoom options
        zoom: {
            // Boolean to enable zooming
            enabled: true,

            // Zooming directions. Remove the appropriate direction to disable
            // Eg. 'y' would only allow zooming in the y direction
            mode: 'y',
        }
    }
};

function onDatas(datas, skipUpdate) {
    for (var i = 0; i < datas.length; i++) {

        //获取数据
        var data = datas[i];
        var dataIndex = channelMapping[data.channel];
        if (!dataIndex && dataIndex != 0)
            continue;//未找到匹配关系

        config.data.datasets[dataIndex].data.push(data.value);
        if (config.data.datasets[dataIndex].data.length > points)
            config.data.datasets[dataIndex].data.shift();
    }

    if(!skipUpdate)
        window.myLine.update();
}

function resetDataSet(dataSets) {
    //debugger;
    clearData();
    for (var i = 0; i < dataSets.length; i++)
        onDatas(dataSets[i],true);
    
        window.myLine.update();
}

function buildChart() {
    var ctx = document.getElementById("myChart").getContext("2d");
    window.myLine = new Chart(ctx, config);
}
