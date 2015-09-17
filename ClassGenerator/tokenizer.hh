
#ifndef __TOKENIZER_H__
#define __TOKENIZER_H__

#include <string>
#include <vector>

class Tokenizer
{
public:
  Tokenizer(const std::string &tokenizer);

  size_t GetNumberOfTokens() const;
  const std::string &GetToken(size_t idx) const;
private:

  std::vector<std::string> m_tokens;

};

#endif /* ! __TOKENIZER_H__ */
