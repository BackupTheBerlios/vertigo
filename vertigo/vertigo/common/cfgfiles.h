/* cfgfiles.h */

#ifndef _XCHAT_CFGFILES_H
#define _XCHAT_CFGFILES_H

#ifdef __cplusplus
extern "C" {
#endif


extern char *xdir;

char *cfg_get_str (char *cfg, char *var, char *dest);
int cfg_get_bool (char *var);
int cfg_get_int_with_result (char *cfg, char *var, int *result);
int cfg_get_int (char *cfg, char *var);
int cfg_put_int (int fh, int value, char *var);
char *get_xdir (void);
void load_config (void);
int save_config (void);
void list_free (GSList ** list);
void list_loadconf (char *file, GSList ** list, char *defaultconf);
int list_delentry (GSList ** list, char *name);
void list_addentry (GSList ** list, char *cmd, char *name);
int cmd_set (session *sess, char *tbuf, char *word[], char *word_eol[]);

#define STRUCT_OFFSET_STR(type,field) \
( (unsigned int) (((char *) (&(((type *) NULL)->field)))- ((char *) NULL)) )

#define STRUCT_OFFSET_INT(type,field) \
( (unsigned int) (((int *) (&(((type *) NULL)->field)))- ((int *) NULL)) )

#define P_OFFSET(field) STRUCT_OFFSET_STR(struct xchatprefs, field)
#define P_OFFINT(field) STRUCT_OFFSET_INT(struct xchatprefs, field)

struct prefs
{
	char *name;
	unsigned short offset;
	unsigned short type;
};

#define TYPE_STR 0
#define TYPE_INT 1
#define TYPE_BOOL 2

#ifdef __cplusplus
}
#endif

#endif
