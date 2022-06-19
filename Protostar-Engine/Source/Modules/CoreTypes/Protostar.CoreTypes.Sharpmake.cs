using System.IO;
using Sharpmake;

namespace Protostar
{
    [Generate]
    public class CoreTypes : Module
    {
        public CoreTypes()
        {
            SourceRootPath = Path.Combine("[project.SharpmakeCsPath]", "Source");
        }
    }
}