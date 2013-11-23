/*
** loop.c for PAMELA in /home/jack/workspace/crypto/pamela
** 
** Made by Jack
** Login   <jack@epitech.net>
** 
** Started on  Tue Nov 19 13:46:08 2013 Jack
** Last update Fri Nov 22 23:52:36 2013 Jack
*/

#include	<sys/stat.h>
#include	<fcntl.h>
#include	<linux/loop.h>
#include	<stropts.h>

#include	"pamela.h"

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

static
char		*get_loop_device_str(const int loopdev_nb)
{
  size_t	size;
  char		*devstr;

  size = strlen(LOOP_DEV_PREFIX) + count_digits(loopdev_nb) + 1;
  if ((devstr = malloc((size + 1) * sizeof(char *))) == NULL)
    return (NULL);
  snprintf(devstr, size, "%s%d", LOOP_DEV_PREFIX, loopdev_nb);
  return (devstr);
}

static
int		get_free_loop_device(char **loopdevice)
{
  int		loopdev_nb;
  int		controlfd;

  if ((controlfd = open(LOOP_CONTROL, O_RDONLY)) < 0)
    return (pamela_log("Failed to open loop control device : %s.\n", strerror(errno)));
  if ((loopdev_nb = ioctl(controlfd, LOOP_CTL_GET_FREE)) < 0) {
    close(controlfd);
    return (pamela_log("Failed to get free loop device : %s.\n", strerror(errno)));
  }
  if ((*loopdevice = get_loop_device_str(loopdev_nb)) == NULL) {
    close(controlfd);
    return (pamela_log("Failed to create loop device string : %s.\n", strerror(errno)));
  }
  return (SUCCESS);
}

static
int		set_loop_device_info(int loopdevfd, const char *cntr)
{
  struct loop_info64	info;

  memset(&info, 0x00, sizeof(struct loop_info64));
  strncpy((char *)info.lo_file_name, cntr, LO_NAME_SIZE);
  if (ioctl(loopdevfd, LOOP_SET_STATUS64, &info) < 0)
    return (pamela_log("Warning : Failed to set loop device information : %s.\n", strerror(errno)));
  return (SUCCESS);
}

static
int		attach_loop_device(const char *cntr, const char *loopdev)
{
  int			cntrfd;
  int			loopdevfd;

  if ((cntrfd = open(cntr, O_RDWR)) < 0)
    return (pamela_log("Failed to open user container : %s.\n", strerror(errno)));
  if ((loopdevfd = open(loopdev, O_RDWR)) < 0) {
    close(cntrfd);
    return (pamela_log("Failed to open loop device : %s.\n", strerror(errno)));
  }
  if (ioctl(loopdevfd, LOOP_SET_FD, cntrfd) < 0) {
    close(cntrfd);
    close(loopdevfd);
    return (pamela_log("Failed to attach container to loop device : %s.\n", strerror(errno)));
  }
  set_loop_device_info(loopdevfd, cntr);
  close(cntrfd);
  close(loopdevfd);
  return (SUCCESS);
}

int		setup_loop_device(t_user *userinfo)
{
  if (get_free_loop_device(&userinfo->loopdevice) != SUCCESS)
    return (ERROR);
  if (attach_loop_device(userinfo->container, userinfo->loopdevice))
    return (ERROR);
  return (SUCCESS);
}

int		detach_loop_device(const char *device)
{
  int		loopdevfd;

  if ((loopdevfd = open(device, O_RDWR)) < 0)
    return (pamela_log("Failed to open loop device : %s.\n", strerror(errno)));
  if (ioctl(loopdevfd, LOOP_CLR_FD) < 0) {
    close(loopdevfd);
    return (pamela_log("Failed to detach loop device : %s.\n", strerror(errno)));
  }
  return (SUCCESS);
}

