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
        private static readonly string s_macroBody = "(...)";
        public static readonly string s_editorSourceDir = Path.Combine(s_rootDir, "Protostar-Editor");
        public static readonly string s_engineSourceDir = Path.Combine(s_rootDir, "Protostar-Engine");
        public static readonly string s_propertyDeclarationName = "PSPROPERTY";
        public static readonly string s_objectDeclarationName = "PSOBJECT";
        public static readonly string s_propertyDeclaration = s_propertyDeclarationName + "(";
        public static readonly string s_objectDeclaration = s_objectDeclarationName + "(";
        public static readonly string s_propertMacroWithoutBody = s_macroDeclaration + s_propertyDeclarationName;
        public static readonly string s_objectMacroWithoutBody = s_macroDeclaration + s_objectDeclarationName;
        public static readonly string s_propertyMacro = s_macroDeclaration + s_propertyDeclarationName + s_macroBody;
        public static readonly string s_objectMacro = s_macroDeclaration + s_objectDeclarationName + s_macroBody;

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
