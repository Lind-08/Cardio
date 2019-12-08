using Microsoft.EntityFrameworkCore;

namespace HearthServer.Models
{
    public class HearthContext: DbContext
    {
        public DbSet<RegisteredDevice> RegisteredDevices { get; set; }
        public DbSet<Session> Sessions { get; set; }
        public DbSet<Measurement> Measurements { get; set; }

        public HearthContext()
        {
            Database.EnsureDeleted();
            Database.EnsureCreated();
        }
    }
}