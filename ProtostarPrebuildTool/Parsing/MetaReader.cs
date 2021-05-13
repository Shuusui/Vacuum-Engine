using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace ProtostarPrebuildTool.Parsing
{
    enum MetadataType
    {
        NAMESPACE,
    }
    public abstract class Metadata
    {
        public CodeBodyRange Range;
        public bool HasOwnBody { get; set; }

        protected Metadata()
        {
            Range = new CodeBodyRange(-1, -1);
            HasOwnBody = false;
        }
        abstract public string[] GetLinesOfCode();
    }
    public class NamespaceMetadata : Metadata
    {
        public NamespaceMetadata()
            :base()
        {
            HasOwnBody = true;
        }
        public List<string> Namespaces = new List<string>();

        public override string[] GetLinesOfCode()
        {
            if (Namespaces.Count > 0)
            {
                StringBuilder sb = new StringBuilder("namespace ");
                foreach (string ns in Namespaces)
                {
                    sb.Append(ns);
                    if (ns != Namespaces.Last())
                    {
                        sb.Append("::");
                        continue;
                    }
                }
                return new string[]
                    {
                        sb.ToString(),
                        Util.GetClosingBracket(),
                        Util.GetClosingBracket()
                    };
            }
            return new string[]
                {
                    Util.GetOpeningBracket(),
                    Util.GetClosingBracket()
                };
        }
    }
    public class Metadatas
    {
        public List<Metadata> MetadataList = new List<Metadata>();
        public bool IsNextOpeningBracketForMetadata()
        {
            if(MetadataList.Count <= 0)
            {
                return false;
            }
            Metadata last = MetadataList.Last();
            return last.Range.Start == -1 && last.HasOwnBody;
        }
        public bool IsNextClosingBracketForMetadata()
        {
            if (MetadataList.Count <= 0)
            {
                return false;
            }
            Metadata last = MetadataList.Last();
            return last.Range.End == -1 && last.HasOwnBody;
        }
        public void SetStartRange(int _startLine)
        {
            MetadataList.Last().Range.Start = _startLine;
        }
        public void SetEndRange(int _endLine)
        {
            MetadataList.Last().Range.End = _endLine;
        }
        public void RemoveLastMetadata()
        {
            MetadataList.RemoveAt(MetadataList.Count - 1);
        }
    }
    public class MetaReader
    {
        private MetadataType m_nextMetadataToRead;
        public bool ContainsMetaKeyword(string _line)
        {
            string[] metadataTypes = Enum.GetNames(typeof(MetadataType));
            
            for(int i = 0; i < metadataTypes.Length; ++i)
            {
                if (_line.Contains(metadataTypes[i].ToLower()))
                {
                    m_nextMetadataToRead = (MetadataType)i;
                    return true;
                }
            }

            return false;
        }

        private int GetMetadataStartIndex(string _line, string _metadataString)
        {
            int indexOf = _line.IndexOf(_metadataString);
            if (indexOf == -1)
            {
                return -1;
            }
            return indexOf + m_nextMetadataToRead.ToString().Length + 1;
        }

        public void ReadMetadata(string _line, in StreamReader _streamReader, in Metadatas _metadata)
        {
            switch(m_nextMetadataToRead)
            {
                case MetadataType.NAMESPACE:
                    NamespaceMetadata nsMetadata = new NamespaceMetadata();
                    string metadataName = MetadataType.NAMESPACE.ToString().ToLower();
                    if(Util.IsStringInLineCommented(_line, metadataName))
                    {
                        return;
                    }
                    int startMetadata = GetMetadataStartIndex(_line, metadataName);
                    if(startMetadata == -1)
                    {
                        return;
                    }
                    ReadNamespaces(startMetadata, _line, nsMetadata);
                    _metadata.MetadataList.Add(nsMetadata);
                    break;
                default:
                    Debug.Assert(true, "Unrecognized metadata keyword");
                    break;
            }
        }
        private void ReadNamespaces(int _startNamespace, string _line, in NamespaceMetadata _nsMetadata)
        {
            StringBuilder sb = new StringBuilder(_line.Length - _startNamespace);
            for (int i = _startNamespace; i < _line.Length; ++i)
            {
                char c = _line[i];
                if (!char.IsWhiteSpace(c) && c != ':')
                {
                    sb.Append(c);
                }
                else
                {
                    if(c == ':' && _line[i + 1] == ':')
                    {
                        _nsMetadata.Namespaces.Add(sb.ToString());
                        sb.Clear();
                    }
                }
            }
            if(sb.Length > 0)
            {
                _nsMetadata.Namespaces.Add(sb.ToString());
                sb.Clear();
            }
        }
    }
}
