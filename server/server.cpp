
//bug在102行 



//-std=c++11 -lwsock32 -lgdi32
#include<windows.h>
#include<bits/stdc++.h>
#define STR_MAX_LEN 200
#define NAME_LEN 20
#define PWD_LEN  16
#define MML_LEN  10
#define SEND_LEN STR_MAX_LEN+NAME_LEN+2
#define DK 5328
using namespace std;
char recvname[100];
ofstream Sf;
const int MAX_ONLINE=40;
const char Server_IP[]="172.17.0.3";
const char Warn_Buf[]="It's over Max connect!",aok[2]="0",anok[2]="1",anok2[2]="2",azok[2]="0",aznok[2]="3",ml[100]="";
vector<SOCKET>online;
vector<string>q;
map<string,map<string,bool> >ATtimepd;
map<string,double>ATstart;
map<string,double>ATEnd;
map<string,double>ltstart;
map<string,double>ltend;
map<string,bool>lttimepd;
map<string,bool>onlinepd;
map<SOCKET,string>onlinename;
map<string,bool>talkname;
char name[NAME_LEN];
FILE *fp,*fp2;
void Wrong_exit(const char* Error_place,int _Code)
{
	printf("%s wrong!(at: %d)\n",Error_place,_Code);
	system("pause");
	exit(_Code);
}
char time_l[20],time_n[20];
time_t timep;
void show_time()
{
	time(&timep);
	strftime(time_n, sizeof(time_n),"%Y-%m-%d %H:%M",localtime(&timep));
	if(strcmp(time_l,time_n))
	{
		strcpy(time_l,time_n);
		printf("//             %s\n",time_n);
	}
}
char ls_send[SEND_LEN];
void broadcast(const char* send_buf)
{
	strcpy(ls_send,name);
	strcat(ls_send,": ");
	strcat(ls_send,send_buf);
	for(auto i=online.begin(); i!=online.end(); i++)send(*i,ls_send,SEND_LEN,0);
}
char title[100];
void flash_online_num()
{
	if(online.empty()) strcpy(title, "title=bb-xjrjyy-Server");
	else sprintf(title, "title=bb-xjrjyy-Server online_now:: %d", online.size());
	system(title);
}
DWORD WINAPI child_thread(LPVOID V_sock)
{
	SOCKET hsock=(SOCKET)V_sock;
	char namea[NAME_LEN],recvbuf[STR_MAX_LEN],mml[MML_LEN];
	strcpy(namea,name);
	show_time();
	printf("%s login Successful!\n", namea);
	FILE*fin=fopen(namea,"r");
	fscanf(fin,"%s",mml);
	fscanf(fin,"%s",mml);
	fclose(fin);
	if(mml[0]=='0')send(hsock,"0",2,0);
	else send(hsock,"1",2,0);
	flash_online_num();
	while(1)
	{
		memset(mml,0,sizeof(mml));
		int fhz=recv(hsock,mml,MML_LEN,0);
		if(!strcmp(recvbuf,"ord::EXIT")||fhz==SOCKET_ERROR||fhz==0)break;
		if(strcmp(mml,"del")==0)
		{
			fhz=recv(hsock,recvbuf,STR_MAX_LEN,0);
			FILE*fin=fopen(recvbuf,"r");
			if(fin==NULL)
			{
				send(hsock,"失败",5,0);
				continue;
			}
			char abc[100];
			fscanf(fin,"%s",abc);
			fscanf(fin,"%s",abc);
			fclose(fin);
			if(abc[0]=='0')
			{
				int k=remove(recvbuf);
				if(k==0)send(hsock,"成功",5,0);
				else send(hsock,"失败",5,0);
			}
			else send(hsock,"失败",5,0);
		}
		//----------------------------------------------------------------------------
		else if(strcmp(mml,"cannottalk")==0)
		{
			fhz=recv(hsock,recvname,25,0);
			string KKK=recvname;//---------------------------------------------------然后接受用户姓名，recvname传进来就为空 ，不知道为什么 
			cout<<"jinyan: -"<<KKK<<"-"<<endl;
			for(auto i=online.begin(); i!=online.end(); i++)
			{
				send(*i,"someonecannottalk",18,0);
				send(*i,recvname,strlen(recvname)+1,0);
			}
		}
		//-----------------------------------------------------------------------------
		else if(strcmp(mml,"搜索")==0)
		{
			fhz=recv(hsock,recvbuf,STR_MAX_LEN,0);
			FILE*fin=fopen(recvbuf,"r");
			if(fin==NULL)
			{
				send(hsock,"%2",3,0);
				continue;
			}
			char abc[100];
			fscanf(fin,"%s",abc);
			fscanf(fin,"%s",abc+1);
			abc[0]='%';
			send(hsock,abc,strlen(abc)+1,0);
			fclose(fin);
		}
		else if(strcmp(mml,"lt")==0)
		{
			fhz=recv(hsock,recvbuf,STR_MAX_LEN,0);
			if(!strcmp(recvbuf,"ord::EXIT")||fhz==SOCKET_ERROR||fhz==0)break;
			strcpy(name,namea);
			string k=recvbuf;
			k=k+' ';
			if(lttimepd[name]==0)
			{
				lttimepd[name]=1;
				ltstart[name]=clock();
			}
			else
			{
				ltend[name]=clock();
				double cha=ltend[name]-ltstart[name];
				if(cha<=750.0)
				{
					send(hsock,"LTtooquick",8,0);
					continue;
				}
				else lttimepd[name]=0;
			}
			for(int i=0; i<q.size(); i++)
			{
				string kkksc03='@'+q[i]+' ';
				if(k.find(kkksc03)!=string::npos)
				{
					if(ATtimepd[name][recvbuf]==0)
					{
						ATstart[name]=clock();
						ATtimepd[name][recvbuf]=1;
						send(online[i],"@@@",4,0);
					}
					else
					{
						ATEnd[name]=clock();
						double Timecha=double(ATEnd[name]-ATstart[name]);
						if(Timecha<=8000.0)send(hsock,"@TIMEtooshort",13,0);
						else
						{
							ATtimepd[name][recvbuf]=0;
							send(online[i],"@@@",4,0);
						}
					}
				}
			}
			broadcast(recvbuf);
		}
		else if(strcmp(mml,"tc")==0)break;
		else if(strcmp(mml,"用户")==0)
		{
			for(int i=0; i<q.size(); i++)
			{
				string kkk='*'+q[i];
				const char* c=kkk.c_str();
				send(hsock,c,strlen(c)+1,0);
			}
		}
		Sleep(10);
	}
	for(int i=0; i<q.size(); i++)
		if(q[i]==(string)namea)
		{
			q.erase(q.begin()+i);
			online.erase(online.begin()+i);
			break;
		}
	show_time();
	printf("%s exited!\n",name);
	flash_online_num();
	return 0;
}
void NewHandle(SOCKET& SockFrom)
{
	HANDLE ls_handle;
	DWORD ls_handle_id;
	ls_handle=(HANDLE)::CreateThread(NULL,0,child_thread,(LPVOID)SockFrom,0,&ls_handle_id);
}
int main()
{
	WSADATA wsd;
	WSAStartup(MAKEWORD(2,2),&wsd);
	SOCKET SockServer;
	sockaddr_in ServerAddr,FromAddr;
	ServerAddr.sin_family=AF_INET;
	ServerAddr.sin_port=htons(DK);
	ServerAddr.sin_addr.S_un.S_addr=inet_addr(Server_IP);
	SockServer=socket(AF_INET,SOCK_STREAM,0);
	if(bind(SockServer,(sockaddr*)&ServerAddr,sizeof(ServerAddr))==SOCKET_ERROR)Wrong_exit("bind",1);
	if(listen(SockServer,MAX_ONLINE)==SOCKET_ERROR)Wrong_exit("listen",2);
	int Socklen=sizeof(sockaddr);
	puts("服务器正常开始运行");
	while(1)
	{
		SOCKET SockFrom;
		SockFrom=accept(SockServer,(sockaddr*)&FromAddr,&Socklen);
		if(SockFrom!=INVALID_SOCKET)
		{
			if(online.size()<MAX_ONLINE)
			{
				char pwd[PWD_LEN],pwdtmp[PWD_LEN],mml[MML_LEN],recvbuf[STR_MAX_LEN],tmpbuf[NAME_LEN+10];
				bool zcf=0,dlf=0;
				recv(SockFrom,mml,MML_LEN,0);
				cout<<"\n-----------接收旗帜------------\n"<<mml<<endl;
				if(mml[0]=='t')recv(SockFrom,mml,MML_LEN,0);
				else if(mml[0]=='z')
				{
					memset(pwd,0,sizeof(pwd));
					memset(name,0,sizeof(name));
					zcf=1;
					recv(SockFrom,name,NAME_LEN,0);
					recv(SockFrom,pwd,PWD_LEN,0);
					cout<<"copy inf\n";
				}
				else if(mml[0]=='d')
				{
					memset(name,0,sizeof(name));
					memset(pwd,0,sizeof(pwd));
					dlf=1;
					recv(SockFrom,name,NAME_LEN,0);
					recv(SockFrom,pwd,PWD_LEN,0);
				}
				if(zcf)
				{
					cout<<"some one want to reg with pas:"<<pwd<<" and name "<<name<<endl;
					FILE*fin=fopen(name,"r");
					if(fin==NULL)
					{
						cout<<"not found so didn't reg\n";
						fclose(fin);
						FILE*fout=fopen(name,"w");
						fprintf(fout,"%s 0",pwd);
						fclose(fout);
						printf("%s register Successful!\n", name);
						send(SockFrom,azok,strlen(azok), 0);
						continue;
					}
					else
					{
						cout<<"have been\n";
						send(SockFrom,aznok,strlen(aznok),0);
						fclose(fin);
						continue;
					}
				}
				else if(dlf)
				{
					if(onlinepd[name]==0)onlinepd[name]=1;
					else
					{
						for(auto i=online.begin(); i!=online.end(); i++)
						{
							if(onlinename[*i]==name)
							{
								send(*i,"dlrepeat",9,0);
								break;
							}
						}
					}
					printf("some one want to login with name: %s and pas: %s\n",name,pwd);
					FILE*fin=fopen(name,"r");
					if(fin==NULL)
					{
						cout<<"false login didn't reg'!\n";
						send(SockFrom,"F",strlen("F"),0);
						continue;
					}
					else
					{
						char asd[NAME_LEN];
						fscanf(fin,"%s",asd);
						string a=asd;
						if(pwd!=a)
						{
							cout<<"false!\n";
							send(SockFrom,"W",strlen("W"),0);
							continue;
						}
						else
						{
							cout<<"successful!\n";
							send(SockFrom,"T",strlen("T"),0);
						}
						fclose(fin);
					}
				}
				else continue;
				q.push_back(name);
				onlinename[SockFrom]=name;
				online.push_back(SockFrom);
				NewHandle(SockFrom);
			}
			else
			{
				send(SockFrom,Warn_Buf,strlen(Warn_Buf),0);
				closesocket(SockFrom);
			}
		}
		Sleep(10);
	}
	return 0;
}
