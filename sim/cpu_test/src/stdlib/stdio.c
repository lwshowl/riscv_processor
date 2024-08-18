#include "klib.h"
#include <klib-macros.h>
#include <stdarg.h>
#include <stdint.h>

#define STDOUT_MMIO ((uint64_t*)0x83000000)

void putch(char c) {
    *STDOUT_MMIO = c;
}

int printf(const char *fmt, ...)
{
  va_list args;
  va_start(args, fmt);
  char buffer[2048];
  vsnprintf(buffer, 2047, fmt, args);
  va_end(args);
  putstr(buffer);
  return 0;
}

int vsprintf(char *out, const char *fmt, va_list ap)
{
  int p = vsnprintf(out, -1, fmt, ap);
  return p;
}

int sprintf(char *out, const char *fmt, ...)
{
  va_list args;
  va_start(args, fmt);
  int p = vsnprintf(out, -1, fmt, args);
  va_end(args);
  return p;
}

int snprintf(char *out, size_t n, const char *fmt, ...)
{
  va_list args;
  va_start(args, fmt);
  int p = vsnprintf(out, n, fmt, args);
  va_end(args);
  return p;
}

int vsnprintf(char *out, size_t n, const char *fmt, va_list ap)
{
  size_t fmt_pos = 0;
  size_t out_pos = 0;
  size_t len = n;
  while (fmt[fmt_pos])
  {
    //把一段字符串 %s 之前的内容输出
    while (fmt[fmt_pos] != '%' && fmt[fmt_pos] && out_pos <= len)
    {
      out[out_pos] = fmt[fmt_pos];
      fmt_pos++;
      out_pos++;
    }

    //如果在%s 之前format 就结束了，或者长度超过了，直接返回
    if (!fmt[fmt_pos] || out_pos > len)
    {
      out[out_pos] = '\0';
      return out_pos;
    }

    // %s 内容
    else
    {
      //遇到string
      if (strncmp(fmt + fmt_pos, "%s", 2) == 0)
      {
        char *string = va_arg(ap, char *);
        size_t remain_len = len - out_pos;
        if (strlen(string) > remain_len)
        {
          strncpy(out + out_pos, string, remain_len);
          return out_pos;
        }
        else
        {
          strcpy(out + out_pos, string);
          out_pos += strlen(string);
          fmt_pos += 2;
        }
      }
      else if (strncmp(fmt + fmt_pos, "%d", 2) == 0 || strncmp(fmt + fmt_pos, "%3d", 3) == 0)
      {
        int num = va_arg(ap, int);
        size_t remain_length = len - out_pos;
        char num_string[100];
        size_t num_pos = 0;
        do
        {
          num_string[num_pos] = num % 10 + '0';
          num_pos++;
          num /= 10;
        } while (num);

        // 反转num_string
        size_t left = 0, right = num_pos - 1;
        while (left < right)
        {
          char temp = num_string[left];
          num_string[left] = num_string[right];
          num_string[right] = temp;
          left++;
          right--;
        }

        if (num_pos > remain_length)
        {
          strncpy(out + out_pos, num_string, num_pos);
          return out_pos;
        }
        else
        {
          strcpy(out + out_pos, num_string);
          out_pos += num_pos;
          if (strncmp(fmt + fmt_pos, "%d", 2) == 0)
            fmt_pos += 2;
          else if (strncmp(fmt + fmt_pos, "%3d", 3) == 0)
            fmt_pos += 3;
        }
      }
      else if ((strncmp(fmt + fmt_pos, "%x", 2) == 0) || (strncmp(fmt + fmt_pos, "%02x", 4) == 0))
      {
        uint64_t num = va_arg(ap, int);
        uint32_t remain_length = len - out_pos;
        char num_string[100];
        size_t num_pos = 0;
        do
        {
          uint32_t remainder = num % 16;
          if (remainder < 10)
            num_string[num_pos] = '0' + remainder;
          else
            num_string[num_pos] = 'A' + remainder - 10;
          num_pos++;
          num /= 16;
        } while (num);

        // 反转num_string
        size_t left = 0, right = num_pos - 1;
        while (left < right)
        {
          char temp = num_string[left];
          num_string[left] = num_string[right];
          num_string[right] = temp;
          left++;
          right--;
        }

        if (num_pos > remain_length)
        {
          strncpy(out + out_pos, num_string, num_pos);
          return out_pos;
        }
        else
        {
          strcpy(out + out_pos, num_string);
          out_pos += num_pos;
          if (strncmp(fmt + fmt_pos, "%x", 2) == 0)
            fmt_pos += 2;
          else if (strncmp(fmt + fmt_pos, "%02x", 4) == 0)
            fmt_pos += 4;
        }
      }
      else
      {
        putstr(fmt + fmt_pos);
        panic("format not implemented\n");
      }
    }
  }
  return out_pos;
}