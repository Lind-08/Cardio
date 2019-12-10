using System;
using System.Collections.Generic;
using System.Linq;
using Microsoft.EntityFrameworkCore;
using Microsoft.AspNetCore.Mvc;
using HearthServer.Models;
 
namespace HearthServer.Controllers
{
    [Route("[controller]")]
    public class RegisteredDeviceController : Controller
    {
        HearthContext db;
        public RegisteredDeviceController(HearthContext context)
        {
            this.db = context;
        }
 
        [HttpGet]
        // GET registereddevice/
        public IEnumerable<RegisteredDevice> Get()
        {
            return db.RegisteredDevices.ToList();
        }
 
        // GET registereddevice/1
        [HttpGet("{id}")]
        public IActionResult Get(int id)
        {
            RegisteredDevice device = db.RegisteredDevices.FirstOrDefault(x => x.Id == id);
            if (device == null)
                return NotFound();
            return new ObjectResult(device);
        }
 
        // POST registereddevice
        [HttpPost]
        public IActionResult Post(string serial)
        {
            if (serial==null)
            {
                return BadRequest();
            }
            RegisteredDevice device = db.RegisteredDevices.FirstOrDefault(x => x.Serial == serial);
            if (device == null)
            {
                device = new RegisteredDevice {Serial = serial};
                db.RegisteredDevices.Add(device);
                db.SaveChanges();
            }
            Session session = new Session {Device = device, DateTime = DateTime.Now};
            db.Sessions.Add(session);
            db.SaveChanges();
            return Ok(session.Id);
        }
 
        // PUT registereddevice/
        [HttpPut]
        public IActionResult Put([FromBody]RegisteredDevice device)
        {
            if (device==null)
            {
                return BadRequest();
            }
            if (!db.RegisteredDevices.Any(x => x.Id == device.Id))
            {
                return NotFound();
            }
            
            db.Update(device);
            db.SaveChanges();
            return Ok(device);
        }
 
        // DELETE registereddevice/1
        [HttpDelete("{id}")]
        public IActionResult Delete(int id)
        {
            RegisteredDevice device = db.RegisteredDevices.FirstOrDefault(x => x.Id == id);
            if(device==null)
            {
                return NotFound();
            }
            //TODO: продумать каскадное удаление сессии при удалении устройства
            db.RegisteredDevices.Remove(device);
            db.SaveChanges();
            return Ok(device);
        }
    }
}