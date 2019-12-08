using System;
using System.Collections.Generic;

namespace HearthServer.Models
{
    public class Session
    {
        public int Id { get; set; }
        public RegisteredDevice Device { get; set; }
        public DateTime DateTime { get; set; }

        public List<Measurement> Measurements { get; set; }
    }
}