using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using ProtostarPrebuildTool.Generator;

namespace ProtostarPrebuildTool
{
    public class StaticallyGeneratedFiles
    {
        static public void GenerateFiles()
        {
            HeaderFileGenerator macroHeaderFileGenerator = new HeaderFileGenerator(@"E:\Repositories\Protostar-Engine\intermediate\PrebuildMacros.generated.h", SharedDefinitions.GetAllMacroDefinitions(), 3);
            macroHeaderFileGenerator.GenerateFile();
        }
    }
}
