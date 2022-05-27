namespace HearthServer.Models 
{
    public interface ISessionRepository
    {
        void AddDevice(RegisteredDevice device);
        void UpdateDevice(RegisteredDevice device);
        void DeleteDevice(int id);
        bool IsConatinDeviceById(int id);
        bool IsConatinDeviceBySerial(string serial);
        RegisteredDevice GetDeviceById(int id);
        RegisteredDevice GetDeviceBySerial(string serial);
    }
}