/*
** pamela_mount.c for PAMELA in /home/jack/workspace/crypto/pamela
** 
** Made by Jack
** Login   <jack@epitech.net>
** 
** Started on  Fri Nov 15 10:24:03 2013 Jack
** Last update Sun Nov 24 19:48:04 2013 Jack
*/

#include		"pamela.h"

#define			PAM_SM_AUTH
#define			PAM_SM_SESSION

struct	globalConf	g_sessionInfo = {
  .silent = false,
  .configFile = NULL,
  .userSession = NULL,
};

extern
int		pam_sm_open_session(pam_handle_t *pamh, int flags, 
				    int argc, const char **argv)
{
  if ((is_root()) || (pamh == NULL))
    return (PAM_SESSION_ERR);
  if (init_session_info(pamh, flags, argc, argv))
    return (PAM_SESSION_ERR);
  if (strncmp(g_sessionInfo.userSession->username, "root", strlen("root")) != 0) {
    if (setup_loop_device(g_sessionInfo.userSession))
      return (cleanup(g_sessionInfo.userSession));
    if (unlock_luks_crypt(g_sessionInfo.userSession))
      return (cleanup(g_sessionInfo.userSession));
    if (mount_volume(g_sessionInfo.userSession))
      return (cleanup(g_sessionInfo.userSession));
  }
  return (PAM_SUCCESS);
}

extern
int		pam_sm_close_session(pam_handle_t *pamh, 
				     __attribute__((unused)) int flags, 
				     __attribute__((unused)) int argc, 
				     __attribute__((unused)) const char **argv)
{
  if ((is_root()) || (pamh == NULL))
    return (PAM_SESSION_ERR);
  if (g_sessionInfo.userSession != NULL) {
    unmount_volume(g_sessionInfo.userSession);
    close_luks_device(g_sessionInfo.userSession);
    detach_loop_device(g_sessionInfo.userSession->loopdevice);
  }
  return (PAM_SUCCESS);
}
