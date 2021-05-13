using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using ProtostarPrebuildTool.Parsing;
using ProtostarPrebuildTool.Generator;

namespace ProtostarPrebuildTool
{
    public static class SourceWriter
    {
        public static void WriteSourceFiles(List<HeaderFileReader> _headerFileReaders)
        {
            foreach(HeaderFileReader reader in _headerFileReaders)
            {
                if(!reader.HasData())
                {
                    continue;
                }
                HeaderFileGenerator generator = new HeaderFileGenerator(SharedDefinitions.GetGeneratedFilesDir(), reader.m_fileName);
                generator.InsertCode(reader);
                generator.GenerateFile();
            }
        }
    }
}
