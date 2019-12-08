using System.Collections.Generic;
using System.Linq;
using Microsoft.EntityFrameworkCore;
using Microsoft.AspNetCore.Mvc;
using HearthServer.Models;
using Newtonsoft.Json;
 
namespace HearthServer.Controllers
{
    [Route("[controller]")]
    public class MeasurementController : Controller
    {
        HearthContext db;
        public MeasurementController(HearthContext context)
        {
            this.db = context;
        }
 
        [HttpGet]
        //Route session/GetBySessionId/?SessionId=1"
        public IActionResult  GetBySessionId(int SessionId)
        {
            var session = db.Sessions.FirstOrDefault(x => x.Id == SessionId);
            if (session == null)
                return NotFound();
            var measurements = (from Measurement in db.Measurements where Measurement.Session == session select Measurement).ToList();
            return new JsonResult(measurements);
        }
 
        [HttpPost]
        public IActionResult  Post(int SessionId, Measurement measurement)
        {
            var session = db.Sessions.FirstOrDefault(x => x.Id == SessionId);
            if (session == null)
                return NotFound();
            measurement.Session = session;
            db.Measurements.Add(measurement);
            db.SaveChanges();
            return Ok();
        }
    }
}