#ifndef _XCHAT_HIST_H
#define _XCHAT_HIST_H

#ifdef __cplusplus
extern "C" {
#endif


#define HISTORY_SIZE 100

struct history
{
	char *lines[HISTORY_SIZE];
	int pos;
	int realpos;
};

void history_add (struct history *his, char *text);
void history_free (struct history *his);
char *history_up (struct history *his);
char *history_down (struct history *his);

#ifdef __cplusplus
}
#endif

#endif
