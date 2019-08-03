#include<windows.h>
#include<windowsx.h>
#include<bits/stdc++.h>
#define STR_MAX_LEN	200
#define MEM_STR_LEN	10000
#define IN_TEXT		101
#define OUT_TEXT	201
#define B_DOWN		301
#define qd_DOWN		401
#define zc_DOWN		601
#define cond 0.6
#define cond3 0.618
#define NAME_MAX_LEN 20
#define PWD_MAX_LEN 16
#define NAME_MIN_LEN 3
#define PWD_MIN_LEN 6
#define IN_MAX_CO 5
#define DK 5328
using namespace std;
const int W_s=11,W_ss=15;
const char zcc[]="zc",dlc[]="dl",ltc[]="lt",tcc[]="tc",quake[]="quake",o_IP[]="127.0.0.1";
char sendbuf[STR_MAX_LEN],recvbuf[STR_MAX_LEN],memobuf[MEM_STR_LEN],namebuf[NAME_MAX_LEN+1],pwdbuf[PWD_MAX_LEN+1];
SOCKET connection;
bool outlock=0;
int cgd=-1;
void hwndOutput_add_buf(HWND, const char*, bool n = 1);
char time_l[20], time_n[20];
time_t timep;
void show_time(HWND hwndout) {
	time(&timep);
	strftime(time_n, sizeof(time_n),"%Y-%m-%d %H:%M", localtime(&timep));
	if(strcmp(time_l, time_n)) {
		strcpy(time_l,time_n);
		strcat(memobuf,"//             ");
		strcat(memobuf,time_n);
		strcat(memobuf,"\r\n");
	}
}
SOCKET gotsock(HWND hwndmind) {
	WSADATA wsd;
	WSAStartup(MAKEWORD(2,2),&wsd);
	SOCKET SockUser;
	sockaddr_in ServerAddr;
	ServerAddr.sin_family          =AF_INET;
	ServerAddr.sin_port            =htons(DK);
	ServerAddr.sin_addr.S_un.S_addr=inet_addr(o_IP);
	SockUser = socket(AF_INET, SOCK_STREAM, 0);
	if (connect(SockUser, (sockaddr*)&ServerAddr, sizeof(sockaddr)) != SOCKET_ERROR) {
		strcat(memobuf, "//========服务器连接成功========//\r\n");
		hwndOutput_add_buf(hwndmind, "", 0);
		return SockUser;
	} else {
		MessageBox(NULL,"服务器不在线!","Error!",MB_ICONEXCLAMATION|MB_OK);
		exit(0);
	}
}
void w_send(HWND hwndin,HWND hwndlog) {
	GetWindowText(hwndin,sendbuf,STR_MAX_LEN);
	SetWindowText(hwndin, "");
	if(!strcmp(sendbuf,"ord::cls")) {
		strcpy(memobuf,"//==========已清理内存==========//");
		hwndOutput_add_buf(hwndlog,"");
		return;
	}
	send(connection,ltc,strlen(ltc),0);
	send(connection,sendbuf,STR_MAX_LEN,0);
	if(!strcmp(sendbuf,"ord::EXIT"))exit(0);
}
DWORD WINAPI w_recv(LPVOID hwndout) {
	HWND hwndOut=(HWND)hwndout;
	int noxtime=0;
	while(noxtime<3) {
		recv(connection,recvbuf,STR_MAX_LEN, 0);
		if(*recvbuf) {
			noxtime = 0;
			hwndOutput_add_buf(hwndOut, recvbuf);
		} else noxtime++;
		*recvbuf='\0';
	}
	MessageBox(NULL,"服务器已下线,将退出程序!","Error!",MB_ICONEXCLAMATION|MB_OK);
	exit(0);
}
int zc(LPVOID hwndout) {
	HWND hwndOut=(HWND)hwndout;
	int aaaa;
	send(connection,zcc,strlen(zcc), 0);
	hwndOutput_add_buf(hwndOut, "//=!已发送至服务器!=//");
	send(connection,namebuf,strlen(namebuf), 0);
	send(connection,pwdbuf,strlen(pwdbuf), 0);
	int noxtime=0;
	*recvbuf='\0';
	while(noxtime<3) {
		recv(connection, recvbuf, STR_MAX_LEN, 0);
		if(*recvbuf) {
			noxtime = 0;
			break;
		} else noxtime++;
		*recvbuf='\0';
	}
	if(noxtime==3) {
		MessageBox(NULL,"服务器已下线,将退出程序!","Error!",MB_ICONEXCLAMATION|MB_OK);
		exit(0);
	}
	if(recvbuf[0]=='3') {
		hwndOutput_add_buf(hwndOut,"注册失败！账户重复！请重启后再次尝试|3秒后自动.");
		Sleep(3000);
		string a="start "+(string)(_pgmptr);
		system(a.c_str());
		exit(0);
	} else {
		hwndOutput_add_buf(hwndOut,"注册成功!请重启后登录!|3秒后自动.");
		Sleep(3000);
		string a="start "+(string)(_pgmptr);
		system(a.c_str());
		exit(0);
	}
	sscanf(recvbuf,"%d",&aaaa);
	return aaaa;
}
int dltry=0;
int dl(LPVOID hwndout) { //登陆?gin？?
	dltry+=1;
	HWND hwndOut=(HWND) hwndout;
	int aaaa;
	send(connection, dlc, strlen(dlc), 0);
	send(connection, namebuf, strlen(namebuf), 0);
	hwndOutput_add_buf(hwndOut, "//=!已发送至服务器!=//");
	send(connection, pwdbuf, strlen(pwdbuf), 0);
	int noxtime = 0;
	while(noxtime<3) {
		recv(connection,recvbuf, STR_MAX_LEN, 0);
		if(recvbuf[0]=='F') {
			hwndOutput_add_buf(hwndOut, "登录失败，账户不存在！\n请重启后再次尝试，三秒后自动！");
			Sleep(4000);
			string a = "start "+(string)(_pgmptr);
			system(a.c_str());
			exit(0);
			return 2;
		} else if(recvbuf[0]=='T') {
			return 0;
		} else if(recvbuf[0]=='W') {
			hwndOutput_add_buf(hwndOut, "登录失败，密码错误！\n请重启后再次尝试，三秒后自动！");
			Sleep(3000);
			string a="start "+(string)(_pgmptr);
			system(a.c_str());
			exit(0);
			return 1;
		}
		if(*recvbuf) {
			noxtime=0;
			break;
		} else noxtime++;
		*recvbuf='\0';
	}
	if(noxtime==3) {
		hwndOutput_add_buf(hwndOut,"//=!服务器已下线,3秒后退出程序!=//");
		Sleep(3000);
		exit(0);
	}
	sscanf(recvbuf,"%d",&aaaa);
	return aaaa;
}
int len;
void hwndOutput_add_buf(HWND hwnd,const char *buf,bool n) {
	while(outlock);
	outlock=1;
	len=strlen(memobuf);
	if(len>MEM_STR_LEN-500)strcpy(memobuf,"//=========自动清理内存=========//");
	show_time(hwnd);
	if(len>MEM_STR_LEN-1000)strcpy(memobuf,"//=!即将清理历史;请注意保存数据!=//");
	strcat(memobuf, buf);
	if(n) strcat(memobuf, "\r\n");
	SetWindowText(hwnd, memobuf);
	SendMessage(hwnd, EM_LINESCROLL, 0, Edit_GetLineCount(hwnd));
	outlock=0;
}
HDC hdc;
PAINTSTRUCT ps;
HANDLE wc_listen;
DWORD wc_listenID;
HWND B_send,qd_send,tc_send,zc_send;
HWND hwndOut,hwndIn,hwndts,hwndname,hwndpwd;
int wmID;
int tu0=-6,step;
HWND hwnd;
LRESULT CALLBACK WndProc(HWND hwnd,UINT Message,WPARAM wParam,LPARAM lParam) {
	switch(Message) {
		case WM_CREATE: {
			hwndOut=CreateWindow(TEXT("edit"),NULL,WS_CHILD|WS_VISIBLE|WS_VSCROLL|WS_BORDER|ES_LEFT|ES_MULTILINE|ES_AUTOVSCROLL|WS_DISABLED,
			                     0,0,620,346,hwnd,(HMENU)OUT_TEXT,((LPCREATESTRUCT)lParam)->hInstance,NULL);
			hwndIn=CreateWindow(TEXT("edit"),NULL,WS_CHILD|WS_VISIBLE|WS_HSCROLL|WS_VSCROLL|WS_BORDER|ES_LEFT|ES_MULTILINE|ES_AUTOVSCROLL,
			                    0,346,488,92,hwnd,(HMENU)IN_TEXT,((LPCREATESTRUCT)lParam)->hInstance,NULL);
			B_send=CreateWindow(TEXT("BUTTON"),TEXT("发送"),WS_VISIBLE|WS_CHILD|BS_DEFPUSHBUTTON,
			                    488,346,132,92,hwnd,(HMENU)B_DOWN,(HINSTANCE)GetWindowLong(hwnd,-6),NULL);
			hwndts=CreateWindow(TEXT("edit"),TEXT("用户名:\r\n    密码:"),WS_CHILD|WS_VISIBLE|WS_BORDER|ES_LEFT|ES_MULTILINE|WS_DISABLED,
			                    200,180,59,36,hwnd,(HMENU)IN_TEXT,((LPCREATESTRUCT)lParam)->hInstance,NULL);
			hwndname=CreateWindow(TEXT("edit"),NULL,WS_CHILD|WS_VISIBLE|WS_BORDER|ES_LEFT|ES_MULTILINE,
			                      259,180,180,18,hwnd,(HMENU)NAME_MAX_LEN,((LPCREATESTRUCT)lParam)->hInstance,NULL);
			hwndpwd=CreateWindow(TEXT("edit"),NULL,WS_CHILD|WS_VISIBLE|WS_BORDER|ES_LEFT|ES_MULTILINE,
			                     259,198,180,18,hwnd,(HMENU)PWD_MAX_LEN,((LPCREATESTRUCT)lParam)->hInstance,NULL);
			qd_send=CreateWindow(TEXT("BUTTON"),TEXT("登录"),WS_VISIBLE|WS_CHILD|BS_DEFPUSHBUTTON,
			                     259,216,72,36,hwnd,(HMENU)qd_DOWN,(HINSTANCE)GetWindowLong(hwnd,tu0),NULL);
			zc_send=CreateWindow(TEXT("BUTTON"),TEXT("注册"),WS_VISIBLE|WS_CHILD|BS_DEFPUSHBUTTON,
			                     331,216,72,36,hwnd,(HMENU)zc_DOWN,(HINSTANCE)GetWindowLong(hwnd,tu0),NULL);
			connection=gotsock(hwndOut);
			Edit_LimitText(hwndIn,STR_MAX_LEN);
			Edit_LimitText(hwndname,NAME_MAX_LEN);
			Edit_LimitText(hwndpwd,PWD_MAX_LEN);
			break;
		}
		case WM_PAINT: {
			hdc=BeginPaint(hwnd, &ps);
			EndPaint(hwnd,&ps);
			break;
		}
		case WM_CLOSE: {
			exit(0);
		}
		case WM_COMMAND: {
			wmID=LOWORD(wParam);
			switch(wmID) {
				case B_DOWN: {
					w_send(hwndIn,hwndOut);
					break;
				}
				case qd_DOWN: {
					memset(namebuf,0,sizeof namebuf);
					memset(pwdbuf,0,sizeof pwdbuf);
					GetWindowText(hwndname, namebuf, NAME_MAX_LEN);
					GetWindowText(hwndpwd, pwdbuf, PWD_MAX_LEN);
					if(strlen(namebuf)<NAME_MIN_LEN)strcat(memobuf,"名字过短!\r\n");
					else if(strlen(namebuf)>NAME_MAX_LEN)strcat(memobuf,"名字过长!\r\n");
					else if(strlen(pwdbuf)<PWD_MIN_LEN)strcat(memobuf,"密码过短!\r\n");
					else if(strlen(pwdbuf)>PWD_MAX_LEN)strcat(memobuf,"密码过长!\r\n");
					else {
						if(cgd==0)send(connection,tcc,strlen(tcc),0);
						cgd=dl((LPVOID)hwndOut);
						if(cgd==0) {
							strcat(memobuf,"登录成功!\r\n");
							step=1;
							wc_listen=(HANDLE)::CreateThread(NULL, 0, w_recv, (LPVOID)hwndOut, 0, &wc_listenID);
						} else {
							if(cgd==1)
								strcat(memobuf,"密码错误!请退出后重新登录！两秒后自动！\r\n");
							if(cgd==2)
								strcat(memobuf,"账号不存在!请退出后重新登录！两秒后自动！\r\n");
						}
					}
					SetWindowText(hwndOut, memobuf);
					break;
				}
				case zc_DOWN: {
					int cg;
					GetWindowText(hwndname, namebuf, NAME_MAX_LEN);
					GetWindowText(hwndpwd, pwdbuf, PWD_MAX_LEN);
					if(strlen(namebuf)<NAME_MIN_LEN)
						strcat(memobuf,"名字过短!\r\n");
					else if(strlen(namebuf)>NAME_MAX_LEN)
						strcat(memobuf,"名字过长!\r\n");
					else if(strlen(pwdbuf)<PWD_MIN_LEN)
	                    MessageBox(hwnd,"密码过短!", "Error!",MB_OK);
					else if(strlen(pwdbuf)>PWD_MAX_LEN)
	                    MessageBox(hwnd,"密码过长!", "Error!",MB_OK);
					else {
						if(cg==3)strcat(memobuf,"账号已注册!\r\n");
					}
					SetWindowText(hwndOut, memobuf);
					break;
				}
			}
			break;
		}
		default:
			return DefWindowProc(hwnd,Message,wParam,lParam);
	}
	return 0;
}
int WINAPI WinMain(HINSTANCE hInstance,HINSTANCE hPrevInstance,LPSTR lpCmdLine,int nCmdShow) {
	srand(static_cast<unsigned int>(time(0)));
	WNDCLASSEX wc;
	memset(&wc,0,sizeof(wc));
	MSG Msg;
	wc.hCursor      =LoadCursor(NULL, IDC_ARROW);
	wc.hIconSm      =LoadIcon(NULL,IDI_APPLICATION);
	wc.hbrBackground=(HBRUSH)(4);
	wc.cbSize       =sizeof(WNDCLASSEX);
	wc.hInstance    =hInstance;
	wc.lpfnWndProc  =WndProc;
	wc.lpszClassName="q";
	wc.hIcon        =LoadIcon(NULL,IDI_APPLICATION);
	if(!RegisterClassEx(&wc)) {
		MessageBox(NULL, "Window Registration Failed!","Error!", MB_ICONEXCLAMATION | MB_OK);
		return 0;
	}
	hwnd=CreateWindowEx(WS_EX_CLIENTEDGE,"q","bb_v1.0.2", WS_VISIBLE|WS_OVERLAPPED|WS_CAPTION|WS_SYSMENU|WS_MINIMIZEBOX,
	                    CW_USEDEFAULT,CW_USEDEFAULT,640,480,NULL,NULL,hInstance,NULL);
	if(hwnd==NULL) {
		MessageBox(NULL,"Window Creation Failed!", "Error!", MB_ICONEXCLAMATION | MB_OK);
		return 0;
	}
	ShowWindow(hwndOut,0);
	ShowWindow(B_send,0);
	ShowWindow(hwndIn,0);
	while(GetMessage(&Msg,NULL,0,0)>0) {
		TranslateMessage(&Msg);
		DispatchMessage(&Msg);
		if(step==1) {
			ShowWindow(hwndOut,1);
			ShowWindow(B_send,1);
			ShowWindow(hwndIn,1);
			ShowWindow(qd_send,0);
			ShowWindow(zc_send,0);
			ShowWindow(hwndts,0);
			ShowWindow(hwndpwd,0);
			ShowWindow(hwndname,0);
			step=2;
		}
	}
	return Msg.wParam;
}
