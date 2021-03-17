using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.IO;

namespace ProtostarPrebuildTool.Generator
{
    public class HeaderFileGenerator : FileGenerator
    {
        public HeaderFileGenerator(string _filePath, string _fileName, string[] _customLines = null, int _offset = 1)
            :base(Path.Combine(_filePath, _fileName + ".generated.h"), _customLines, _offset)
        {
            string[] linesOfCode = new string[]
            {
                "#pragma once",
                "", 
                "namespace Protostar", 
                "{",
                "}"
            };

            m_staticLinesOfCode.InsertRange(0, linesOfCode);
            for(int i = 0; i < linesOfCode.Length; ++i)
            {
                m_setLinesOfCode.Add(i);
            }
            m_lineStart = 4;
        }
    }
}
