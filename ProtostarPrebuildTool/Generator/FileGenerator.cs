using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.IO;

namespace ProtostarPrebuildTool.Generator
{
    public abstract class FileGenerator
    {
        protected List<string> m_staticLinesOfCode;
        protected List<string> m_dynamicLinesOfCode;
        protected HashSet<int> m_setLinesOfCode;
        protected string m_filePath;

        public FileGenerator(string _filePath, string[] _customLines, int _offset)
        {
            m_filePath = _filePath;
            m_dynamicLinesOfCode = new List<string>();
            m_setLinesOfCode = new HashSet<int>();
            if (_customLines != null)
            {
                m_staticLinesOfCode = new List<string>(new string[_offset + _customLines.Length]);
                m_staticLinesOfCode.InsertRange(_offset, _customLines);
                for (int i = 0; i < _customLines.Length; ++i)
                {
                    m_setLinesOfCode.Add(_offset + i);
                }
            }
            else
            {
                m_staticLinesOfCode = new List<string>(new string[_offset]);
            }
        }

        /*
         * Generates the file base of the lines of code added to the lists of strings
         */
        public void GenerateFile()
        {
            using (StreamWriter streamWriter = new StreamWriter(m_filePath))
            {
                List<string> linesOfCode = new List<string>(new string[m_staticLinesOfCode.Count + m_dynamicLinesOfCode.Count]);

                for (int i = 0; i < m_staticLinesOfCode.Count; ++i)
                {
                    linesOfCode.Insert(i, m_staticLinesOfCode[i]);
                }

                for (int i = 0; i < m_dynamicLinesOfCode.Count; ++i)
                {
                    linesOfCode.Insert(i, m_dynamicLinesOfCode[i]);
                }

                foreach (string s in linesOfCode)
                {
                    streamWriter.WriteLineAsync(s);
                }
            }
        }

        /* This will insert a line of code to the List of dynamic lines.
         * @param _line The line where the code should get placed. If this exceeds the limit of the dynamic line list it will dynamically grow the list and inserts the line of code in the extend of the lines
         * @param _code The code to write to the line 
         * @return If the _offset is already contained it will return false, true otherwise
         */
        protected bool InsertDynamicLineOfCode(int _line, string _code)
        {
            if (m_setLinesOfCode.Contains(_line))
            {
                return false;
            }
            if(_line > m_dynamicLinesOfCode.Count)
            {
                m_dynamicLinesOfCode.AddRange(new string[_line - m_dynamicLinesOfCode.Count]);
            }
            m_dynamicLinesOfCode.Insert(_line, _code);

            return true;
        }

        /* Inserts a range of code lines to the dynamic lines of code list
         * @param _offset The offset where to insert the range of code lines. If the offset exceeds the limit of the list it will grow the list dynamically and inserts the lines of code.
         * @param _code The lines of code to insert to the list.
         * @return If the _offset is already contained it will return false, true otherwise
         */
        protected bool InsertDynamicLinesOfCode(int _offset, string[] _code)
        {
            if (m_setLinesOfCode.Contains(_offset))
            {
                return false;
            }

            if (_offset > m_dynamicLinesOfCode.Count)
            {
                m_dynamicLinesOfCode.AddRange(new string[_offset - m_dynamicLinesOfCode.Count]);
            }
            m_dynamicLinesOfCode.InsertRange(_offset, _code);
            return true;
        }

        /* Returns the next free line of code 
         * @param _offset The offset where to start looking 
         * @return The next free line of code
         */
        public int GetNextLineFromOffset(int _offset)
        {
            if(!m_setLinesOfCode.Contains(_offset))
            {
                return _offset;
            }
            return GetNextLineFromOffset(_offset + 1);
        }
    }
}
