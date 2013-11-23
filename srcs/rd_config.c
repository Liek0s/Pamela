/*
** config.c for PAMELA in /home/jack/workspace/crypto/pamela
** 
** Made by Jack
** Login   <jack@epitech.net>
** 
** Started on  Wed Nov 20 11:59:27 2013 Jack
** Last update Fri Nov 22 19:40:28 2013 Jack
*/

#include	"pamela.h"

static
xmlNodePtr	find_user(xmlNodePtr root, const char *username)
{
  int		ret;
  xmlNodePtr	node;
  xmlChar	*usrattr;

  for (node = root->xmlChildrenNode; node != NULL; node = node->next) {
    if (!xmlStrcmp(node->name, (const xmlChar *)"user")) {
      if ((usrattr = xmlGetProp(node, (const xmlChar *)"username")) != NULL) {
	ret = xmlStrcmp(usrattr, (const xmlChar *)username);
	xmlFree(usrattr);
	if (ret == 0)
	  return (node);
      }
    }
  }
  return (NULL);
}

static
xmlNodePtr	get_root(xmlDocPtr doc)
{
  xmlNodePtr	root;

  if ((root = xmlDocGetRootElement(doc)) == NULL)
    return (NULL);
  if (xmlStrcmp(root->name, (const xmlChar *)CONFIG_ROOT_NODE) != 0)
    return (NULL);
  return (root);
}

int		read_config(const char *docname, t_user **userinfo)
{
  xmlDocPtr	document;
  xmlNodePtr	root, usernode;

  if ((document = xmlParseFile(docname)) == NULL)
    return (pamela_log("Failed to parse configuration file '%s'.\n", docname));

  if ((root = get_root(document)) == NULL) {
    xmlFreeDoc(document);
    return (pamela_log("Failed to parse configuration file '%s'.\n", docname));
  }

  if ((usernode = find_user(root, (*userinfo)->username)) == NULL) {
    xmlFreeDoc(document);
    return (pamela_log("Failed to find user in configuration file.\n"));
  }

  if (read_user_config(document, usernode, userinfo) == ERROR) {
    xmlFreeDoc(document);
    return (pamela_log("An error occurred while parsing user '%s' configuration.\n", 
		       (*userinfo)->username));
  }
  xmlFreeDoc(document);
  return (SUCCESS);
}
