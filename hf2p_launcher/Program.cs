using System;
using System.Diagnostics;
using System.IO;

using Lunar;
using Newtonsoft.Json;

namespace hf2p_launcher
{
    public class ConfigurationStructure
    {
        public string username { get; set; }
        public string password { get; set; }
        public string environment { get; set; }
    }

    public class Program
    {
        private static Process _process;
        private static LibraryMapper _mapper;

        private static string _username;
        private static string _password;
        private static string _environment;
        private const string _dllpath = "hf2p_dedicated_server.dll";

        private const MappingFlags _flags = MappingFlags.DiscardHeaders;

        private static int Main(string[] args)
        {
            if (!ReadConfig())
            {
                if (args.Length == 0)
                {
                    Console.WriteLine("ERROR: you must provide 2 arguments (hint: username and password).");

                    return -1;
                }

                _username = args[0];
                _password = args[1];
                _environment = args[2];
            }

            if (!string.IsNullOrWhiteSpace(_username) && !string.IsNullOrWhiteSpace(_password) && !string.IsNullOrWhiteSpace(_environment))
            {
                _process = new Process();
                _process.StartInfo.FileName = "halo_online.exe";
                _process.StartInfo.Arguments = $"--account {_username} --sign-in-code {_password} --environment {_environment}";
                _process.StartInfo.WorkingDirectory = Environment.CurrentDirectory;
                _process.Start();

                _mapper = new LibraryMapper(_process, _dllpath, _flags);
                _mapper.MapLibrary();

                return 0;
            }

            Console.WriteLine("ERROR: no server credentials provided.");

            return -1;
        }

        private static bool ReadConfig()
        {
            string json = File.ReadAllText("dedi_config.json");

            ConfigurationStructure obj = JsonConvert.DeserializeObject<ConfigurationStructure>(json);

            if (obj != null)
            {
                _username = obj.username;
                _password = obj.password;
                _environment = obj.environment;

                return true;
            }

            return false;
        }
    }
}
