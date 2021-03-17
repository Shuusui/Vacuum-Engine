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
    public abstract class MacroReader
    {
        public abstract bool ReadName(in StreamReader _streamReader);
        public abstract void ReadMacroContent(in StreamReader _streamReader);

        public List<Property> m_properties { get; set; }

        public string m_name { get; set; }
    }
}
