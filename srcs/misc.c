/*
** misc.c for PAMELA in /home/jack/workspace/crypto/Pamela-git/srcs
** 
** Made by Jack
** Login   <jack@epitech.net>
** 
** Started on  Sun Nov 24 19:45:42 2013 Jack
** Last update Sun Nov 24 19:46:25 2013 Jack
*/

#include	"pamela.h"

int		is_root(void)
{
  if (getuid())
    return (pamela_log("This module must be run as root.\n"));
  return (SUCCESS);
}

size_t		count_digits(int n)
{
  size_t	count;

  if (n == 0)
    return (1);
  else {
    count = 0;
    while (n != 0) {
      n /= 10;
      ++count;
    }
    return (count);
  }
}
