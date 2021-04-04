using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.IO;

namespace ProtostarPrebuildTool
{
    public class SharedDefinitions
    {
        private static readonly string s_currentDir = Directory.GetCurrentDirectory();
        private static readonly string s_rootDir = s_currentDir.Replace(@"bin\Debug", "").Replace(@"ProtostarPrebuildTool", "");
        private static readonly string s_generatedFilesDir = Path.Combine(s_rootDir, "intermediate", "Project", "GeneratedFiles");
        private static readonly string s_macroDeclaration = "#define ";
        public static readonly string s_propertyMacro = s_macroDeclaration + "PROPERTY()";
        public static readonly string s_objectMacro = s_macroDeclaration + "OBJECT()";

        public static string[] GetAllMacroDefinitions()
        {
            return new string[]
            {
                s_propertyMacro,
                s_objectMacro
            };
        }
        public static string GetGeneratedFilesDir()
        {
            return s_generatedFilesDir;
        }
    }
}
