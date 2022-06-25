using System.IO;
using Sharpmake;

namespace Protostar
{
    [Generate]
    public class GUI : Module
    {
        public GUI()
        {
            SourceRootPath = Path.Combine("[project.SharpmakeCsPath]", "Source");
        }
    }
}
