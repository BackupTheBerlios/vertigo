#include <glib/gslist.h>

#ifdef __cplusplus
extern "C" {
#endif

extern GSList *url_list;

void url_clear (void);
void url_save (const char *fname, const char *mode);
void url_autosave (void);
void url_check (char *buf);

#ifdef __cplusplus
}
#endif
