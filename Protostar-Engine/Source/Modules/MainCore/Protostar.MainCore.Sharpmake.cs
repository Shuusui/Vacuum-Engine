using System.IO;
using Sharpmake;

namespace Protostar
{
    [Generate]
    public class MainCore : Module
    {
        public MainCore()
        {
            SourceRootPath = Path.Combine("[project.SharpmakeCsPath]", "Source");
        }
    }
}
