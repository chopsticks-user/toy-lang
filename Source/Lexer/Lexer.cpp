#include "Lexer.hpp"

namespace tl::lexer {
  auto Lexer::operator()(std::string filepath) -> Tokens {
    auto reader = FileReader(std::move(filepath));
    Tokens tokens;

    while (!reader.eof()) {
      auto tokenType = EToken::Empty;
      std::string tokenStr;

      auto c = reader.advance();
      tokenStr += c;

      switch (c) {
        case '(': {
          tokenType = EToken::LeftParen;
          break;
        }
        case ')': {
          tokenType = EToken::RightParen;
          break;
        }
        case '[': {
          tokenType = EToken::LeftBracket;
          break;
        }
        case ']': {
          tokenType = EToken::RightBracket;
          break;
        }
        case '{': {
          tokenType = EToken::LeftBrace;
          break;
        }
        case '}': {
          tokenType = EToken::RightBrace;
          break;
        }
        case '.': {
          tokenType = EToken::Dot;
          if (reader.match('.', '.')) {
            tokenType = EToken::Dot3;
            tokenStr += "..";
          }
          break;
        }
        case ',': {
          tokenType = EToken::Comma;
          break;
        }
        case ':': {
          tokenType = EToken::Colon;
          if (reader.match(':')) {
            tokenType = EToken::Colon2;
            tokenStr += ':';
          }
          break;
        }
        case ';': {
          tokenType = EToken::Semicolon;
          break;
        }
        case '*': {
          tokenType = EToken::Star;
          if (reader.match('*')) {
            tokenType = EToken::Star2;
            tokenStr += '*';
          } else if (reader.match('=')) {
            tokenType = EToken::StarEqual;
            tokenStr += '=';
          }
          break;
        }
        case '&': {
          tokenType = EToken::Ampersand;
          if (reader.match('&')) {
            tokenType = EToken::Ampersand2;
            tokenStr += '&';
          } else if (reader.match('=')) {
            tokenType = EToken::AmpersandEqual;
            tokenStr += '=';
          }
          break;
        }
        case '|': {
          tokenType = EToken::Bar;
          if (reader.match('|')) {
            tokenType = EToken::Bar2;
            tokenStr += '|';
          } else if (reader.match('=')) {
            tokenType = EToken::BarEqual;
            tokenStr += '=';
          } else if (reader.match('>')) {
            tokenType = EToken::BarGreater;
            tokenStr += '>';
          }
          break;
        }
        case '+': {
          tokenType = EToken::Plus;
          if (reader.match('+')) {
            tokenType = EToken::Plus2;
            tokenStr += '+';
          } else if (reader.match('=')) {
            tokenType = EToken::PlusEqual;
            tokenStr += '=';
          }
          break;
        }
        case '-': {
          tokenType = EToken::Minus;
          if (reader.match('-')) {
            tokenType = EToken::Minus2;
            tokenStr += '-';
          } else if (reader.match('=')) {
            tokenType = EToken::MinusEqual;
            tokenStr += '=';
          } else if (reader.match('>')) {
            tokenType = EToken::MinusGreater;
            tokenStr += '>';
          }
          break;
        }
        case '/': {
          tokenType = EToken::FwdSlash;
          if (reader.match('/')) {
            tokenType = EToken::Empty;
            reader.skipLine();
            m_currentColumn = 0;
            ++m_currentLine;
          } else if (reader.match('=')) {
            tokenType = EToken::FwdSlashEqual;
            tokenStr += '=';
          }
          break;
        }
        case '%': {
          tokenType = EToken::Percent;
          if (reader.match('=')) {
            tokenType = EToken::PercentEqual;
            tokenStr += '=';
          }
          break;
        }
        case '!': {
          tokenType = EToken::Exclaim;
          if (reader.match('!')) {
            tokenType = EToken::Exclaim2;
            tokenStr += '!';
          } else if (reader.match('=')) {
            tokenType = EToken::ExclaimEqual;
            tokenStr += '=';
          }
          break;
        }
        case '=': {
          tokenType = EToken::Equal;
          if (reader.match('=')) {
            tokenType = EToken::Equal2;
            tokenStr += '=';
          }
          break;
        }
        case '>': {
          tokenType = EToken::Greater;
          if (reader.match('>')) {
            tokenType = EToken::Greater2;
            tokenStr += '>';
            if (reader.match('=')) {
              tokenType = EToken::Greater2Equal;
              tokenStr += '=';
            }
          } else if (reader.match('=')) {
            tokenType = EToken::GreaterEqual;
            tokenStr += '=';
          }
          break;
        }
        case '<': {
          tokenType = EToken::Less;
          if (reader.match('>')) {
            tokenType = EToken::Less2;
            tokenStr += '>';
            if (reader.match('=')) {
              tokenType = EToken::Less2Equal;
              tokenStr += '=';
            }
          } else if (reader.match('=')) {
            tokenType = EToken::LessEqual;
            tokenStr += '=';
          } else if (reader.match('-')) {
            tokenType = EToken::LessMinus;
            tokenStr += '-';
          }
          break;
        }
        case '\'': {
          tokenType = EToken::SQuote;
          break;
        }
        case '\"': {
          tokenType = EToken::DQuote;
          break;
        }
        case ' ': {
          break;
        }
        // case '\r': {
        //   m_currentColumn = 0;
        //   break;
        // }
        case '\t': {
          // todo:
          tokenStr = "";
          m_currentColumn += 2;
          break;
        }
        case '\n': {
          tokenStr = "";
          m_currentColumn = 0;
          ++m_currentLine;
          break;
        }
        default: {
          if (Token::isDigit(c)) {
            tokenType = EToken::Number;
            tokenStr += scanNumber(reader);
          } else if (Token::isIdentifierSymbol(c)) {
            tokenStr += scanIdentifier(reader);
            tokenType = Token::identifierType(tokenStr);
          } else {
            failed = true;
          }
          break;
        }
      }

      if (tokenType != EToken::Empty) {
        tokens.emplace_back(tokenType, tokenStr, m_currentLine, m_currentColumn);
      }

      m_currentColumn += tokenStr.length();
    }

    return tokens;
  }

  auto Lexer::scanNumber(FileReader &reader) -> std::string {
    std::string str = scanInteger(reader);

    if (auto c = reader.advance(); c != '.' || !Token::isDigit(reader.peek())) {
      reader.revert();
      return str;
    }
    str += scanInteger(reader);

    return str;
  }

  auto Lexer::scanIdentifier(FileReader &reader) -> std::string {
    std::string str;

    for (auto c = reader.advance(); Token::isIdentifierSymbol(c);) {
      str += c;
      c = reader.advance();
    }
    reader.revert();

    return str;
  }

  auto Lexer::scanInteger(FileReader &reader) -> std::string {
    std::string str;

    for (auto c = reader.advance(); Token::isDigit(c);) {
      str += c;
      c = reader.advance();
    }
    reader.revert();

    return str;
  }
}
