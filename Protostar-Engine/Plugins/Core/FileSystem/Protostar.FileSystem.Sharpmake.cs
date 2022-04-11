using System.IO;
using Sharpmake;

namespace Protostar
{
    [Generate]
    public class FileSystem : Module
    {
        public FileSystem()
        {
            SourceRootPath = Path.Combine("[project.SharpmakeCsPath]", "Source"); 
            PluginName = "Core";
        }
        public override void ConfigureAll(Configuration configuration, Target target)
        {
            base.ConfigureAll(configuration, target);
            configuration.ProjectPath = "[project.SharpmakeCsPath]";

        }
    }
}