<template lang="html">
    <div>
        <h1>Measurements</h1>
        <canvas id="cardioChart" width="200" height="100"></canvas>
    </div>    
</template>

<script>
import axios from 'axios';
import Chart from 'chart.js'

  export default {
    props: ['id'],
    data () {
      return {
        myChart: null,
        lastDelta: 0,
        endpoint: 'http://127.0.0.1:5000/measurement/',
      }
    },
    mounted() {
        var ctx = document.getElementById("cardioChart").getContext('2d');
        this.myChart = new Chart(ctx, {
            type: 'line',
            data: {
                labels: [],
                datasets: [{
                    label: "Potential",
                    backgroundColor: ['rgba(255, 99, 132, 0.2)'], 
                    borderColor: ['rgba(255, 99, 132, 1)'],
                    fill: false,
                    data: [],
                }],
            },
            options: {
                scales: {
                    yAxes: [{
                        ticks: {
                            beginAtZero: true
                        }
                    }]
                }
            }
        });
        this.getAllMeasures(this.id);
    },
    methods: {

        updateChart(measures)
        {
            measures.forEach(m => {
                this.myChart.data.labels.push(m.millisDelta);
                this.myChart.data.datasets.forEach(d => d.data.push(m.value));
            });
            this.myChart.update();
        },

      getAllMeasures(id) {
        axios.get(this.endpoint, {
            params: {
                SessionId: id,
                lastDelta: this.lastDelta
            }
        })
        .then(responce => {
            if (responce.data.length > 0) {
                this.updateChart(responce.data);
                var elem = responce.data[responce.data.length - 1];
                this.lastDelta = elem.millisDelta;
            }
            setTimeout(this.getAllMeasures, 1000, this.id);
        })
        .catch(error => {
          console.log("------error------", error);
        })
      }
    },
    watch: {
      '$route'() {
        this.getAllMeasures(this.id);
      }
    }
  }
</script>