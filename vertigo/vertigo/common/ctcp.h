#ifndef _XCHAT_CTCP_H
#define _XCHAT_CTCP_H

#ifdef __cplusplus
extern "C" {
#endif

void ctcp_handle (session *sess, char *to, char *nick, char *msg, char *word[], char *word_eol[]);

#ifdef __cplusplus
}
#endif

#endif
