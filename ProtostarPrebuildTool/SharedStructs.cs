using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace ProtostarPrebuildTool
{
    public struct CodeBodyRange
    {
        //Including scope beginning bracket line
        public int Start { get; set; }
        //Excluding scope ending bracket line
        public int End { get; set; }

        public CodeBodyRange(int _start, int _end)
        {
            Start = _start;
            End = _end;
        }

        public bool IsValid()
        {
            return Start != -1 && End != -1;
        }
    }
}
