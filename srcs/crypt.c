/*
** crypt.c for PAMAELA in /home/jack/workspace/crypto/pamela
** 
** Made by Jack
** Login   <jack@epitech.net>
** 
** Started on  Thu Nov 21 11:26:36 2013 Jack
** Last update Sat Nov 23 01:29:49 2013 Jack
*/

#include		<libcryptsetup.h>

#include		"pamela.h"

static
int			generate_volume_name(const char *username, char **volume)
{
  size_t		size;
  char			*name;

  size = strlen("user-") + strlen(username) + strlen("-volume") + 1;
  if ((name = malloc((size + 1) * sizeof(char *))) == NULL)
    return (pamela_log("Failed to allocate memory : %s.\n", strerror(errno)));
  snprintf(name, size, "user-%s-volume", username);
  *volume = name;
  return (SUCCESS);
}

static
int			activate_luks_device(struct crypt_device *cryptdev, t_user *userinfo)
{  
  if (crypt_activate_by_keyfile(cryptdev, userinfo->volume,
  				CRYPT_ANY_SLOT, userinfo->keyfile, 0,
  				CRYPT_ACTIVATE_ALLOW_DISCARDS) < 0) {
    crypt_free(cryptdev);
    return (pamela_log("Failed to activate device %s.\n", crypt_get_device_name(cryptdev)));
  }
  crypt_free(cryptdev);
  return (SUCCESS);
}

int			unlock_luks_crypt(t_user *userinfo)
{
  struct crypt_device	*cryptdev;

  if (crypt_init(&cryptdev, userinfo->loopdevice) < 0)
    return (pamela_log("Failed to initialise device %s.\n", userinfo->loopdevice));
  if (crypt_load(cryptdev, CRYPT_LUKS1, NULL) < 0) {
    crypt_free(cryptdev);
    return (pamela_log("Failed to load device %s.\n", crypt_get_device_name(cryptdev)));
  }
  if (generate_volume_name(userinfo->username, &userinfo->volume)) {
    crypt_free(cryptdev);
    return (ERROR);
  }
  return (activate_luks_device(cryptdev, userinfo));
}

int			close_luks_device(t_user *userinfo)
{
  struct crypt_device	*cryptdev;

  if (crypt_init_by_name(&cryptdev, userinfo->volume) < 0)
    return (pamela_log("Failed to initialise volume %s.\n", userinfo->volume));
  if (crypt_deactivate(cryptdev, userinfo->volume) < 0) {
    crypt_free(cryptdev);
    return (pamela_log("Failed to deactivate volume %s.\n", userinfo->volume));
  }
  return (SUCCESS);
}
