using System.IO;
using Sharpmake;

namespace Protostar
{
    [Generate]
    public class CoreMain : Module
    {
        public CoreMain()
        {
            SourceRootPath = Path.Combine("[project.SharpmakeCsPath]", "Source");
        }
    }
}