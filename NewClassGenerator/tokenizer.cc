
#include "tokenizer.hh"
#include "sourcefile.hh"

#define PUSH(tkn) if (!tkn.empty()) { m_tokens.push_back(tkn); } tkn = ""
#define PUSH_CH(tkn, ch) PUSH(tkn); tkn += ch; PUSH(tkn)

namespace cs::classgenerator
{


Tokenizer::Tokenizer(const SourceFile& sourceFile)
{
  SourceFileIterator it = sourceFile.GetIterator();
  std::string tkn = "";
  char last = ' ';
  char ch = ' ';

  bool singleChar = false;
  bool string = false;

  bool readNext = true;
  while (!readNext || it.HasMore())
  {
    if (readNext)
    {
      ch = it.Next();
    }
    readNext = true;

    if (string)
    {
      tkn += ch;
      if (ch == '\\')
      {
        if (it.HasMore())
        {
          char next = it.Next();
          tkn += next;
        }
      }
      else if ()c
    }
    else
    {

      switch (ch)
      {
      case '{':
      case '}':
      case '[':
      case ']':
      case '(':
      case ')':
      case '<':
      case '>':
      case '&':
      case '*':
      case '+':
      case '-':
      case '=':
      case '/':
      case '~':
      case ';':
        PUSH_CH(tkn, ch);
        break;

      case ':':
        if (it.HasMore())
        {
          char next = it.Next();
          if (next == ':')
          {
            PUSH_CH(tkn, "::");
          }
          else
          {
            PUSH_CH(tkn, ch);
            ch = next;
            readNext = false;
          }
        }
        else
        {
          PUSH_CH(tkn, ch);
        }
        break;
      case ' ':
      case '\t':
        PUSH(tkn);
        break;

      case '\'':
      default:
        tkn += ch;
        break;
      }
    }
  }
  PUSH(tkn);
}


const std::vector<std::string> &Tokenizer::GetTokens() const
{
  return m_tokens;
}



}