using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.IO;
using ProtostarPrebuildTool.Generator;

namespace ProtostarPrebuildTool
{
    public static class StaticallyGeneratedFiles
    {
        private static List<FileGenerator> FileGenerators;

        static StaticallyGeneratedFiles()
        {
            FileGenerators = new List<FileGenerator>();
            FileGenerators.Add(new MacroFileGenerator(SharedDefinitions.GetGeneratedFilesDir(), "PreBuildMacros", SharedDefinitions.GetAllMacroDefinitions(), 2));
        }
        public static void GenerateFiles()
        {
            foreach (FileGenerator fileGenerator in FileGenerators)
            {
                fileGenerator.GenerateFile();
            }
        }
    }
}
