using Microsoft.EntityFrameworkCore;

namespace HearthServer.Models
{
    public class HearthContext: DbContext
    {
        public DbSet<RegisteredDevice> RegisteredDevices { get; set; }
        public DbSet<Session> Sessions { get; set; }
        public DbSet<Measurement> Measurements { get; set; }

        public HearthContext(DbContextOptions<HearthContext> options) : base(options)
        {
            Database.EnsureDeleted();
            Database.EnsureCreated();
        }

        protected override void OnModelCreating(ModelBuilder modelBuilder)
        {
            modelBuilder.Entity<Measurement>()
                .HasOne(p => p.Session)
                .WithMany(t => t.Measurements)
                .OnDelete(DeleteBehavior.Cascade);
        }
    }
}