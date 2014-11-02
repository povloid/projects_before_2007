/* Ниже приводится простенький демон, который при старте пишет в лог-файл что
 он стартовал и текущее время, а когда его убивают сигналом SIGINT или
 SIGQUIT или SIGTERM, то он в тот же лог-файл пишет, что он пришиблен и пишет
 текущее время.. */

#include <unistd.h>
#include <stdio.h>
#include <syslog.h>
#include <signal.h>
#include <time.h>
/*Здесь идет собственно тело демона, в моем случае программа
 приостанавливает свою работу до получения какого-то сигнала*/
inline void do_packet_loop()
{

while(1) pause();

}
 /*здесь идет обработчик сигнала, тоесть когда программа получит сигнал, то
   он будет именно здесь обрабатываться*/
void fsignal(int sig)
{
        switch(sig)
        {
                case SIGTERM:
                case SIGINT:
                case SIGQUIT:
                        time_t ti_mv;
                        tm *local_tim;
                        local_tim  = localtime(&ti_mv);
                        syslog(LOG_NOTICE,"DAEMON_TST daemon stopped on signal\
                                           %d at %s",sig,asctime(local_tim));
                        closelog();
                        exit(0);
                        break;
                default:
                        syslog(LOG_NOTICE,"got signal %d. ignore...",sig);
                        break;
        }
}

int main(int argc,char** argv)
{
        setgid(65534);/*в целях безопасности меняем UID и GID процесса*/
        setuid(65534);
        chdir("/");/*переходим на рут, чтоб не блокировать файловые системы*/
        fclose(stdin);/*так как демон ничего не собирается выводить на
			экран, то закрываем stdout, stdin, stderr*/
        fclose(stdout);
       fclose(stderr);
       if(fork())  /*форкаемся*/
                exit(0);
       setsid();  /*отрываемся от управляющего терминала и переходим в
		   фоновый режим*/
        for(int j=1; j<32; j++)/*настраиваем обработчиком всех сигналов функцию fsignal*/
        signal(j,fsignal);

        time_t timv;
        struct tm *local_tm = localtime(&timv);

        syslog(LOG_NOTICE,"DAEMON_TST daemon started at %s",asctime(local_tm));
        do_packet_loop();/*демонизируемся.. Ж-)*/
}
  
