using System.IO;
using Sharpmake;

namespace Protostar
{
    [Generate]
    public class CoreMath : Module
    {
        public CoreMath()
        {
            SourceRootPath = Path.Combine("[project.SharpmakeCsPath]", "Source");
        }
    }
}