import Vue from 'vue'
import Router from 'vue-router'
import Meta from 'vue-meta'
import Device from './components/Device.vue'
import Sessions from './components/Sessions.vue'
import Measurements from './components/Measurements.vue'

Vue.use(Router)
Vue.use(Meta)

const router = new Router({
 routes: [
   {
     path: '/',
     name:'Device',
     component: Device,
   },
   {
     path: '/sessions/:id',
     name: 'Sessions',
     component: Sessions,
     props: true
   },
   {
     path: '/session/:id/measurements',
     name: 'Measurements',
     component: Measurements,
     props: true     
   }

 ],
  mode: 'history'
})

new Vue({
 el: '#app',
 render: h => h(App),
 router
})