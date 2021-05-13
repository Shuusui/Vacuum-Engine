using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.IO;

namespace ProtostarPrebuildTool.Parsing
{
    public struct Property
    {
        public string Name;
    }

    public class ObjectReader
    {
        public List<Property> m_properties { get; set; }
        public string m_name { get; set; }
        public Metadatas m_metadatas;
        public ObjectReader(Metadatas _metadatas)
        {
            m_metadatas = _metadatas;
        }
        public bool ReadName(in StreamReader _streamReader)
        {
            string line = "";

            int indexOf = -1;
            string cppStruct = new string("struct");
            string cppClass = new string("class");
            bool bIsStruct = false;
            if (line.Contains(cppStruct))
            {
                while (indexOf == -1)
                {
                    line = _streamReader.ReadLine();
                    indexOf = line.IndexOf(cppStruct);
                    if (_streamReader.Peek() < 0)
                    {
                        return false;
                    }
                }
                bIsStruct = true;
            }
            else if (line.Contains(cppClass))
            {
                while (indexOf == -1)
                {
                    line = _streamReader.ReadLine();
                    indexOf = line.IndexOf(cppClass);
                    if(_streamReader.Peek() < 0)
                    {
                        return false;
                    }
                }
                bIsStruct = false;
            }
            else 
            {
                return false;
            }

            int startObject = bIsStruct ? indexOf + cppStruct.Length + 1 : indexOf + cppClass.Length + 1;
            StringBuilder sb = new StringBuilder(line.Length - startObject);
            for (int i = startObject; i < line.Length; ++i)
            {
                sb.Append(line[i]);
            }
            m_name = sb.ToString();
            return true;
        }

        public void ReadMacroContent(in StreamReader _streamReader)
        {
            string line;
            while (_streamReader.Peek() >= 0)
            {
                line = _streamReader.ReadLine();
                if (line.Contains(Util.GetOpeningBracket()))
                {
                    break;
                }
            }
            while(_streamReader.Peek() >= 0)
            {
                line = _streamReader.ReadLine();
                if(line.Contains(Util.GetClosingBracket()))
                {
                    break;
                }

                if(line.Contains(SharedDefinitions.s_propertyMacro))
                {
                    ReadProperty(line, _streamReader);
                }
            }
        }

        private void ReadProperty(string _currentLine, in StreamReader _streamReader)
        {
            _currentLine = _streamReader.ReadLine();
            int indexAfterFirstWord = GetIndexAfterFirstWord(_currentLine);
            if(indexAfterFirstWord == -1)
            {
                return;
            }
            Property property = new Property();
            StringBuilder sb = new StringBuilder(_currentLine.Length - indexAfterFirstWord);
            for(int i = indexAfterFirstWord + 1; i < _currentLine.Length; ++i)
            {
                if (_currentLine[i] != ';')
                {
                    sb.Append(_currentLine[i]);
                    continue;
                }
                property.Name = sb.ToString();
            }
            m_properties.Add(property);
        }

        private int GetIndexAfterFirstWord(string _line)
        {
            bool bFoundWord = false;
            for(int i = 0; i < _line.Length; ++i)
            {
                if (!char.IsWhiteSpace(_line[i]))
                {
                    if (!bFoundWord)
                    {
                        bFoundWord = true;
                        continue;
                    }
                }
                else
                {
                    if (bFoundWord)
                    {
                        return i;
                    }
                }
            }
            return -1;
        }
    }
}
