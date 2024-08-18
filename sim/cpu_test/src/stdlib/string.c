#include <klib.h>
#include <klib-macros.h>
#include <stdint.h>

#if !defined(__ISA_NATIVE__) || defined(__NATIVE_USE_KLIB__)

size_t strlen(const char *s)
{
  char *buf = (char *)s;
  size_t len = 0;
  while (*buf++)
    len++;
  return len;
}

char *strcpy(char *dst, const char *src)
{
  char *dst_buf = dst;
  char *src_buf = (char *)src;
  size_t pos = 0;
  while (src_buf[pos])
  {
    dst_buf[pos] = src_buf[pos];
    pos++;
  }
  dst_buf[pos] = 0;
  return dst;
}

char *strncpy(char *dst, const char *src, size_t n)
{
  int pos = 0;
  char *dst_buffer = dst;
  char *src_buffer = (char *)src;
  while (*(src + pos) && n > 0)
  {
    dst_buffer[pos] = src_buffer[pos];
    pos++;
    n--;
  }
  dst_buffer[pos] = 0;
  return dst;
}

char *strcat(char *dst, const char *src)
{
  char *dest_buf = dst;
  char *src_buf = (char *)src;
  char *dest_rear;

  // find the terminator position of dst
  while (*dest_buf)
    dest_buf++;

  dest_rear = dest_buf;

  // append src to dest while src is not terminated
  while (*src_buf)
  {
    *dest_rear = *src_buf;
    dest_rear++;
    src_buf++;
  }

  // append terminator to the end
  *dest_rear = 0;
  return dst;
}

int strcmp(const char *s1, const char *s2)
{
  // buffer pointer of each string
  char *str1 = (char *)s1;
  char *str2 = (char *)s2;

  // compare them until an unmatch
  // or either of them is not terminated
  while (*str1 || *str2)
  {
    if ((*str1 - *str2) == 0)
    {
      str1++;
      str2++;
      continue;
    }
    else
      return *str1 - *str2;
  }

  // both str1 and str2 has terminated
  // and previous characters are all matched
  // return succeess
  return 0;
}

int strncmp(const char *s1, const char *s2, size_t n)
{
  // buffer pointer of each string
  char *str1 = (char *)s1;
  char *str2 = (char *)s2;

  // compare them until an unmatch
  // or either of them is not terminated
  while ((*str1 || *str2) && n > 0)
  {
    if ((*str1 - *str2) == 0)
    {
      str1++;
      str2++;
      n--;
      continue;
    }
    else
      return *str1 - *str2;
  }

  // both str1 and str2 has terminated
  // and previous characters are all matched
  // return succeess
  return 0;
}

void *memset(void *s, int c, size_t n)
{
  // treat memory as byte array
  // set each byte to value {c}

  uint8_t *buf = (uint8_t*)s;
  for (int i = 0; i < n; i++)
    *(buf + i) = c;

  return s;
}

void *memmove(void *dst, const void *src, size_t n)
{
  panic("Not implemented");
}

void *memcpy(void *out, const void *in, size_t n)
{
  uint8_t *out_buf = (uint8_t *)out;
  uint8_t *in_buf = (uint8_t *)in;
  for (size_t pos = 0; pos < n; pos++)
    out_buf[pos] = in_buf[pos];
  return 0;
}

int memcmp(const void *s1, const void *s2, size_t n)
{
  uint8_t *s1_buf = (uint8_t *)s1;
  uint8_t *s2_buf = (uint8_t *)s2;

  // compare two chunks of memory as byte oriented
  // for n times
  int i = 0;
  while (i < n)
  {
    if (*s1_buf == *s2_buf)
    {
      i++;
      continue;
    }
    else
      return *s1_buf - *s2_buf;
  }

  // n comparasion is performed
  // return success
  return 0;
}

#endif
