using System.IO;
using Sharpmake;

namespace Protostar
{
    [Generate]
    public class Engine : Module
    {
        public Engine()
        {
            Name = "Protostar-Engine";
            SourceRootPath = Path.Combine("[project.SharpmakeCsPath]", "Source");
        }
        public override void ConfigureAll(Configuration configuration, Target target)
        {
            base.ConfigureAll(configuration, target);
            configuration.SolutionFolder = "/Protostar-Engine/";
        }
    }
}