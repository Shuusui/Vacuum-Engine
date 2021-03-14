using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace ProtostarPrebuildTool.Generator
{
    public class FileGenerator
    {
        protected List<string> m_staticLinesOfCode;
        protected string m_filePath;

        public FileGenerator(string _filePath, string[] _customLines, int _offset)
        {
            m_filePath = _filePath;

            m_staticLinesOfCode = new List<string>(new string[_offset + _customLines.Length]);
            m_staticLinesOfCode.InsertRange(_offset, _customLines);
        }
    }
}
