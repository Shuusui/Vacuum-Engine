using System.IO;
using Sharpmake;

namespace Protostar
{
    [Generate]
    public class RHI : Module
    {
        public RHI()
        {
            SourceRootPath = Path.Combine("[project.SharpmakeCsPath]", "Source");
        }
    }
}
