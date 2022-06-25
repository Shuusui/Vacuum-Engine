using System.IO;
using Sharpmake;

namespace Protostar
{
    [Generate]
    public class Editor : ExecutingProject
{
        public Editor()
        {
            SourceRootPath = Path.Combine("[project.SharpmakeCsPath]", "Source");
        }
    }
}
