#ifndef _XCHAT_URL_H
#define _XCHAT_URL_H

#ifdef __cplusplus
extern "C" {
#endif


extern void *url_tree;

void url_clear (void);
void url_save (const char *fname, const char *mode);
void url_autosave (void);
void url_check (char *buf);


#ifdef __cplusplus
}
#endif

#endif
