#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#ifdef HAVE_STRINGS_H
#include <strings.h>
#endif
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>
#include <ctype.h>

#include <kapplication.h>
#include <kaboutdata.h>
#include <kmessagebox.h>
#include <kcmdlineargs.h>
#include <kdebug.h>
#include <kglobalsettings.h>
#include <ksystemtray.h>
#include <kiconloader.h>
#include <knotifyclient.h>


#include "../common/xchat.h"
#include "../common/xchatc.h"
#include "../common/cfgfiles.h"
#include "../common/outbound.h"
#include "../common/util.h"
#include "../common/fe.h"

#include "fe-kde.h"
#include "mainwindow.h"
#include "app.h"
#include "mainview.h"
#include "serverlist.h"
#include "rawlog.h"
#include "chanlist.h"
#include "extraview.h"
#include "textwidget.h"

extern "C"
{

    static bool g_inited = false;

    void fill_cmdcomp(KCompletion * c)
    {
        int i = 0;
        QString a;

        while   (xc_cmds[i].name != NULL)
        {
            //      kdDebug() << "addingItem "<<xc_cmds[i].name<<endl;
            a = xc_cmds[i].name;
            c->addItem(a.lower());
            i++;
        }
    }
    void fe_new_window(struct session *sess)
    {
        kdDebug() << "fe:newwindow   type=" << sess->type << endl;
        bool forceTab = ((sess->type == SESS_DIALOG) && prefs.privmsgtab)
            || prefs.tabchannels;
        session_gui *gui;

        gui = new session_gui;
        gui->win = xchatapp->getMainWindow(!forceTab);
        gui->view = gui->win->getMainView(sess);
        gui->comp = new KCompletion();
        gui->comp->setOrder(KCompletion::Insertion);
        gui->comp->setCompletionMode(KGlobalSettings::CompletionShell);

        gui->cmdcomp = new KCompletion();
        gui->cmdcomp->setOrder(KCompletion::Insertion);
        gui->cmdcomp->setCompletionMode(KGlobalSettings::CompletionShell);

        fill_cmdcomp(gui->cmdcomp);

        QObject::connect(gui->cmdcomp,
                         SIGNAL(matches(const QStringList &)), gui->view,
                         SLOT(completionMatches(const QStringList &)));



        QObject::connect(gui->comp, SIGNAL(matches(const QStringList &)),
                         gui->view,
                         SLOT(completionMatches(const QStringList &)));

        gui->showbar = false;
        gui->lag = 0;

        sess->gui = gui;
        current_sess = sess;
        //gui->view->show();
        kdDebug() << "fe:newwindow :::: end" << endl;
    }

    void fe_print_text(struct session *sess, char *text)
    {
        kdDebug() << "fe:fe_print_text :" << text << endl;
        //if (!sess->gui)
        //    fe_new_window(sess);

        sess->gui->view->getTextView()->appendLines(text, true);
        if (!sess->new_data && sess != current_tab && !sess->nick_said) {
            sess->new_data = 1;
            if (sess->msg_said)
                sess->gui->win->setViewColor(sess->gui->view,
                                                         Qt::darkRed);
            else
                sess->gui->win->setViewColor(sess->gui->view,
                                                         Qt::red);
        }
    }

    void fe_timeout_remove(int tag)
    {
        kdDebug() << "fe:fe_timeout_remove" << endl;
        xchatapp->removeTimeoutFunction(tag);
    }

    int fe_timeout_add(int interval, void *func, void *data)
    {
        int id = xchatapp->addTimeoutFunction(interval, func, data);

        //kdDebug() << "fe:fe_timeout_add tag=" << id <<endl;
        return (id);
    }

    void fe_input_remove(int tag)
    {
        kdDebug() << "fe:fe_input_remove tag=" << tag << endl;
        xchatapp->removeInputFunction(tag);

    }

    int fe_input_add(int sok, int flags, void *func, void *data)
    {
        int tag = xchatapp->addInputFunction(sok, flags, func, data);

        kdDebug() << "fe:fe_input_add tag=" << tag << endl;

        return (tag);
    }
    static KCmdLineOptions options[] = {
        {"d", 0, 0},
        {"cfgdir <directory>",
         I18N_NOOP("Use directory as the configuration directory."), 0},
        {"a", 0, 0},
        {"noauto", I18N_NOOP("Do not automatically connect to servers."), 0},
        {"+[URL]",
         I18N_NOOP
         ("Server/Channel to Open. example: irc://server:port/channel"), 0},
        {0, 0, 0}
    };




    int fe_args(int argc, char *argv[])
    {
        KAboutData *about = new KAboutData("vertigo", "Vertigo", VERSION,
                                           "An IRC Client for KDE",
                                           KAboutData::License_GPL,
                                           "(C) 2003 vertigo developers",
                                           "smt@inbox.lv", QString::null,
                                           "http://vertigo.berlios.de");


        kdDebug() << "fe_args" << endl;
        KCmdLineArgs::init(argc, argv, about);
        KCmdLineArgs::addCmdLineOptions(options);
        App::addCmdLineOptions();
        return 1;
    }

    void fe_init(void)
    {
        kdDebug() << "fe:fe_init" << endl;
        App * a = new App();
        //static App a;
//tmp=&a;
        KCmdLineArgs *args = KCmdLineArgs::parsedArgs();

        if (args->isSet("auto")) {
            auto_connect = 1;
        } else {
            auto_connect = 0;
        }
        QCString dir = args->getOption("cfgdir");

        if (!dir.isEmpty()) {
            if (dir[dir.length() - 1] == '/')
                dir.truncate(dir.length() - 1);
            xdir = dir.data();
        }
        // XXX: xchat doesn't support multiple args fix this
	
	char *aa=new char[1024];
        for (int i = 0; i < args->count(); i++) {
        	qstrcpy(aa,args->url(i).path().latin1());
		connect_url = aa;//args->url(i).path().latin1();
        }

        args->clear();

    }

    void fe_main(void)
    {
        kdDebug() << "fe:fe_main" << endl;
        g_inited = true;
        //KSystemTray *d=new KSystemTray(0);
        //d->setPixmap(KGlobal::iconLoader()->loadIconSet("tab_breakoff", KIcon::Small).pixmap());
        //d->show();
        xchatapp->exec();
        //xchatapp->processEvents();
        kdDebug() << "fe:fe_main---out of main loop" << endl;
        if (prefs.wait_on_exit) {
            ;
            CleanupHandler * h = new CleanupHandler();
            h->cleanup();
            //sleep(3);         // everything GUI is hidden
        }
    }

    void fe_exit(void)
    {
        kdDebug() << "fe:fe_exit" << endl;
        xchatapp->quit();

    }

    void fe_new_server(struct server *serv)
    {
        kdDebug() << "fe:newserver--->" << endl;
        server_gui *gui = new server_gui;
        gui->rawlog = 0;
        gui->chanlist = 0;
        serv->gui = gui;

    }

    void fe_message(char *msg, int wait)
    {
        kdDebug() << "fe:fe_message " << msg << " inited=" << g_inited <<
            endl;
        if (g_inited)
            KMessageBox::error(0, msg);
    }

    void fe_close_window(struct session *sess)
    {
        kdDebug() << "fe:fe_close_window" << endl;
	sess->gui->win->closeView(sess->gui->view);
	/*
        sess->gui->view->close(true);
        if (!sess->gui->win->viewCount())
            sess->gui->win->close(true);
        delete sess->gui->comp;
        delete sess->gui->cmdcomp;
        delete sess->gui;

        sess->gui = 0;
        kill_session_callback(sess);*/
    }

    void fe_beep(void)
    {
        kdDebug() << "fe:fe_beep" << endl;
    }

    void fe_add_rawlog(struct server *serv, char *text, int len, int outbound)
    {
        kdDebug() << "fe:fe_add_rawlog" << endl;
        if (!serv->gui->rawlog)
            return;
        QString str = outbound ? "\0034<<\t" : "\0033>>\t";
        str.append(text);
        serv->gui->rawlog->appendText(str);
    }

    void fe_set_topic(struct session *sess, char *topic)
    {
        kdDebug() << "fe:fe_set_topic: " << topic << endl;
        sess->gui->view->setTopic(topic);
    }
    void fe_cleanup(void)
    {
        kdDebug() << "fe:fe_cleanup" << endl;
    }
    void fe_set_hilight(struct session *sess)
    {
        kdDebug() << "fe:fe_set_hilight" << endl;
        sess->gui->win->setViewColor(sess->gui->view, Qt::blue);
    }
    void fe_update_mode_buttons(struct session *sess, char mode, char sign)
    {
        kdDebug() << "fe:fe_update_mode_buttons  " << sign << mode << endl;
    }
    void fe_update_channel_key(struct session *sess)
    {
        kdDebug() << "fe:fe_update_channel_key" << endl;
    }
    void fe_update_channel_limit(struct session *sess)
    {
        kdDebug() << "fe:fe_update_channel_limit" << endl;
    }
    int fe_is_chanwindow(struct server *serv)
    {
        kdDebug() << "fe:fe_is_chanwindow" << endl;
        if (!serv->gui->chanlist)
            return 0;
        return 1;

    }

    void fe_add_chan_list(struct server *serv, char *chan, char *users,
                          char *topic)
    {
        kdDebug() << "fe:fe_add_chan_list" << endl;
        serv->gui->chanlist->appendChannel(chan, users, topic);
    }
    void fe_chan_list_end(struct server *serv)
    {
        serv->gui->chanlist->enableItems(true);
        kdDebug() << "fe:fe_chan_list_end" << endl;
    }
    int fe_is_banwindow(struct session *sess)
    {
        kdDebug() << "fe:fe_is_banwindow" << endl;
        return 0;
    }
    void fe_add_ban_list(struct session *sess, char *chan, char *users,
                         char *topic)
    {
        kdDebug() << "fe:fe_add_ban_list" << endl;
    }
    void fe_ban_list_end(struct session *sess)
    {
        kdDebug() << "fe:fe_ban_list_end" << endl;
    }
    void fe_notify_update(char *name)
    {
        kdDebug() << "fe:fe_notify_update :" << name << endl;
    }
    void fe_event_emitted(char *name)
    {
        QString d = name;
        d.replace(' ', QString::null);
        kdDebug() << "fe:fe_event_emitted :" << d << endl;
        KNotifyClient::event(d);
    }
    void fe_text_clear(struct session *sess)
    {
        kdDebug() << "fe:fe_text_clear" << endl;
        sess->gui->view->getTextView()->clear();
    }
    void fe_progressbar_start(struct session *sess)
    {
        kdDebug() << "fe:fe_progressbar_start" << endl;
        sess->gui->showbar = true;
        if (current_sess == sess) {
            if (sess->gui->win->isCurrent(sess))
                sess->gui->win->showProgressBar();
        }
    }
    void fe_progressbar_end(struct server *serv)
    {
        kdDebug() << "fe:fe_progressbar_end" << endl;
        GSList *list = sess_list;
        session *sess;

        while (list) {
            sess = (session *) list->data;
            if (sess->server == serv) {
                sess->gui->showbar = false;
                if (current_sess == sess) {
                    if (sess->gui->win->isCurrent(sess))

                        sess->gui->win->hideProgressBar();
                }
            }
            list = list->next;
        }
    }

    void fe_userlist_insert(struct session *sess, struct User *newuser,
                            int row, int sel)
    {
        kdDebug() << "fe:fe_userlist_insert  row=" << row << "   sel=" <<
            sel << endl;
        sess->gui->view->insertUser(sess->server, newuser, row, sel);
        sess->gui->comp->addItem(newuser->nick);
    }

    int fe_userlist_remove(struct session *sess, struct User *user)
    {
        kdDebug() << "fe:fe_userlist_remove" << endl;

        sess->gui->comp->removeItem(user->nick);
        return sess->gui->view->removeUser(user);
    }
    void fe_userlist_move(struct session *sess, struct User *user,
                          int new_row)
    {
        kdDebug() << "fe:fe_userlist_move" << endl;
        fe_userlist_insert(sess, user, new_row,
                           fe_userlist_remove(sess, user));
    }
    void fe_userlist_numbers(struct session *sess)
    {
        kdDebug() << "fe:fe_userlist_numbers" << endl;
        if (current_sess == sess) {
            kdDebug() << "is_current_sess" << endl;
            if (sess->gui->win->isCurrent(sess))
                sess->gui->win->setNumbers(sess->ops, sess->total);
        }
    }

    void fe_userlist_clear(struct session *sess)
    {
        kdDebug() << "fe:fe_userlist_clear" << endl;
        sess->gui->view->clearUserlist();
    }

    void fe_dcc_add(struct DCC *dcc)
    {
        kdDebug() << "fe:fe_dcc_add" << endl;
    }

    void fe_dcc_update(struct DCC *dcc)
    {
        kdDebug() << "fe:fe_dcc_update" << endl;
    }



    void fe_clear_channel(struct session *sess)
    {
        kdDebug() << "fe:fe_clear_channel" << endl;
    }
    void fe_session_callback(struct session *sess)
    {
        kdDebug() << "fe:fe_session_callback" << endl;
    }
    void fe_server_callback(struct server *serv)
    {
        kdDebug() << "fe:fe_server_callback" << endl;
    }
    void fe_url_add(const char *text)
    {
        kdDebug() << "fe:fe_url_add     url=" << text << endl;
    }
    void fe_pluginlist_update(void)
    {
        kdDebug() << "fe:fe_pluginlist_update" << endl;
    }
    void fe_buttons_update(struct session *sess)
    {
        kdDebug() << "fe:fe_buttons_update" << endl;
    }
    void fe_dlgbuttons_update(struct session *sess)
    {
        kdDebug() << "fe:fe_dlgbuttons_update" << endl;
    }
    void fe_dcc_send_filereq(struct session *sess, char *nick, int maxcps)
    {
        kdDebug() << "fe:fe_dcc_send_filereq" << endl;
    }
    void fe_set_channel(struct session *sess)
    {
        kdDebug() << "fe:fe_set_channel" << endl;
        QString tip;

        if (sess->server->networkname) {
            tip = sess->server->networkname;
            tip = tip.append(":");
        }
        tip = tip.append(sess->channel);
        sess->gui->win->setViewLabel(sess->gui->view,
                                                 sess->channel);
        sess->gui->win->setViewToolTip(sess->gui->view, tip);
    }
    void fe_set_title(struct session *sess)
    {
        kdDebug() << "fe:fe_set_title" << endl;
        QString n;

        if (!sess) {
            kdDebug() << "fe:fe_set_title nosess" << endl;
            goto def;
        }


        if (sess != current_tab)
            return;

        if (sess->server->connected == FALSE && sess->type != SESS_DIALOG)
            goto def;

        switch (sess->type) {
        case SESS_DIALOG:
            n = QString("Dialog with %1 @ %2").arg(sess->channel).
                arg(sess->server->servername);
            break;
        case SESS_SERVER:
            n = QString("%1 @ %2").arg(sess->server->nick).arg(sess->
                                                               server->
                                                               servername);
            break;
        case SESS_CHANNEL:
            n = QString("%1 @ %2 / %3 (%4)").arg(sess->server->nick).
                arg(sess->server->servername).arg(sess->channel).arg(sess->
                                                                     current_modes
                                                                     ?
                                                                     sess->
                                                                     current_modes
                                                                     : "");
            break;
        case SESS_NOTICES:
        case SESS_SNOTICES:
            n = QString("%1 @ %2 (notices)").arg(sess->server->nick).
                arg(sess->server->servername);
            break;
        default:
          def:
            n = QString::null;
            break;
        }
        if (sess && sess->gui)
            sess->gui->win->setCaption(n);


    }
    void fe_set_nonchannel(struct session *sess, int state)
    {
        kdDebug() << "fe:fe_set_nonchannel" << endl;
    }
    void fe_set_nick(struct server *serv, char *newnick)
    {
        kdDebug() << "fe:fe_set_nick" << endl;
    }
    void fe_change_nick(struct server *serv, char *nick, char *newnick)
    {
        kdDebug() << "fe:fe_change_nick" << endl;
    }
    void fe_ignore_update(int level)
    {
        kdDebug() << "fe:fe_ignore_update" << endl;
    }
    int fe_dcc_open_recv_win(int passive)
    {
        kdDebug() << "fe:fe_dcc_open_recv_win" << endl;
    }
    int fe_dcc_open_send_win(int passive)
    {
        kdDebug() << "fe:fe_dcc_open_send_win" << endl;
    }
    int fe_dcc_open_chat_win(int passive)
    {
        kdDebug() << "fe:fe_dcc_open_chat_win" << endl;
    }
    void fe_userlist_hide(session * sess)
    {
        kdDebug() << "fe:fe_userlist_hide" << endl;
    }
    void fe_dcc_remove(struct DCC *dcc)
    {
        kdDebug() << "fe:fe_dcc_remove" << endl;
    }
    void fe_lastlog(session * sess, session * lastlog_sess, char *sstr)
    {
        kdDebug() << "fe:fe_lastlog" << endl;
    }
    void fe_set_lag(server * serv, int lag)
    {
        kdDebug() << "fe:fe_set_lag :" << lag << endl;
        GSList *list = sess_list;
        session *sess;
        while (list) {
            sess = (session *) list->data;
            if (sess->server == serv) {
                sess->gui->lag = lag * 10;
                if (current_sess == sess) {
                    if (sess->gui->win->isCurrent(sess))
                        sess->gui->win->setLag(lag * 10);
                }
            }
            list = list->next;
        }
    }
    void fe_set_throttle(server * serv)
    {
        kdDebug() << "fe:fe_set_throttle" << endl;
    }
    void fe_set_away(server * serv)
    {
        kdDebug() << "fe:setaway" << endl;
    }

    void fe_serverlist_open(session * sess)
    {
        kdDebug() << "fe:serverlistopen" << endl;
        Serverlist * l = xchatapp->serverList();

        l->setSession(sess);
        l->show();
        l->raise();
        l->setActiveWindow();
    }

    void fe_get_str(char *prompt, char *def, void *callback, void *ud)
    {
        kdDebug() << "fe:getstr" << endl;
        xchatapp->getString(prompt, def, callback, ud);

    }


    void fe_get_int(char *prompt, int def, void *callback, void *ud)
    {
        kdDebug() << "fe:getint" << endl;
        xchatapp->getInt(prompt, def, callback, ud);
    }

    void fe_ctrl_gui(session * sess, int action)
    {
        kdDebug() << "fe:ctrl_gui" << endl;

        switch (action) {
        case 0:
            sess->gui->win->hide();
            break;
        case 1:
            sess->gui->win->show();
            break;
        case 2:
            {
                sess->gui->win->show();
                sess->gui->win->raise();
                sess->gui->win->setActiveWindow();
                sess->gui->win->showView(sess->gui->view);
                break;
            }
        default:
            break;
        }
    }

// typically called when a hostname has been found
    void fe_userlist_rehash(session * sess, struct User *user)
    {
        kdDebug() << "fe:userlist_rehash" << endl;
        sess->gui->view->rehashUser(user);
    }

    void fe_play_wave(const char *file)
    {
        kdDebug() << "fe:playwav" << endl;
    }
    void fe_idle_add(void *func, void *data)
    {
        kdDebug() << "fe:idle_add" << endl;
        xchatapp->addIdleFunction(func, data);
    }
}

