using System.IO;
using Sharpmake;

namespace Protostar
{
    [Generate]
    public class Management : Module
    {
        public Management()
        {
            SourceRootPath = Path.Combine("[project.SharpmakeCsPath]", "Source");
        }
    }
}
