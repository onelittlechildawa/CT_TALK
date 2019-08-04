#include <winsock2.h>
#include <io.h>
#include <iostream>
#include <fstream>
#include <windows.h>
//#include <thread>
#include <cstdio>
#include <cstring>
#include <list>
#include <time.h>
#pragma comment (lib,"ws2_32.lib")
#define STR_MAX_LEN 200
#define NAME_LEN        20
#define PWD_LEN         16
#define MML_LEN         10
#define SEND_LEN        STR_MAX_LEN + NAME_LEN + 2
#define DK 5328//调试使用5328 
using namespace std;

ofstream Sf;
const int   MAX_ONLINE   =  20;
const char  Server_IP[]  =  "127.0.0.1";//"172.17.0.3";
const char  Warn_Buf[]   =  "It's over Max connect!",
                            aok[2]="0",
                                   anok[2]="1",
                                           anok2[2]="2",
                                                   azok[2]="0",
                                                           aznok[2]="3",
                                                                   ml[100]="";
list < SOCKET > online;
bool            online_using =  0;
char name[NAME_LEN];
FILE *fp,*fp2;
void NewHandle(SOCKET &SockFrom);
void Wrong_exit(const char* Error_place, int _Code) {
	printf("%s wrong!(at: %d)\n", Error_place, _Code);
	system("pause");
	exit(_Code);
}

void show_time() {
	static char time_l[20], time_n[20];
	static time_t timep;
	{
		time(&timep);
		strftime(time_n, sizeof(time_n), "%Y-%m-%d %H:%M", localtime(&timep));
		if (strcmp(time_l, time_n)) {
			strcpy(time_l, time_n);
			printf("//             %s\n", time_n);
		}
	}
}

void broadcast(const char* name, const char* send_buf) {
	static char ls_send[SEND_LEN];

	strcpy(ls_send, name);
	strcat(ls_send, ": ");
	strcat(ls_send, send_buf);
	freopen("rr.txt","a",stdout);
	printf("%s\n",ls_send);
	freopen("CON","a",stdout);

	while(online_using);
	online_using = 1;

	for(auto i = online.begin(); i != online.end(); i++)
		send(*i, ls_send, SEND_LEN, 0);

	online_using = 0;
}

void flash_online_num() {
	static char title[100];
	if(online.empty()) strcpy(title, "title=bb-xjrjyy-Server");
	else sprintf(title, "title=bb-xjrjyy-Server online_now:: %d", online.size());
	system(title);
}

DWORD WINAPI child_thread(LPVOID V_sock) {
	SOCKET hsock = (SOCKET) V_sock;
	char namea[NAME_LEN],recvbuf[STR_MAX_LEN],mml[MML_LEN];
	strcpy(namea,name);
	show_time();
	printf("%s login Successful!\n", namea);
	flash_online_num();
	while(true) {
		int fhz=recv(hsock, mml, MML_LEN, 0);
		if(!strcmp(recvbuf, "ord::EXIT")||fhz==SOCKET_ERROR||fhz==0) break;
		if(strcmp(mml,"lt")==0) {
			fhz=recv(hsock, recvbuf, STR_MAX_LEN, 0);
			if(!strcmp(recvbuf, "ord::EXIT")||fhz==SOCKET_ERROR||fhz==0) break;
			broadcast(name, recvbuf);
		} else {
			if(strcmp(mml,"tc")==0/*strcmp(mml,"zc")==0||strcmp(mml,"dl")==0*/) {
				break;
			}
		}
		Sleep(10);
	}
	online_using = 1;
	online.remove(hsock);
	online_using = 0;

	show_time();
	printf("%s exited!\n", name);
	flash_online_num();
	return 0;
}

void NewHandle(SOCKET &SockFrom) {
	HANDLE  ls_handle;
	DWORD    ls_handle_id;
	ls_handle = (HANDLE)::CreateThread(NULL, 0,
	                                   child_thread, (LPVOID)SockFrom, 0, &ls_handle_id);
}

int main() {
	system("chcp 936>nul");
	system("title=bb-xjrjyy-Server");
	puts("bb-xjrjyy-Server");
	puts("                                        powerd by xjrjyy");

	WSADATA wsd;
	WSAStartup(MAKEWORD(2, 2), &wsd);
	SOCKET SockServer;
	sockaddr_in ServerAddr, FromAddr;

	ServerAddr.sin_family                =   AF_INET;
	ServerAddr.sin_port                  =   htons(DK);
	ServerAddr.sin_addr.S_un.S_addr  =   inet_addr(Server_IP);

	SockServer = socket(AF_INET, SOCK_STREAM, 0);

	if(bind(SockServer, (sockaddr*)&ServerAddr, sizeof(ServerAddr)) == SOCKET_ERROR)
		Wrong_exit("bind", 1);

	if(listen(SockServer, MAX_ONLINE) == SOCKET_ERROR)
		Wrong_exit("listen", 2);

	int Socklen = sizeof(sockaddr);
	while(true) {
		SOCKET SockFrom;
		SockFrom = accept(SockServer, (sockaddr*)&FromAddr, &Socklen);
		if(SockFrom != INVALID_SOCKET) {
			if(online.size() < MAX_ONLINE) {
//              while(online_using)
//                  online_using = 1;
				char pwd[PWD_LEN],pwdtmp[PWD_LEN],mml[MML_LEN];
				char recvbuf    [STR_MAX_LEN],tmpbuf[NAME_LEN+10],tmpcd[NAME_LEN+105];
				bool zcf=0,dlf=0;
				recv(SockFrom, mml, MML_LEN, 0);
				cout<<"\n-----------接收旗帜------------\n"<<mml<<endl;
				if(mml[0]=='t') {
					recv(SockFrom, mml, MML_LEN, 0);
				} else if(mml[0]=='z') {
					memset(pwd,0,sizeof(pwd));
					memset(name,0,sizeof(name));
					cout<<"some one want to reg\n";
					zcf=1;
					recv(SockFrom, name, NAME_LEN, 0);
					recv(SockFrom, pwd, PWD_LEN, 0);
					cout<<"copy inf\n";
				} else if(mml[0]=='d') {
					memset(pwd,0,sizeof(pwd));
					memset(name,0,sizeof(name));
					dlf=1;
					recv(SockFrom, name, NAME_LEN, 0);
					recv(SockFrom, pwd, PWD_LEN, 0);
//                	cout<<"---------"<<pwd<<"----------\n";
				}
				sprintf(tmpcd,"%s.txt",name);
//              cout<<zcf<<"------------"<<name<<endl;
				if(zcf) {
					cout<<"some one want to reg with pas:"<<pwd<<" and name "<<name<<endl;
//					cout<<tmpcd<<endl;
					if(freopen(name,"r",stdin)==NULL) {
						cout<<"not found so didn't reg\n";
						fclose(stdin);
						freopen(name,"w",stdout);
//						freopen(tmpcd,"w",stdout);
						printf("%s\n\n",pwd);
						freopen("CON","a",stdout);
						printf("%s register Successful!\n", name);
						send(SockFrom, azok, strlen(azok), 0);
						continue;
					}
					//scanf("%s",pwdtmp)==-1
					else {
						cout<<"have been\n";
						send(SockFrom, aznok, strlen(aznok), 0);
						fclose(stdin);
						continue;
					}
				} else if(dlf) {
					//在这里写登陆判定
					printf("some one want to login with name: %s and pas: %s\n", name, pwd);
					if(freopen(name,"r",stdin)==NULL) {
						cout<<"false login didn't reg'!\n";
						send(SockFrom, "F", strlen("F"), 0);
						continue;
					} else {
						fclose(stdin);
						freopen(name,"r",stdin);
						string a;
						cin>>a;
						cout<<"getpas is "<<name<<" "<<a<<endl;
						fclose(stdin);
						if(pwd!=a) {
							cout<<"false!\n";
							send(SockFrom, "W", strlen("W"), 0);
							continue;
						} else {
							cout<<"successful!\n";
							send(SockFrom, "T", strlen("T"), 0);
							char his[STR_MAX_LEN],cp[1];
							freopen("rr.txt","r",stdin);
//							string a;
							while(cp[0]!=EOF)
							{
								cp[0]=getchar();
//								cout<<his<<endl;
								send(SockFrom,cp,1,0);
							}
							fclose(stdin);
						}
					}
//                    if()
					/*freopen(tmpcd,"r",stdin);
					if(scanf("%s",pwdtmp)==-1||strcmp(pwdtmp,"")==0) {
					    send(hsock, anok2, int(sizeof(anok2)/sizeof(char)), 0);
					    fclose(stdin);
					    continue;
					}
					fclose(stdin);
					if(strcmp(pwd,pwdtmp)==0) {
					send(hsock, aok, int(sizeof(aok)/sizeof(char)), 0);
					} else {
					    send(hsock, anok, int(sizeof(anok)/sizeof(char)), 0);
					    return 0;
					}*/
				} else {
					online_using = 0;
					continue;
				}

				recv(SockFrom, recvbuf, STR_MAX_LEN, 0);
				online.push_back(SockFrom);
				online_using = 0;
				NewHandle(SockFrom);
			} else {
				send(SockFrom, Warn_Buf, strlen(Warn_Buf), 0);
				closesocket(SockFrom);
			}
		}
		Sleep(10);
	}
	WSACleanup();
	fclose(stdout);
	return 0;
}
