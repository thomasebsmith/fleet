class ParseError: public std::runtime_error {
public:
  ParseError(std::string message);
  virtual const char *what() const throw();
};
