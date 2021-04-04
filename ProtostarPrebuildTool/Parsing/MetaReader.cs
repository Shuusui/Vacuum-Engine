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
    public struct NamespaceMetadata
    {
        public List<string> Namespaces;
    }
    public class Metadata
    {
        public List<NamespaceMetadata> NamepsaceMetadata;
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

        public void ReadMetadata(string _line, in StreamReader _streamReader, in Metadata _metadata)
        {
            switch(m_nextMetadataToRead)
            {
                case MetadataType.NAMESPACE:
                    NamespaceMetadata nsMetadata = new NamespaceMetadata();
                    string metadataName = MetadataType.NAMESPACE.ToString().ToLower();
                    int startMetadata = GetMetadataStartIndex(_line, metadataName);
                    ReadNamespaces(startMetadata, _line, nsMetadata);
                    _metadata.NamepsaceMetadata.Add(nsMetadata);
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
