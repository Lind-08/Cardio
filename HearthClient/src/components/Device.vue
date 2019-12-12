<template lang="html">
    <div>
        <h1>Devices</h1>
        <ul>
          <li v-for="device in Devices">
            <router-link
                v-bind:key="device.id"
                active-class="is-active"
                class="link"
                :to="{ name: 'Sessions', params: { id: device.id } }">
              {{device.id}}. {{device.serial}}
            </router-link>
          </li>
        </ul>
    </div>    
</template>

<script>
import axios from 'axios';
  export default {
    data () {
      return {
        Devices: [],
        endpoint: 'http://127.0.0.1:5000/registereddevice/',
      }
    },
    created() {
      this.getAllDevices();
    },
    methods: {
      getAllDevices() {
        axios.get(this.endpoint)
        .then(responce => {
          this.Devices = responce.data;
        })
        .catch(error => {
          console.log("------error------", error);
        })
      }
    },
  }
</script>