#ifndef TOKENSTREAM_HPP
#define TOKENSTREAM_HPP

class TokenStream {
private:
  const std::string code;
  int index {0};
  std::optional<Token> nextToken {};
  void queueNext();

  std::string takeWhitespace();
  std::string takeComment();
  std::string takeGrouper();
  std::string takeIdentifier();
  std::string takeNumber();
  std::string takeOperator();
  std::string takeString();
  std::string takeEscape();
public:
  TokenStream(std::string code);
  Token peek();
  Token next();
  bool hasNext();
};

#endif
