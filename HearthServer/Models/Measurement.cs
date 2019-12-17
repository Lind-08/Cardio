using System;
using Newtonsoft.Json;

namespace HearthServer.Models
{
    public class Measurement
    {
        public int id { get; set; }
        public int Value { get; set; }

        [JsonIgnore]
        public Session Session { get; set; }
        public int  MillisDelta { get; set; }
    }
}