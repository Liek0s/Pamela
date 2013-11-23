/*
** mount.c for PAMELA in /home/jack/workspace/crypto/pamela
** 
** Made by Jack
** Login   <jack@epitech.net>
** 
** Started on  Thu Nov 21 23:54:03 2013 Jack
** Last update Sat Nov 23 01:38:57 2013 Jack
*/

#include	<sys/stat.h>
#include	<sys/mount.h>
#include	<pwd.h>

#include	"pamela.h"

static
int		make_pathname(char **targetdir, const char *username)
{
  size_t	size;
  char		*path;

  size = strlen(*targetdir) + strlen(username) + 1;
  if ((path = malloc(size * sizeof(char *))) == NULL)
    return (pamela_log("Failed to allocate memory %s.\n", strerror(errno)));
  snprintf(path, size, "%s%s", *targetdir, username);
  free(*targetdir);
  *targetdir = path;
  return (SUCCESS);
}

static
int		create_userdir(t_user *userinfo)
{  
  if (make_pathname(&userinfo->mountinfo.mountpoint, userinfo->username))
    return (ERROR);
  if (mkdir(userinfo->mountinfo.mountpoint, (S_IRWXU | S_IXGRP | S_IXOTH)) == -1)
    return (pamela_log("Failed to create user directory %s.\n", strerror(errno)));
  return (SUCCESS);
}

static
int		make_source_path(char **source, const char *volume)
{
  size_t	size;
  char		*volume_path;

  size = strlen(DEV_MAP_DIR) + strlen(volume) + 1;
  if ((volume_path = malloc((size + 1) * sizeof(char *))) == NULL)
    return (pamela_log("Failed to allocate memory : %s.\n", strerror(errno)));
  snprintf(volume_path, size, "%s%s", DEV_MAP_DIR, volume);
  *source = volume_path;
  return (SUCCESS);
}

static
int		set_user_perm(t_user *userinfo)
{
  struct passwd	*pwd;

  if ((pwd = getpwnam(userinfo->username)) == NULL)
    return (pamela_log("Failed to retrieve information for %s.\n", userinfo->username));
  if (chown(userinfo->mountinfo.mountpoint, pwd->pw_uid, pwd->pw_gid) == -1)
    return (pamela_log("Failed to set user priviledges for %s.\n", userinfo->username));
  return (SUCCESS);
}

int		mount_volume(t_user *userinfo)
{
  int		ret;

  if (create_userdir(userinfo))
    return (ERROR);
  if (make_source_path(&userinfo->mountinfo.source, userinfo->volume))
    return (ERROR);
  ret = mount(userinfo->mountinfo.source, userinfo->mountinfo.mountpoint,
	      userinfo->mountinfo.fstype, 0, NULL);
  if (ret != 0)
    return (pamela_log("Failed to mount %s.\n", userinfo->mountinfo.source));
  if (set_user_perm(userinfo))
    return (ERROR);
  return (SUCCESS);
}

int		unmount_volume(t_user *userinfo)
{
  if (umount(userinfo->mountinfo.mountpoint) == -1)
    return (pamela_log("Failed to unmount %s.\n", userinfo->mountinfo.mountpoint));
  if (rmdir(userinfo->mountinfo.mountpoint) == -1)
    return (ERROR);
  return (SUCCESS);
}
