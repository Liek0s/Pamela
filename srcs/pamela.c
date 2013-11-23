/*
** pamela_mount.c for PAMELA in /home/jack/workspace/crypto/pamela
** 
** Made by Jack
** Login   <jack@epitech.net>
** 
** Started on  Fri Nov 15 10:24:03 2013 Jack
** Last update Sat Nov 23 01:54:13 2013 Jack
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

int		set_username(pam_handle_t *pamh, const char **username)
{
  return (pam_get_user(pamh, username, NULL));
}

int		init_user_data(pam_handle_t *pamh, t_user **userSession)
{
  t_user	*session;

  if ((session = malloc(sizeof(struct userSettings))) == NULL)
    return (pamela_log("Failed to allocate memory %s.\n", strerror(errno)));
  memset((char *)session, 0x00, sizeof(struct userSettings));  
  if (set_username(pamh, &session->username) != PAM_SUCCESS) {
    free(session);
    return (pamela_log("Failed to retrieve username.\n"));
  }
  if (read_config(g_sessionInfo.configFile, &session) != 0) {
    clean_user_session_data(session);
    return (ERROR);
  }
  *userSession = session;
  return (SUCCESS);
}

int		init_session_info(pam_handle_t *pamh, int flags, int argc, const char **argv)
{
  g_sessionInfo.silent = (flags == PAM_SILENT) ? true : false;
  if (argc == 1) {
    g_sessionInfo.configFile = argv[0];
    if (init_user_data(pamh, &g_sessionInfo.userSession) != 0)
      return (ERROR);
    return (SUCCESS);
  }
  return (pamela_log("Missing argument for \"pamela_mount\" module.\n"));
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
  (void) pamh;
  (void)argc; (void) argv; (void) flags;
  unmount_volume(g_sessionInfo.userSession);
  close_luks_device(g_sessionInfo.userSession);
  detach_loop_device(g_sessionInfo.userSession->loopdevice);
  return (PAM_SUCCESS);
}
