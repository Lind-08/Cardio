using System;

namespace HearthServer.Models
{
    public class RegisteredDevice
    {
        public int Id { get; set; }
        public Guid Serial { get; set; }
    }
}