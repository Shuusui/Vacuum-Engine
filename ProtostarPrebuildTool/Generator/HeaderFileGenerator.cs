using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.IO;
using ProtostarPrebuildTool.Parsing;

namespace ProtostarPrebuildTool.Generator
{
    public class HeaderFileGenerator : FileGenerator
    {
        public const int m_namespaceLine = 1;
        public CodeBodyRange m_codeRange;
        public HeaderFileGenerator(string _filePath, string _fileName, string[] _customLines = null, int _offset = 1)
            :base(Path.Combine(_filePath, _fileName + ".generated.h"), _customLines, _offset)
        {
            string[] linesOfCode = new string[]
            {
                "#pragma once",
            };

            m_staticLinesOfCode.InsertRange(0, linesOfCode);
            for(int i = 0; i < linesOfCode.Length; ++i)
            {
                m_setLinesOfCode.Add(i);
            }
            m_codeRange = new CodeBodyRange(-1, -1);
        }

        public void InsertCode(HeaderFileReader _reader)
        {
            foreach (ObjectReader objReader in _reader.m_readObjects)
            {
                foreach(Metadata metadata in objReader.m_metadatas.MetadataList)
                {
                    if(metadata.HasOwnBody)
                    {
                        string[] codeLines = metadata.GetLinesOfCode();
                        int lineIndex;
                        if (m_codeRange.IsValid())
                        {
                            lineIndex = m_codeRange.End + 3;
                        }
                        else
                        {
                            lineIndex = 3;
                        }
                        m_codeRange.Start = lineIndex;
                        m_codeRange.End = lineIndex;
                        InsertDynamicLinesOfCode(lineIndex, codeLines);
                    }
                }
            }
        }
        public bool InsertCodeLineInsideLastCodeBrackets(string _code)
        {
            if(!InsertDynamicLineOfCode(m_codeRange.End, _code))
            {
                return false;
            }
            m_codeRange.End++;
            return true;
        }

        public bool InsertCodeLinesInsideLastCodeBrackets(string[] _code)
        {
            if(!InsertDynamicLinesOfCode(m_codeRange.End, _code))
            {
                return false;
            }
            m_codeRange.End += _code.Length;
            return true;
        }
    }
}
