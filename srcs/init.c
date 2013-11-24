/*
** init.c for PAMELA in /home/jack/workspace/crypto/Pamela-git/srcs
** 
** Made by Jack
** Login   <jack@epitech.net>
** 
** Started on  Sun Nov 24 13:32:27 2013 Jack
** Last update Sun Nov 24 19:35:17 2013 Jack
*/

#include	"pamela.h"

static
int		set_username(pam_handle_t *pamh, const char **username)
{
  return (pam_get_user(pamh, username, NULL));
}

static
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
  if (strncmp(session->username, "root", strlen("root"))!= 0) {
    if (read_config(g_sessionInfo.configFile, &session) != 0) {
      clean_user_session_data(session);
      return (ERROR);
    }
  }
  *userSession = session;
  return (SUCCESS);
}

int		init_session_info(pam_handle_t *pamh, int flags, 
				  int argc, const char **argv)
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
