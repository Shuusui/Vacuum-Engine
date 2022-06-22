using System.IO;
using Sharpmake;

namespace Protostar
{
    [Generate]
    public class {ModuleName} : ExecutingProject
{
        public {ModuleName}()
        {
            SourceRootPath = Path.Combine("[project.SharpmakeCsPath]", "Source");
        }
    }
}