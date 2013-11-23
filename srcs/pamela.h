/*
** pamela.h for PAMELA in /home/jack/workspace/crypto/pamela
** 
** Made by Jack
** Login   <jack@epitech.net>
** 
** Started on  Wed Nov 20 11:52:52 2013 Jack
** Last update Sat Nov 23 01:38:49 2013 Jack
*/

#ifndef		__PAMELA_H__
#define		__PAMELA_H__

#include	<stdio.h>
#include	<stdlib.h>
#include	<stdbool.h>
#include	<unistd.h>
#include	<string.h>
#include	<errno.h>
#include	<stdarg.h>

#include	<security/pam_modules.h>

#include	<libxml/tree.h>
#include	<libxml/parser.h>

#define		SUCCESS			0
#define		ERROR			-1

#define		CONFIG_ROOT_NODE	"pamela"

#define		LOOP_CONTROL		"/dev/loop-control"
#define		LOOP_DEV_PREFIX		"/dev/loop"

#define		DEV_MAP_DIR		"/dev/mapper/"

typedef struct	mountInfo {  
  char		*source;
  char		*mountpoint;
  char		*fstype;
}		t_mntinfo;

struct		userSettings {
  const char	*username;
  t_mntinfo	mountinfo;
  char		*container;
  char		*volume;
  char		*loopdevice;
  char		*keyfile;
};

typedef struct	userSettings	t_user;

struct		globalConf {
  bool		silent;
  const char	*configFile;
  t_user	*userSession;
};

extern struct	globalConf	g_sessionInfo;

int		read_config(const char *, t_user **);
int		read_user_config(xmlDocPtr, xmlNodePtr, t_user **);

int		setup_loop_device(t_user *);
int		detach_loop_device(const char *);

int		unlock_luks_crypt(t_user *);
int		close_luks_device(t_user *);

int		mount_volume(t_user *);
int		unmount_volume(t_user *);

void		clean_user_session_data(t_user *);

size_t		pamela_log(const char *, ...);
size_t		count_digits(int);

#endif		/* ! __PAMELA_H__ */



















