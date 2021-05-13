using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace ProtostarPrebuildTool
{
    public static class Util
    {
        public static int GetIndexOfCommentInLine(string _line)
        {
            int commentIndex = _line.IndexOf("//");
            if(commentIndex != -1)
            {
                return commentIndex;
            }
            return _line.IndexOf("/*");
        }
        public static string GetSingleLineCommentString()
        {
            return "//";
        }
        public static string GetMultiLineCommentString()
        {
            return "/*";
        }
        public static string GetOpeningBracket()
        {
            return "{";
        }
        public static string GetClosingBracket()
        {
            return "}";
        }
        public static string[] GetCommentStrings()
        {
            return new string[]
            {
                GetSingleLineCommentString(), 
                GetMultiLineCommentString()
            };
        }
        public static string GetClosingCommentString()
        {
            return "*/";
        }
        public static bool IsStringBeforeString(string _line, string _first, string _second)
        {
            int indexOfFirst = _line.IndexOf(_first);
            int indexOfSecond = _line.IndexOf(_second);

            if(indexOfFirst == -1 || indexOfSecond == -1)
            {
                return false;
            }

            return indexOfFirst < indexOfSecond;
        }
        public static int[] GetIndicesOfStringsInString(string[] _strings, string _str)
        {
            int[] indices = new int[_strings.Length];
            for(int i = 0; i < _strings.Length; ++i)
            {
                indices[i] = _str.IndexOf(_strings[i]);
            }
            return indices;
        }
        /*This will return true only if all _toCheck strings are before the _against string in _line but only if the string of
         * _toCheck strings is in _line, if not they will get skipped. 
         */
        public static bool AreStringsBeforeString(string _line, string[] _toCheck, string _against)
        {
            int[] indices = GetIndicesOfStringsInString(_toCheck, _line);
            int againstIndex = _line.IndexOf(_against);
            if(againstIndex == -1)
            {
                return false;
            }
            bool bIsAtleastOneStringContained = false;
            foreach(int index in indices)
            {
                if(index == -1)
                {
                    continue;
                }
                bIsAtleastOneStringContained = true;
                if(index > againstIndex)
                {
                    return false;
                }
            }
            return bIsAtleastOneStringContained;
        }
        public static bool IsStringInLineCommented(string _line, string _against)
        {
            return AreStringsBeforeString(_line, GetCommentStrings(), _against);
        }

        public static bool ContainsMultiLineCommentStart(string _line)
        {
            return _line.Contains(GetMultiLineCommentString());
        }

        public static bool IsContentBeforeMultiLineCommentStart(string _line)
        {
            int indexOfComment = _line.IndexOf(GetMultiLineCommentString());
            if(indexOfComment == -1)
            {
                return false;
            }
            return indexOfComment != 0;
        }

        public static bool ContainsMultiLineCommentEnd(string _line)
        {
            return _line.Contains(GetClosingCommentString());
        }
        public static bool IsContentAfterMultiLineCommentEnd(string _line)
        {
            int indexOfComment = _line.IndexOf(GetClosingCommentString());
            if(indexOfComment == -1)
            {
                return false;
            }
            return indexOfComment == _line.Length - 2;
        }
        public static bool ContainsOpeningBracket(string _line)
        {
            return _line.Contains(GetOpeningBracket());
        }
        public static bool ContainsClosingBracket(string _line)
        {
            return _line.Contains(GetClosingBracket());
        }
    }
}
