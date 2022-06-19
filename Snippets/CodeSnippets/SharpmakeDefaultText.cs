using System.IO;
using Sharpmake;

namespace Protostar
{
    [Generate]
    public class {ModuleName} : Module
    {
        public {ModuleName} ()
        {
            SourceRootPath = Path.Combine("[project.SharpmakeCsPath]", "Source");
        }
    }
}