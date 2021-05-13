using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.IO;

namespace ProtostarPrebuildTool.Parsing
{
    public class HeaderFileReader
    {
#pragma warning disable 0649
        public readonly List<ObjectReader> m_readObjects;
#pragma warning restore 0649
        public string m_fileName;
        public HeaderFileReader(string _filePath)
        {
            m_fileName = Path.GetFileNameWithoutExtension(_filePath);
            m_readObjects = new List<ObjectReader>();
            Metadatas metadatas = new Metadatas();

            using (StreamReader reader = new StreamReader(_filePath))
            {
                MetaReader metaReader = new MetaReader();
                bool bIsInMultiLineComment = false;
                int lineIndex = 0;
                while (reader.Peek() >= 0)
                {
                    string line = reader.ReadLine();
                    if(bIsInMultiLineComment)
                    {
                        if(Util.ContainsMultiLineCommentEnd(line))
                        {
                            bIsInMultiLineComment = false;
                            if(!Util.IsContentAfterMultiLineCommentEnd(line))
                            {
                                continue;
                            }
                        }
                    }
                    else
                    {
                        if(Util.ContainsMultiLineCommentStart(line))
                        {
                            bIsInMultiLineComment = true;
                            if(!Util.IsContentBeforeMultiLineCommentStart(line))
                            {
                                continue;
                            }
                        }
                    }
                    //TODO: Needs to consider inner brackets inside namespaces. 
                    if(Util.ContainsOpeningBracket(line) && metadatas.IsNextOpeningBracketForMetadata()) 
                    {
                        metadatas.SetStartRange(lineIndex);
                        NamespaceMetadata nsMetadata = (NamespaceMetadata)metadatas.MetadataList.Last();
                        if (nsMetadata.Namespaces.Last() == "Protostar")
                        {
                            Console.WriteLine(nsMetadata.Namespaces.Last());
                        }
                    }
                    if(Util.ContainsClosingBracket(line) && metadatas.IsNextClosingBracketForMetadata())
                    {
                        metadatas.SetEndRange(lineIndex);
                        metadatas.RemoveLastMetadata();
                    }
                    if(metaReader.ContainsMetaKeyword(line))
                    {
                        metaReader.ReadMetadata(line, reader, metadatas);
                    }
                    if (line.Contains(SharedDefinitions.s_objectDeclaration))
                    {
                        ObjectReader objectReader = new ObjectReader(metadatas);
                        objectReader.ReadName(reader);
                        objectReader.ReadMacroContent(reader);
                        m_readObjects.Add(objectReader);
                    }
                    lineIndex++;
                }
            }
        }
        public bool HasData()
        {
            return m_readObjects.Count > 0;
        }
    }
}
