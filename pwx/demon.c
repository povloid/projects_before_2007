/* ���� ���������� ����������� �����, ������� ��� ������ ����� � ���-���� ���
 �� ��������� � ������� �����, � ����� ��� ������� �������� SIGINT ���
 SIGQUIT ��� SIGTERM, �� �� � ��� �� ���-���� �����, ��� �� ��������� � �����
 ������� �����.. */

#include <unistd.h>
#include <stdio.h>
#include <syslog.h>
#include <signal.h>
#include <time.h>
/*����� ���� ���������� ���� ������, � ���� ������ ���������
 ���������������� ���� ������ �� ��������� ������-�� �������*/
inline void do_packet_loop()
{

while(1) pause();

}
 /*����� ���� ���������� �������, ������ ����� ��������� ������� ������, ��
   �� ����� ������ ����� ��������������*/
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
        setgid(65534);/*� ����� ������������ ������ UID � GID ��������*/
        setuid(65534);
        chdir("/");/*��������� �� ���, ���� �� ����������� �������� �������*/
        fclose(stdin);/*��� ��� ����� ������ �� ���������� �������� ��
			�����, �� ��������� stdout, stdin, stderr*/
        fclose(stdout);
       fclose(stderr);
       if(fork())  /*���������*/
                exit(0);
       setsid();  /*���������� �� ������������ ��������� � ��������� �
		   ������� �����*/
        for(int j=1; j<32; j++)/*����������� ������������ ���� �������� ������� fsignal*/
        signal(j,fsignal);

        time_t timv;
        struct tm *local_tm = localtime(&timv);

        syslog(LOG_NOTICE,"DAEMON_TST daemon started at %s",asctime(local_tm));
        do_packet_loop();/*��������������.. �-)*/
}
  
