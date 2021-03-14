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
        public HeaderFileGenerator(string _filePath, string[] _customLines = null, int _offset = 0)
            :base(_filePath, _customLines, _offset)
        {
            m_staticLinesOfCode.Insert(0, "#pragma once");
        }
        public void GenerateFile()
        {
            using(StreamWriter streamWriter = new StreamWriter(m_filePath))
            {
                foreach(string s in m_staticLinesOfCode)
                {
                    streamWriter.WriteLineAsync(s);
                }
            }
        }
    }
}
