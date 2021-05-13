using System;

namespace ProtostarPrebuildTool
{
    class Program
    {
        static void Main(string[] args)
        {
            StaticallyGeneratedFiles.GenerateFiles();
            SourceReader.ReadAllFiles(SharedDefinitions.s_engineSourceDir);
            SourceWriter.WriteSourceFiles(SourceReader.GetHeaderFileReaders());
        }
    }
}
