/*
** pamela_mount.c for PAMELA in /home/jack/workspace/crypto/pamela
** 
** Made by Jack
** Login   <jack@epitech.net>
** 
** Started on  Fri Nov 15 10:24:03 2013 Jack
** Last update Sun Nov 24 13:32:40 2013 Jack
*/

#define			PAM_SM_SESSION

#include		"pamela.h"

struct	globalConf	g_sessionInfo = {
  .silent = false,
  .configFile = NULL,
  .userSession = NULL,
};

static
int		is_root(void)
{
  if (getuid())
    return (pamela_log("This module must be run as root.\n"));
  return (SUCCESS);
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

extern
int		pam_sm_open_session(pam_handle_t *pamh, int flags, int argc, const char **argv)
{
  if ((is_root()) || (pamh == NULL))
    return (PAM_SESSION_ERR);
  if (init_session_info(pamh, flags, argc, argv))
    return (PAM_SESSION_ERR);
  if (setup_loop_device(g_sessionInfo.userSession))
    return (PAM_SESSION_ERR);
  if (unlock_luks_crypt(g_sessionInfo.userSession))
    return (PAM_SESSION_ERR);
  if (mount_volume(g_sessionInfo.userSession))
    return (PAM_SESSION_ERR);
  return (PAM_SUCCESS);
}

extern
int		pam_sm_close_session(pam_handle_t *pamh, int flags, int argc, const char **argv)
{
  if ((is_root()) || (pamh == NULL) || (g_sessionInfo.userSession == NULL))
    return (PAM_SESSION_ERR);
  (void)argc; (void) argv; (void) flags;
  unmount_volume(g_sessionInfo.userSession);
  close_luks_device(g_sessionInfo.userSession);
  detach_loop_device(g_sessionInfo.userSession->loopdevice);
  return (PAM_SUCCESS);
}
