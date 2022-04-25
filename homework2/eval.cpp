#include <cassert>
#include <iostream>
#include <stack>
#include <stdexcept>

const char LEFT_PAREN = '(';
const char RIGHT_PAREN = ')';
const char AND_CHAR = '&';
const char OR_CHAR = '|';
const char NOT_CHAR = '!';
const char TRUE_CHAR = 'T';
const char FALSE_CHAR = 'F';
const char SPACE_CHAR = ' ';

// Following convention, ! has higher precedence than &, which has higher
// precedence than |, and operators of equal precedence associate left to
// right (so the postfix form of T|F|T is TF|T|, not TFT||, which would be
// the postfix form of T|(F|T).
// Returns -1 if the argument is not an operator
int precedence(char &operator_char) {
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

  std::stack<char> operators;

  // Convert infix to postfix
  // Does not validate yet
  for (char &current : infix) {
    switch (current) {
    // Operands
    case TRUE_CHAR:
    case FALSE_CHAR:
      postfix += current;
      break;

    case LEFT_PAREN:
      operators.push(LEFT_PAREN);
      break;

    case RIGHT_PAREN:
      while (operators.top() != LEFT_PAREN) {
        postfix += operators.top();
        operators.pop();
      }
      operators.pop(); // Pops `LEFT_PAREN`
      break;

    // Unary operator
    case NOT_CHAR:
      operators.push(NOT_CHAR);
      break;

    // Binary operators
    case OR_CHAR:
    case AND_CHAR:
      while (!operators.empty() && operators.top() != LEFT_PAREN &&
             precedence(current) <= precedence(operators.top())) {
        postfix += operators.top();
        operators.pop();
      }
      operators.push(current);
      break;

    // Spaces are noop
    case SPACE_CHAR:
      break;

    // Invalid characters in `infix`
    default:
      return 1;
      break;
    }
  }

  while (!operators.empty()) {
    postfix += operators.top();
    operators.pop();
  }

  std::stack<bool> operands;

  // Evaluate postfix
  for (char &current : postfix) {
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

  result = operands.top();

  return 0;
}

int main() {
  std::string postfix;
  bool answer;

  assert(evaluate("T", postfix, answer) == 0 && postfix == "T" && answer);
  assert(evaluate("(F)", postfix, answer) == 0 && postfix == "F" && !answer);
  assert(evaluate("T&(F)", postfix, answer) == 0 && postfix == "TF&" &&
         !answer);
  assert(evaluate("T & !F", postfix, answer) == 0 && postfix == "TF!&" &&
         answer);
  assert(evaluate("!(F|T)", postfix, answer) == 0 && postfix == "FT|!" &&
         !answer);
  assert(evaluate("!F|T", postfix, answer) == 0 && postfix == "F!T|" && answer);
  assert(evaluate("T|F&F", postfix, answer) == 0 && postfix == "TFF&|" &&
         answer);
  assert(evaluate("T&!(F|T&T|F)|!!!(F&T&F)", postfix, answer) == 0 &&
         postfix == "TFTT&|F|!&FT&F&!!!|" && answer);
}
