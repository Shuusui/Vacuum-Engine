using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.IO;

namespace ProtostarPrebuildTool.Parsing
{
    public class HeaderFileReader
    {
        private List<MacroReader> m_generatedMacroCodes;
        public HeaderFileReader(string _filePath)
        {
            using (StreamReader reader = new StreamReader(_filePath))
            {
                while (reader.Peek() >= 0)
                {
                    string line = reader.ReadLine();

                    if (line.Contains(SharedDefinitions.s_structMacro))
                    {
                        StructReader structReader = new StructReader();
                        structReader.ReadName(reader);
                        structReader.ReadMacroContent(reader);
                        m_generatedMacroCodes.Add(structReader);
                    }

                    if (line.Contains(SharedDefinitions.s_classMacro))
                    {

                    }
                }
            }
        }
    }
}
