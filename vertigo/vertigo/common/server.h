#ifndef _XCHAT__H
#define _XCHAT__H

#ifdef __cplusplus
extern "C" {
#endif



/* eventually need to keep the tcp_* functions isolated to server.c */
int tcp_send_len (server *serv, char *buf, int len);
int tcp_send (server *serv, char *buf);
void tcp_sendf (server *serv, char *fmt, ...);

void server_fill_her_up (server *serv);

#ifdef __cplusplus
}
#endif

#endif
