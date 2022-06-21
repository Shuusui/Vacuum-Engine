using System.IO;
using Sharpmake;

namespace Protostar
{
    [Generate]
    public class Core : Module
    {
        public Core()
        {
            SourceRootPath = Path.Combine("[project.SharpmakeCsPath]", "Source");
        }
    }
}
