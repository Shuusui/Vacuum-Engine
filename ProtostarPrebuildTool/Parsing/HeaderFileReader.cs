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
        private List<ObjectReader> m_generatedMacroCodes;
        public HeaderFileReader(string _filePath)
        {
            m_generatedMacroCodes = new List<ObjectReader>();
            using (StreamReader reader = new StreamReader(_filePath))
            {
                MetaReader metaReader = new MetaReader();
                while (reader.Peek() >= 0)
                {
                    string line = reader.ReadLine();
                    Metadata metadata = new Metadata();

                    if(metaReader.ContainsMetaKeyword(line))
                    {
                        metaReader.ReadMetadata(line, reader, metadata);
                    }

                    if (line.Contains(SharedDefinitions.s_objectMacro))
                    {
                        ObjectReader structReader = new ObjectReader(metadata);
                        structReader.ReadName(reader);
                        structReader.ReadMacroContent(reader);
                        m_generatedMacroCodes.Add(structReader);
                    }
                }
            }
        }
    }
}
