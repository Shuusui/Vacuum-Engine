using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace ProtostarPrebuildTool
{
    public class SharedDefinitions
    {
        private static readonly string s_macroDeclaration = "#define ";
        public static readonly string s_propertyMacro = s_macroDeclaration + "PROPERTY()";

        public static string[] GetAllMacroDefinitions()
        {
            return new string[]
            {
                s_propertyMacro
            };
        }
    }
}
