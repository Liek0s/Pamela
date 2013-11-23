/*
** rd_user_config.c for PAMELA in /home/jack/workspace/crypto/pamela
** 
** Made by Jack
** Login   <jack@epitech.net>
** 
** Started on  Fri Nov 22 17:42:43 2013 Jack
** Last update Fri Nov 22 19:41:18 2013 Jack
*/

#include	"pamela.h"

static int	container_handler(xmlDocPtr doc, xmlNodePtr curr, t_user **usettings);
static int	mountpoint_handler(xmlDocPtr doc, xmlNodePtr curr, t_user **usettings);
static int	keyfile_handler(xmlDocPtr doc, xmlNodePtr curr, t_user **usettings);
static int	fstype_handler(xmlDocPtr doc, xmlNodePtr curr, t_user **usettings);

static
struct		funcTable {
  const char	*tagname;
  int		(*funcptr)(xmlDocPtr doc, xmlNodePtr curr, t_user **usettings);
} func_ptr_table[5] = {
  { "container", &container_handler },
  { "mountpoint", &mountpoint_handler },
  { "keyfile", &keyfile_handler },
  { "fstype", &fstype_handler },
  {NULL, NULL}
};

int		container_handler(xmlDocPtr doc, xmlNodePtr curr, t_user **usettings)
{
  xmlChar	*value;

  value = xmlNodeListGetString(doc, curr->xmlChildrenNode, 1);
  if (value) {
    (*usettings)->container = (char *)xmlStrndup(value, xmlStrlen(value));  
    xmlFree(value);
    return (SUCCESS);
  }
  xmlFree(value);
  return (ERROR);
}

int		mountpoint_handler(xmlDocPtr doc, xmlNodePtr curr, t_user **usettings)
{
  xmlChar	*value;

  value = xmlNodeListGetString(doc, curr->xmlChildrenNode, 1);
  if (value) {
    (*usettings)->mountinfo.mountpoint = (char *)xmlStrndup(value, xmlStrlen(value));  
    xmlFree(value);
    return (SUCCESS);
  }
  xmlFree(value);
  return (ERROR);
}

int		keyfile_handler(xmlDocPtr doc, xmlNodePtr curr, t_user **usettings)
{
  xmlChar	*value;

  value = xmlNodeListGetString(doc, curr->xmlChildrenNode, 1);
  if (value) {
    (*usettings)->keyfile = (char *)xmlStrndup(value, xmlStrlen(value));  
    xmlFree(value);
    return (SUCCESS);
  }
  xmlFree(value);
  return (ERROR);
}

int		fstype_handler(xmlDocPtr doc, xmlNodePtr curr, t_user **usettings)
{
  xmlChar	*value;

  value = xmlNodeListGetString(doc, curr->xmlChildrenNode, 1);
  if (value) {
    (*usettings)->mountinfo.fstype = (char *)xmlStrndup(value, xmlStrlen(value));  
    xmlFree(value);
    return (SUCCESS);
  }
  xmlFree(value);
  return (ERROR);
}

int		read_user_config(xmlDocPtr doc, xmlNodePtr usernode, t_user **userinfo)
{
  size_t	i;
  xmlNodePtr	node;

  for (node = usernode->xmlChildrenNode; node != NULL; node = node->next) {
    i = 0;
    while (func_ptr_table[i].tagname != NULL) {
      if (xmlStrcmp(node->name, (const xmlChar *)func_ptr_table[i].tagname) == 0) {
	if (func_ptr_table[i].funcptr(doc, node, userinfo) != SUCCESS)
	  return (ERROR);
      }
      ++i;
    }
  }
  return (SUCCESS);
}
