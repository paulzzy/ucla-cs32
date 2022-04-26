#include <cassert>
#include <iostream>
#include <stack>
#include <stdexcept>
#include <string>

const char LEFT_PAREN = '(';
const char RIGHT_PAREN = ')';
const char AND_CHAR = '&';
const char OR_CHAR = '|';
const char NOT_CHAR = '!';
const char TRUE_CHAR = 'T';
const char FALSE_CHAR = 'F';
const char SPACE_CHAR = ' ';
const char SENTINEL = '\0';

// Following convention, ! has higher precedence than &, which has higher
// precedence than |, and operators of equal precedence associate left to
// right (so the postfix form of T|F|T is TF|T|, not TFT||, which would be
// the postfix form of T|(F|T).
// Returns -1 if the argument is not an operator
int precedence(const char &operator_char) {
  switch (operator_char) {
  case OR_CHAR:
    return 0;
    break;
  case AND_CHAR:
    return 1;
    break;
  case NOT_CHAR:
    return 2;
    break;
  default:
    throw std::invalid_argument("Invalid operator");
    break;
  }
}

std::string remove_spaces(std::string infix) {
  for (size_t i = 0; i < infix.length(); i++) {
    if (infix.at(i) == SPACE_CHAR) {
      infix.erase(i, 1);
      i--; // Erasing a value moves a new value into the current index.
           // Neutralizing the `i++` allows the current index to be checked
           // again.
    }
  }

  return infix;
}

bool evaluate_postfix(const std::string &postfix) {
  std::stack<bool> operands;

  for (char current : postfix) {
    if (current == TRUE_CHAR || current == FALSE_CHAR) {
      operands.push(current == TRUE_CHAR);
    } else if (current == NOT_CHAR) { // `current` is a unary operator
      const bool operand = operands.top();
      operands.pop();
      operands.push(!operand);
    } else { // `current` is a binary operator
      const bool operand_two = operands.top();
      operands.pop();
      const bool operand_one = operands.top();
      operands.pop();

      switch (current) {
      case OR_CHAR:
        operands.push(operand_one || operand_two);
        break;

      case AND_CHAR:
        operands.push(operand_one && operand_two);
        break;

      default:
        throw std::invalid_argument("Invalid binary operator");
      }
    }
  }

  return operands.top();
}

// Returns true if the infix is valid, false otherwise.
// Directly modifies `infix`.
//
// Validation rules:
// - `infix` cannot be empty
// - Cannot have empty parentheses
// - Binary operators must have either parentheses or operands on both sides
// - Each left parenthesis must be paired with a right parenthesis, and vice
//   versa
// - Operands must be followed by a binary operator, a right parenthesis, or a
//   sentinel character (which indicates the string's start or end). They must
//   be preceded by a binary or unary operator, a left parenthesis, or a
//   sentinel.
// - Unary operators must be followed by another unary operator, an operand, or
//   a left parenthesis
bool infix_to_postfix(std::string infix, std::string &postfix) {
  std::stack<char> operators;

  for (size_t j = 0; j < infix.length(); j++) {
    const char current = infix.at(j);
    const char prev = (j > 0) ? infix.at(j - 1) : SENTINEL;
    const char next = (j < infix.length() - 1) ? infix.at(j + 1) : SENTINEL;

    switch (current) {
    // Operands
    case TRUE_CHAR:
    case FALSE_CHAR:
      // Operands must be followed by a binary operator, a right parenthesis, or
      // a sentinel character (which indicates the string's start or end). They
      // must be preceded by a binary or unary operator, a left parenthesis, or
      // a sentinel.
      if (!(next == AND_CHAR || next == OR_CHAR || next == RIGHT_PAREN ||
            next == SENTINEL) ||
          !(prev == AND_CHAR || prev == OR_CHAR || prev == NOT_CHAR ||
            prev == LEFT_PAREN || prev == SENTINEL)) {
        return false;
      }

      postfix += current;
      break;

    case LEFT_PAREN:
      // Cannot have empty parentheses
      if (next == RIGHT_PAREN) {
        return false;
      }

      operators.push(LEFT_PAREN);
      break;

    case RIGHT_PAREN:
      while (!operators.empty() && operators.top() != LEFT_PAREN) {
        postfix += operators.top();
        operators.pop();
      }

      // Each left parenthesis must be paired with a right parenthesis, and vice
      // versa
      if (operators.empty()) {
        return false;
      }

      operators.pop(); // Pops `LEFT_PAREN`
      break;

    // Unary operator
    case NOT_CHAR:
      if (!(next == NOT_CHAR || next == TRUE_CHAR || next == FALSE_CHAR ||
            next == LEFT_PAREN)) {
        return false;
      }

      operators.push(NOT_CHAR);
      break;

    // Binary operators
    case OR_CHAR:
    case AND_CHAR:
      // Binary operators must have either operands or parentheses on both
      // sides, or a unary operator on the right side with either an operand or
      // parenthesis on the left side
      if (!(next == TRUE_CHAR || next == FALSE_CHAR || next == LEFT_PAREN ||
            next == NOT_CHAR) ||
          !(prev == TRUE_CHAR || prev == FALSE_CHAR || prev == RIGHT_PAREN)) {
        return false;
      }

      while (!operators.empty() && operators.top() != LEFT_PAREN &&
             precedence(current) <= precedence(operators.top())) {
        postfix += operators.top();
        operators.pop();
      }
      operators.push(current);
      break;

    // Invalid characters in `infix`
    default:
      return false;
      break;
    }
  }

  while (!operators.empty()) {
    // Each left parenthesis must be paired with a right parenthesis, and vice
    // versa
    if (operators.top() == LEFT_PAREN) {
      return false;
    }

    postfix += operators.top();
    operators.pop();
  }

  return true;
}

// Evaluates a boolean expression
//   If infix is a syntactically valid infix boolean expression,
//   then set postfix to the postfix form of that expression, set
//   result to the value of the expression, and return zero.  If
//   infix is not a syntactically valid expression, return 1; in
//   that case, postfix may or may not be changed, but result must
//   be unchanged.
int evaluate(std::string infix, std::string &postfix, bool &result) {
  if (infix.empty()) {
    return 1;
  }

  postfix = "";
  result = false;

  infix = remove_spaces(infix);

  const bool valid = infix_to_postfix(infix, postfix);
  if (!valid) {
    return 1;
  }

  // Validate `infix` syntax and convert to postfix notation

  result = evaluate_postfix(postfix);
  return 0;
}

int main() {
  std::string postfix;
  bool result;

  assert(evaluate("T", postfix, result) == 0 && postfix == "T" && result);
  assert(evaluate("(F)", postfix, result) == 0 && postfix == "F" && !result);
  assert(evaluate("T&(F)", postfix, result) == 0 && postfix == "TF&" &&
         !result);
  assert(evaluate("T & !F", postfix, result) == 0 && postfix == "TF!&" &&
         result);
  assert(evaluate("!(F|T)", postfix, result) == 0 && postfix == "FT|!" &&
         !result);
  assert(evaluate("!F|T", postfix, result) == 0 && postfix == "F!T|" && result);
  assert(evaluate("T|F&F", postfix, result) == 0 && postfix == "TFF&|" &&
         result);
  assert(evaluate("T&!(F|T&T|F)|!!!(F&T&F)", postfix, result) == 0 &&
         postfix == "TFTT&|F|!&FT&F&!!!|" && result);
  assert(evaluate("T| F", postfix, result) == 0 && postfix == "TF|" && result);
  assert(evaluate("T|", postfix, result) == 1);
  assert(evaluate("F F", postfix, result) == 1);
  assert(evaluate("TF", postfix, result) == 1);
  assert(evaluate("()", postfix, result) == 1);
  assert(evaluate("()T", postfix, result) == 1);
  assert(evaluate("T(F|T)", postfix, result) == 1);
  assert(evaluate("T(&T)", postfix, result) == 1);
  assert(evaluate("(T&(F|F)", postfix, result) == 1);
  assert(evaluate("T+F", postfix, result) == 1);
  assert(evaluate("", postfix, result) == 1);
  assert(evaluate("F  |  !F & (T&F) ", postfix, result) == 0 &&
         postfix == "FF!TF&&|" && !result);
  assert(evaluate(" F  ", postfix, result) == 0 && postfix == "F" && !result);
  assert(evaluate("((T))", postfix, result) == 0 && postfix == "T" && result);

  // My test cases
  assert(evaluate("T)", postfix, result) == 1);
  assert(evaluate("!!!", postfix, result) == 1);
  assert(evaluate("T| F", postfix, result) == 0 && postfix == "TF|" &&
         result); // eval spaces
  assert(evaluate("T|", postfix, result) == 1);
  assert(evaluate("F F", postfix, result) == 1); // eval just two operands 1
  assert(evaluate("TF", postfix, result) == 1);  // eval just two operands 2
  assert(evaluate("()", postfix, result) == 1);  // eval empty paren
  assert(evaluate("()T", postfix, result) == 1);
  assert(evaluate("T(F|T)", postfix, result) ==
         1); // eval invalid operand @start
  assert(evaluate("(F|T)T", postfix, result) == 1); // eval invalid operand @end
  assert(evaluate("T(&T)", postfix, result) == 1);
  assert(evaluate("(T&(F|F)", postfix, result) == 1);
  assert(evaluate("T+F", postfix, result) == 1);
  assert(evaluate("", postfix, result) == 1);
  assert(evaluate("!!!!", postfix, result) == 1); // edge case

  assert(evaluate("F  |  !F & (T&F) ", postfix, result) == 0 &&
         postfix == "FF!TF&&|" && !result);
  assert(evaluate("!!F", postfix, result) == 0 && postfix == "F!!" &&
         !result); // should evaluate

  assert(evaluate(" F  ", postfix, result) == 0 && postfix == "F" && !result);
  assert(evaluate("((T))", postfix, result) == 0 && postfix == "T" && result);
  assert(evaluate("T!&F", postfix, result) == 1);
  assert(evaluate("T&!F", postfix, result) == 0 && postfix == "TF!&" && result);

  std::string cases[57] = {
      "",       "()",     "(())",   ")(",     "))((",   "(T",    "T)",
      "((T|F)", "(T|F))", ")T(",    ")F(",    "))T((",  "))F((", "TT",
      "TF",     "FT",     "FF",     "TTT",    "TTF",    "TFT",   "FTT",
      "TFF",    "FFT",    "FTF",    "FFF",    "T&&T",   "T&|F",  "T|&F",
      "T!&F",   "T!|F",   "T!!F",   "&",      "&T",     "&F",    "|",
      "|T",     "|F",     "T&",     "F&",     "T|",     "F|",    "(&T&T)",
      "(|T&T)", "(T&T!)", "(T&T&)", "(T&T|)", "(T&T)!", "(&T)",  "(&F)",
      "(|T)",   "(|F)",   "T!T",    "T!F",    "F!T",    "F!F",   "T!",
      "F!"};

  result = false;
  for (const std::string &c : cases) {
    std::cerr << "Case: " << c << std::endl;
    assert(evaluate(c, postfix, result) == 1);
    assert(result == false);
  }

  std::cerr << "tada passed :DD" << std::endl;
}
