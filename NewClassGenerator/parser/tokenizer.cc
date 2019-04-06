
#include <parser/tokenizer.hh>
#include <parser/sourcefile.hh>

#define PUSH(tkn) if (!tkn.empty()) { m_tokens.push_back(Token(tkn)); } tkn = ""
#define PUSH_CH(tkn, ch) PUSH(tkn); tkn += ch; PUSH(tkn)


namespace cs::classgenerator
{
Tokenizer::Tokenizer(const SourceFile& sourceFile)
{
  SourceFileIterator it = sourceFile.GetIterator();
  std::string tkn = "";
  char ch = ' ';

  bool singleChar = false;
  bool string = false;

  while (it.HasMore())
  {
    ch = it.Next();

    if (string)
    {
      tkn += ch;
      if (ch == '\\')
      {
        if (it.HasMore())
        {
          ch = it.Next();
          tkn += ch;
        }
      }
      else if (ch == '"')
      {
        string = false;
        PUSH(tkn);
      }
    }
    else if (singleChar)
    {
      tkn += ch;
      if (ch == '\\')
      {
        if (it.HasMore())
        {
          ch = it.Next();
          tkn += ch;
        }
      }
      else if (ch == '\'')
      {
        singleChar = false;
        PUSH(tkn);
      }
    }
    else
    {


      switch (ch)
      {
      case '"':
        PUSH(tkn);
        tkn += ch;
        string = true;
        break;
      case '\'':
        PUSH(tkn);
        tkn += ch;
        singleChar = true;
        break;

      case '{':
      case '}':
      case '[':
      case ']':
      case '(':
      case ')':
      case ';':
      case ',':
      case '<':
      case '&':
      case '*':
      case '+':
      case '-':
      case '=':
      case '/':
      case '~':
        PUSH(tkn);
        tkn += ch;
        break;

      case '>':
        if (tkn != "-")
        {
          PUSH(tkn);
        }
        tkn += ch;
        break;


      case ':':
        if (tkn != ":")
        {
          PUSH(tkn);
        }
        tkn += ch;
        break;
      case '.':
        if (tkn != "." && tkn != "..")
        {
          PUSH(tkn);
        }
        tkn += ch;
        break;
      case ' ':
      case '\t':
        PUSH(tkn);
        break;

      default:
        if (!IsIdentifier(tkn))
        {
          PUSH(tkn);
        }
        tkn += ch;
        break;
      }
    }
  }
  PUSH(tkn);
}


bool Tokenizer::IsIdentifier(const std::string& token) const
{

  if (token.size() == 0)
  {
    return false;
  }

  char ch = token[0];
  if (!(::isalpha(ch) || ch == '_'))
  {
    return false;
  }


  for (size_t i = 1, in = token.size(); i < in; ++i)
  {
    ch = token[0];
    if (!(::isalnum(ch) || ch == '_'))
    {
      return false;
    }
  }
  return true;
}

#if 0
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
      case '=':
      case '/':
      case '~':
      case ';':
      case ',':
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
      case '-':
        if (it.HasMore())
        {
          char next = it.Next();
          if (next == '>')
          {
            PUSH_CH(tkn, "->");
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
#endif

const std::vector<Token> &Tokenizer::GetTokens() const
{
  return m_tokens;
}



}