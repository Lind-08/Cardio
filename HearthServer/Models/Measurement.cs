using System;

namespace HearthServer.Models
{
    public class Measurement
    {
        public int id { get; set; }
        public int Value { get; set; }
        public Session Session { get; set; }
        public DateTime Timestamp { get; set; }
    }
}