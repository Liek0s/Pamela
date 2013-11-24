/*
** cleanup.c for PAMELA in /home/jack/workspace/crypto/Pamela-git/srcs
** 
** Made by Jack
** Login   <jack@epitech.net>
** 
** Started on  Sun Nov 24 19:43:52 2013 Jack
** Last update Sun Nov 24 19:45:01 2013 Jack
*/

#include	"pamela.h"

int		cleanup(t_user *usersession)
{
  clean_user_session_data(usersession);
  unmount_volume(g_sessionInfo.userSession);
  close_luks_device(g_sessionInfo.userSession);
  detach_loop_device(g_sessionInfo.userSession->loopdevice);  
  return (PAM_SESSION_ERR);
}

void		clean_user_session_data(t_user *usersession)
{
  if (usersession != NULL) {
    free(usersession->mountinfo.mountpoint);
    free(usersession->mountinfo.fstype);
    free(usersession->container);
    free(usersession->volume);
    free(usersession->loopdevice);
    free(usersession->keyfile);
    free(usersession);
  }
}
