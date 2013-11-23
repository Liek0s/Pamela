/*
** log.c for PAMELA in /home/jack/workspace/crypto/pamela
** 
** Made by Jack
** Login   <jack@epitech.net>
** 
** Started on  Fri Nov 22 17:58:30 2013 Jack
** Last update Fri Nov 22 18:02:31 2013 Jack
*/

#include	"pamela.h"

size_t		pamela_log(const char *format, ...)
{
  va_list	args;
  size_t	res;

  if (g_sessionInfo.silent == false) {
    va_start(args, format);
    res = vfprintf(stderr, format, args);
    va_end(args);
  }
  return (res);
}
