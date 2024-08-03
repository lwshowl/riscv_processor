/* We use the POSIX regex functions to process regular expressions.
 * Type 'man regex' for more information about POSIX regex functions.
 */
#include <regex.h>
#include <string>
#include <memory.h>
#include "include/utils.h"
#include "include/common.h"
#include "include/monitor.h"
#include "include/expr.h"
#include "include/utils.h"

enum
{
  TK_NOTYPE = 256,
  TK_DEREF,
  TK_MINUS,
  TK_PLUS,
  TK_MUL,
  TK_DIV,
  TK_EQ,
  TK_LPAREN,
  TK_RPAREN,
  TK_NUM,
  TK_STR,
  /* TODO: Add more token types */
};

static struct rule
{
  const char *regex;
  int token_type;
} rules[] = {

    /* TODO: Add more rules.
     * Pay attention to the precedence level of different rules.
     */

    {" +", TK_NOTYPE},             // spaces
    {"0[xX][0-9a-fA-F]+", TK_NUM}, // hex numbers
    {"[0-9]+", TK_NUM},            // numbers
    {"\\$[0-9a-zA-Z]+", TK_STR},   // string
    {"\\(", TK_LPAREN},            // left paren
    {"\\)", TK_RPAREN},            // right paren
    {"\\+", TK_PLUS},              // plus
    {"-", TK_MINUS},               // minus
    {"\\*", TK_MUL},               // multiply
    {"/", TK_DIV},                 // division
    {"==", TK_EQ},                 // equal
};

#define ARRLEN(arr) (int)(sizeof(arr) / sizeof(arr[0]))
#define NR_REGEX ARRLEN(rules)

static regex_t re[NR_REGEX] = {};

/* Rules are used for many times.
 * Therefore we compile them only once before any usage.
 */
void init_regex()
{
  int i;
  char error_msg[128];
  int ret;

  for (i = 0; i < NR_REGEX; i++)
  {
    ret = regcomp(&re[i], rules[i].regex, REG_EXTENDED);
    if (ret != 0)
    {
      regerror(ret, &re[i], error_msg, 128);
      printf("regex compilation failed: %s\n%s", error_msg, rules[i].regex);
    }
  }
}

typedef struct token
{
  int type;
  char str[32];
} Token;

static Token tokens[32] __attribute__((used)) = {};
static int nr_token __attribute__((used)) = 0;
static uint32_t token_num = 0;

static inline bool isOperator(int index)
{
  return (tokens[index].type == TK_MINUS ||
          tokens[index].type == TK_PLUS ||
          tokens[index].type == TK_MUL ||
          tokens[index].type == TK_DIV);
}

static bool make_token(char *e)
{
  int position = 0;
  int i;
  char str[32];
  regmatch_t pmatch;
  nr_token = 0;
  token_num = 0;

  while (e[position] != '\0')
  {
    /* Try all rules one by one. */
    for (i = 0; i < NR_REGEX; i++)
    {
      if (regexec(&re[i], e + position, 1, &pmatch, 0) == 0 && pmatch.rm_so == 0)
      {
        char *substr_start = e + position;
        int substr_len = pmatch.rm_eo;

        // printf("match rules[%d] = \"%s\" at position %d with len %d: %.*s\n",
        //     i, rules[i].regex, position, substr_len, substr_len, substr_start);

        position += substr_len;
        /* TODO: Now a new token is recognized with rules[i]. Add codes
         * to record the token in the array `tokens'. For certain types
         * of tokens, some extra actions should be performed.
         */
        switch (rules[i].token_type)
        {
        case TK_NOTYPE:
          break;
        case TK_NUM:
          memcpy(str, substr_start, substr_len);
          tokens[token_num].type = TK_NUM;
          str[substr_len] = 0;
          strcpy(tokens[token_num].str, str);
          token_num++;
          break;
        case TK_STR:
          memcpy(str, substr_start, substr_len);
          tokens[token_num].type = TK_STR;
          str[substr_len] = 0;
          strcpy(tokens[token_num].str, str);
          token_num++;
          break;
        case TK_LPAREN:
          tokens[token_num].type = TK_LPAREN;
          strcpy(tokens[token_num].str, "(");
          token_num++;
          break;
        case TK_RPAREN:
          tokens[token_num].type = TK_RPAREN;
          strcpy(tokens[token_num].str, ")");
          token_num++;
          break;
        case TK_MUL:
          if (token_num == 0 || isOperator(token_num - 1))
            tokens[token_num].type = TK_DEREF;
          else
            tokens[token_num].type = TK_MUL;
          strcpy(tokens[token_num].str, "*");
          token_num++;
          break;
        case TK_DIV:
          tokens[token_num].type = TK_DIV;
          strcpy(tokens[token_num].str, "/");
          token_num++;
          break;
        case TK_PLUS:
          tokens[token_num].type = TK_PLUS;
          strcpy(tokens[token_num].str, "+");
          token_num++;
          break;
        case TK_MINUS:
          tokens[token_num].type = TK_MINUS;
          strcpy(tokens[token_num].str, "-");
          token_num++;
          break;
        case TK_EQ:
          tokens[token_num].type = TK_EQ;
          strcpy(tokens[token_num].str, "==");
          token_num++;
          break;
        default:
          break;

          // TODO();
        }
        break;
      }
    }

    if (i == NR_REGEX)
    {
      printf("no match at position %d\n%s\n%*.s^\n", position, e, position, "");
      return false;
    }
  }

  return true;
}

int check_parentheses(int start, int end)
{
  int curr = start;
  int stack[100];
  int stack_top = 0;
  //记录是第几个括号？
  int parentheses_index = 0;

  //如果第一个字符不是左括号，或者最后一个字符不是右括号直接退出
  if ((tokens[start].type != TK_LPAREN) || (tokens[end].type != TK_RPAREN))
    return false;
  else
  {
    //将左括号编号入栈，准备匹配
    stack[stack_top] = parentheses_index;
    stack_top++;
    while (curr < end)
    {
      curr++;
      //检测到右括号，和可能的左括号匹配？？
      if (tokens[curr].type == TK_RPAREN)
      {
        if (stack[stack_top - 1] == TK_LPAREN)
        {
          stack_top--;
        }
        else
          return false;
      }
      //检测到左括号，将左括号编号入栈
      else if (tokens[curr].type == TK_LPAREN)
      {
        parentheses_index++;
        stack[stack_top++] = parentheses_index;
      }
    }
    //检查栈是否只剩下最后一个左括号？左括号的编号是否是第一个？？
    if (stack_top == 0 && stack[stack_top] == 0)
      return true;
    else
      return false;
  }
}

//表达式求值
word_t eval(int start, int end)
{
  int operator_main = 0;
  int operator_main_index = 0;
  int curr = start;
  int in_parentheses = 0;

  // printf("start,end: %d,%d\n", start, end);

  if (start > end)
    return -1;

  //求值
  else if (start == end)
  {
    if (tokens[start].type == TK_NUM)
    {
      if (tokens[start].str[0] == '0' && tokens[start].str[1] == 'x')
        return strtoul(tokens[start].str + 2, NULL, 16);
      else
        return strtoul(tokens[start].str, NULL, 10);
    }

    // registers
    if ((tokens[start].type == TK_STR) && (tokens[start].str[0] == '$'))
    {
      char *regname = tokens[start].str + 1;
      int success;
      word_t val;
      val = isa_reg_str2val(regname, &success);
      if (success)
        return val;
      else
      {
        printf("no such register %s\n", regname);
        return 0xdeadbeef;
      }
    }
  }

  // derefrencing
  else if ((end - start) == 1 && (tokens[start].type == TK_DEREF))
  {
    bool success;
    word_t val;
    std::string ori;
    if (tokens[end].type != TK_STR)
      return mem_read(atol(tokens[end].str), 1, ori);
    else
    {
      printf("dereferancing str: %s\n", tokens[end].str);
      int success;
      val = isa_reg_str2val(tokens[end].str + 1, &success);
      printf("reg val: %x\n", (uint32_t)val);
      if (success)
        return mem_read(val, 4, ori);
      else
      {
        printf("error parsing str %s", tokens[end].str);
        return 0xdeadbeef;
      }
    }
  }

  //检测括号？？
  else if (check_parentheses(start, end) == true)
  {
    return eval(start + 1, end - 1);
  }
  //找到主运算符
  else
  {
    while (curr <= end)
    {
      //检查运算符是否在括号里？？
      if (tokens[curr].type == TK_LPAREN)
        in_parentheses++;
      else if (tokens[curr].type == TK_RPAREN)
        in_parentheses--;

      //运算符不在括号里，按优先级分配主运算符
      if (tokens[curr].type == TK_MINUS ||
          tokens[curr].type == TK_PLUS ||
          tokens[curr].type == TK_MUL ||
          tokens[curr].type == TK_DIV ||
          tokens[curr].type == TK_EQ)
      {
        if ((tokens[curr].type >= operator_main) && (in_parentheses == 0))
        {
          operator_main = tokens[curr].type;
          operator_main_index = curr;
        }
      }
      curr++;
    }

    //递归下降分析
    // printf("primary operator %s\n", tokens[operator_main_index].str);
    word_t left_value = eval(start, operator_main_index - 1);
    word_t right_value = eval(operator_main_index + 1, end);

    switch (operator_main)
    {
    case TK_PLUS:
      return left_value + right_value;
    case TK_MINUS:
      return left_value - right_value;
    case TK_MUL:
      return left_value * right_value;
    case TK_DIV:
      if (right_value != 0)
        return left_value / right_value;
      else
        printf("division by zero\n");
      return 0xdeadbeef;
    case TK_EQ:
      return left_value == right_value;
    default:
      printf("error parsing primary expression\n");
      return 0xdeadbeef;
    }
    printf("primary expression successfully parsed\n");
    return 0xdeadbeef;
  }
  // NO REACH ???
  return 0;
}

word_t expr(char *e)
{
  word_t result;

  if (!make_token(e))
  {
    return 0;
  }

  int token_len = token_num;
  // printf("tokens %d\n", token_len);
  /* TODO: Insert codes to evaluate the expression. */
  // TODO();
  result = eval(0, token_len - 1);
  return result;
}