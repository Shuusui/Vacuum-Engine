using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.IO;

namespace ProtostarPrebuildTool.Generator
{
    public class MacroFileGenerator : FileGenerator
    {
        public MacroFileGenerator(string _filePath, string _fileName, string[] _customLines = null, int _offset = 0)
            :base(Path.Combine(_filePath, _fileName + ".generated.h"), _customLines, _offset)
        {
            m_staticLinesOfCode.Insert(0, "#pragma once");
        }
    }
}
