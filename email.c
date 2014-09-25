#include <windows.h>
#include <stdio.h>
#include <WinSock.h>
#include <iostream>
using namespace std;
#pragma  comment(lib, "ws2_32.lib")

//Base64数据结构
struct Base64Date6
{
  unsigned int d4:6;
  unsigned int d3:6;
  unsigned int d2:6;
  unsigned int d1:6;
};

// 函数声明
char ConvertToBase64  (char   c6);
void EncodeBase64     (char   *dbuf, char *buf128, int len);
void SendMail         (char   *email,char *body);
int  OpenSocket       (struct sockaddr *addr);

int main()
{
  char EmailTo[]         = "xinyong1536@126.com";
  char EmailContents[]   = "From: \"Tacey\"<xinyong1536@126.com>\r\n"
                           "To: \"xxx\"<285289578@qq.com>\r\n"
                           "Subject: HelloTest\r\n\r\n"
                           "Hello World, Hello Email!\nThis is Tacey.";
  SendMail(EmailTo, EmailContents);
  return 0;
}


//转换为Base64编码（参考Base64编码文档）
char ConvertToBase64(char uc)
{
  if(uc < 26)
  {
    return 'A' + uc;
  }
  if(uc < 52)
  {
    return 'a' + (uc - 26);
  }
  if(uc < 62)
  {
    return '0' + (uc - 52);
  }
  if(uc == 62)
  {
    return '+';
  }
  return '/';
}

// base64??ʵ??
void EncodeBase64(char *dbuf, char *buf128, int len)
{
  struct Base64Date6 *ddd      = NULL;
  int           i        = 0;
  char          buf[256] = {0};
  char          *tmp     = NULL;
  char          cc       = '\0';

  memset(buf, 0, 256);
  strcpy_s(buf, 256, buf128);
  for(i = 1; i <= len/3; i++)
  {
    tmp             = buf + (i-1)*3;
    cc              = tmp[2];
    tmp[2]          = tmp[0];
    tmp[0]          = cc;
    ddd             = (struct Base64Date6 *)tmp;
    dbuf[(i-1)*4+0] = ConvertToBase64((unsigned int)ddd->d1);
    dbuf[(i-1)*4+1] = ConvertToBase64((unsigned int)ddd->d2);
    dbuf[(i-1)*4+2] = ConvertToBase64((unsigned int)ddd->d3);
    dbuf[(i-1)*4+3] = ConvertToBase64((unsigned int)ddd->d4);
  }
  if(len % 3 == 1)
  {
    tmp             = buf + (i-1)*3;
    cc              = tmp[2];
    tmp[2]          = tmp[0];
    tmp[0]          = cc;
    ddd             = (struct Base64Date6 *)tmp;
    dbuf[(i-1)*4+0] = ConvertToBase64((unsigned int)ddd->d1);
    dbuf[(i-1)*4+1] = ConvertToBase64((unsigned int)ddd->d2);
    dbuf[(i-1)*4+2] = '=';
    dbuf[(i-1)*4+3] = '=';
  }
  if(len%3 == 2)
  {
    tmp             = buf+(i-1)*3;
    cc              = tmp[2];
    tmp[2]          = tmp[0];
    tmp[0]          = cc;
    ddd             = (struct Base64Date6 *)tmp;
    dbuf[(i-1)*4+0] = ConvertToBase64((unsigned int)ddd->d1);
    dbuf[(i-1)*4+1] = ConvertToBase64((unsigned int)ddd->d2);
    dbuf[(i-1)*4+2] = ConvertToBase64((unsigned int)ddd->d3);
    dbuf[(i-1)*4+3] = '=';
  }
  return;
}
// 发送邮件
void SendMail(char *email, char *body)
{
  int     sockfd     = {0};
  char    buf[1500]  = {0};
  char    rbuf[1500] = {0};
  char    login[128] = {0};
  char    pass[128]  = {0};
  WSADATA WSAData;
  struct sockaddr_in their_addr = {0};
  WSAStartup(MAKEWORD(2, 2), &WSAData);
  memset(&their_addr, 0, sizeof(their_addr));
  
  their_addr.sin_family = AF_INET;
  their_addr.sin_port   = htons(25);
  hostent* hptr         = gethostbyname("smtp.126.com"); 
  memcpy(&their_addr.sin_addr.S_un.S_addr, hptr->h_addr_list[0], hptr->h_length); 
  printf("IP of smpt.126.com is : %d:%d:%d:%d\n", 
          their_addr.sin_addr.S_un.S_un_b.s_b1, 
          their_addr.sin_addr.S_un.S_un_b.s_b2, 
          their_addr.sin_addr.S_un.S_un_b.s_b3, 
          their_addr.sin_addr.S_un.S_un_b.s_b4); 

  // ?????ʼ????????????????Ӻ?û????Ӧ????2 ????????????
  sockfd = OpenSocket((struct sockaddr *)&their_addr);
  memset(rbuf, 0, 1500);
  while(recv(sockfd, rbuf, 1500, 0) == 0)
  {
    cout<<"reconnect..."<<endl;
    Sleep(2);
    sockfd = OpenSocket((struct sockaddr *)&their_addr);
    memset(rbuf, 0, 1500);
  }

  cout<<rbuf<<endl;

  // EHLO
  memset(buf, 0, 1500);
  sprintf_s(buf, 1500, "EHLO HYL-PC\r\n");
  send(sockfd, buf, strlen(buf), 0);
  memset(rbuf, 0, 1500);
  recv(sockfd, rbuf, 1500, 0);
  cout<<"EHLO REceive: "<<rbuf<<endl;

  // AUTH LOGIN
  memset(buf, 0, 1500);
  sprintf_s(buf, 1500, "AUTH LOGIN\r\n");
  send(sockfd, buf, strlen(buf), 0);
  memset(rbuf, 0, 1500);
  recv(sockfd, rbuf, 1500, 0);
  cout<<"Auth Login Receive: "<<rbuf<<endl;

  // USER
  memset(buf, 0, 1500);
  sprintf_s(buf, 1500, "hanyinlong@126.com");//?????????˺?
  memset(login, 0, 128);
  EncodeBase64(login, buf, strlen(buf));
  sprintf_s(buf, 1500, "%s\r\n", login);
  send(sockfd, buf, strlen(buf), 0);
  cout<<"Base64 UserName: "<<buf<<endl;
  memset(rbuf, 0, 1500);
  recv(sockfd, rbuf, 1500, 0);
  cout<<"User Login Receive: "<<rbuf<<endl;

  // PASSWORD
  sprintf_s(buf, 1500, "XXXXXXXXXXXX");//????????????
  memset(pass, 0, 128);
  EncodeBase64(pass, buf, strlen(buf));
  sprintf_s(buf, 1500, "%s\r\n", pass);
  send(sockfd, buf, strlen(buf), 0);
  cout<<"Base64 Password: "<<buf<<endl;

  memset(rbuf, 0, 1500);
  recv(sockfd, rbuf, 1500, 0);
  cout<<"Send Password Receive: "<<rbuf<<endl;

  // MAIL FROM
  memset(buf, 0, 1500);
  sprintf_s(buf, 1500, "MAIL FROM: <hanyinlong@126.com>\r\n");
  send(sockfd, buf, strlen(buf), 0);
  memset(rbuf, 0, 1500);
  recv(sockfd, rbuf, 1500, 0);
  cout<<"set Mail From Receive: "<<rbuf<<endl;

  // RCPT TO ??һ???ռ???
  sprintf_s(buf, 1500, "RCPT TO:<%s>\r\n", email);
  send(sockfd, buf, strlen(buf), 0);
  memset(rbuf, 0, 1500);
  recv(sockfd, rbuf, 1500, 0);
  cout<<"Tell Sendto Receive: "<<rbuf<<endl;

  // DATA ׼????ʼ?????ʼ?????
  sprintf_s(buf, 1500, "DATA\r\n");
  send(sockfd, buf, strlen(buf), 0);
  memset(rbuf, 0, 1500);
  recv(sockfd, rbuf, 1500, 0);
  cout<<"Send Mail Prepare Receive: "<<rbuf<<endl;

  // ?????ʼ????ݣ?\r\n.\r\n???ݽ???????
  sprintf_s(buf, 1500, "%s\r\n.\r\n", body);
  send(sockfd, buf, strlen(buf), 0);
  memset(rbuf, 0, 1500);
  recv(sockfd, rbuf, 1500, 0);
  cout<<"Send Mail Receive: "<<rbuf<<endl;

  // QUIT
  sprintf_s(buf, 1500, "QUIT\r\n");
  send(sockfd, buf, strlen(buf), 0);
  memset(rbuf, 0, 1500);
  recv(sockfd, rbuf, 1500, 0);
  cout<<"Quit Receive: "<<rbuf<<endl;

  //????????
  closesocket(sockfd);
  WSACleanup();
  return;
}
// 打开Socket
int OpenSocket(struct sockaddr *addr)
{
  int sockfd = 0;
  sockfd=socket(PF_INET, SOCK_STREAM, 0);
  if(sockfd < 0)
  {
    cout<<"Open sockfd(TCP) error!"<<endl;
    exit(-1);
  }
  if(connect(sockfd, addr, sizeof(struct sockaddr)) < 0)
  {
    cout<<"Connect sockfd(TCP) error!"<<endl;
    exit(-1);
  }
  return sockfd;
} 