<template lang="html">
    <div>
        <h1>Sessions list</h1>
        <ul>
          <li v-for="session in Sessions">
            <router-link
                v-bind:key="session.id"
                active-class="is-active"
                class="link"
                :to="{ name: 'Measurements', params: { id: session.id } }">
              {{session.id}}. {{session.dateTime}}
            </router-link>
          </li>
        </ul>
    </div>    
</template>

<script>
import axios from 'axios';
  export default {
    props: ['id'],
    data () {
      return {
        Sessions: [],
        endpoint: 'http://127.0.0.1:5000/session/',
      }
    },
    created() {
      this.getAllSessions(this.id);
    },
    methods: {
      getAllSessions(id) {
        axios.get(this.endpoint + id)
        .then(responce => {
          this.Sessions = responce.data;
          this.Sessions.forEach(element => {
            element.dateTime = Date.parse(element.dateTime).toString();            
          });
        })
        .catch(error => {
          console.log("------error------", error);
        })
      }
    },
    watch: {
      '$route'() {
        this.getAllSessions(this.id);
      }
    }
  }
</script>