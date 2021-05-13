using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using ProtostarPrebuildTool.Parsing;
using System.IO;

namespace ProtostarPrebuildTool
{
    public static class SourceReader
    {
        static List<HeaderFileReader> s_Readers = new List<HeaderFileReader>();
        public static void ReadAllFiles(string _sourcePath)
        {
            List<string> files = Directory.EnumerateFiles(_sourcePath, "*.h", SearchOption.AllDirectories).ToList();
            foreach(string file in files)
            {
                s_Readers.Add(new HeaderFileReader(file));
            }
        }

        public static List<HeaderFileReader> GetHeaderFileReaders()
        {
            return s_Readers;
        }
    }
}
