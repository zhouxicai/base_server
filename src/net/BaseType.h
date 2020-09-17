/////////////////////////////////////////////////////////////////////////////////
//�ļ����ƣ�BaseType.h
//����������ϵͳ�ײ����ͻ����ļ�������Ȩ��Ա�����޸Ĵ��ļ�����
//�汾˵����Windows����ϵͳ��Ҫ����꣺__WINDOWS__
//			Linux����ϵͳ��Ҫ����꣺__LINUX__
//�޸������
//
/////////////////////////////////////////////////////////////////////////////////
#ifndef __BASETYPE_H__
#define __BASETYPE_H__


#if 0
#ifndef __LINUX__
	#define __LINUX__ 0
#endif
#else
#ifndef __WINDOWS__
	#define __WINDOWS__ 1
#endif
#endif // 0



/////////////////////////////////////////////////////////////////////////////////
//������ϢԤ�����
/////////////////////////////////////////////////////////////////////////////////
#ifndef FD_SETSIZE
#define FD_SETSIZE      1024
#endif /* FD_SETSIZE */


/////////////////////////////////////////////////////////////////////////////////
//��ǰ������ϵͳͷ�ļ�����
/////////////////////////////////////////////////////////////////////////////////
#if defined(__WINDOWS__)
	#pragma warning ( disable : 4786 )
	#pragma warning ( disable : 4996 )
	#include <Windows.h>
	#include "crtdbg.h"
#elif defined(__LINUX__)
	#include <sys/types.h>
	#include <pthread.h>
	#include <execinfo.h>
	#include <signal.h>
	#include <exception>
	#include <setjmp.h>
	#include <sys/epoll.h>
#endif

#include <stdio.h>
#include <iostream>
#include <fstream>
#include <string>
#include <time.h>
#include <math.h>
#include <stdarg.h>
#include "Assertx.h"
using namespace std;

///////////////////////////////////////////////////////////////////////
//��׼�������Ͷ���
///////////////////////////////////////////////////////////////////////
#define VOID			void			//��׼��
typedef unsigned char	UCHAR;			//��׼�޷���CHAR
typedef char			CHAR;			//��׼CHAR
typedef unsigned int	UINT;			//��׼�޷���INT
typedef int				INT;			//��׼INT
typedef unsigned short	USHORT;			//��׼�޷���short
typedef short			SHORT;			//��׼short
typedef unsigned long	ULONG;			//��׼�޷���LONG(���Ƽ�ʹ��)
typedef long			LONG;			//��׼LONG(���Ƽ�ʹ��)
typedef float			FLOAT;			//��׼float

typedef UCHAR			uchar;
typedef USHORT			ushort;
typedef UINT			uint;
typedef ULONG			ulong;
typedef ULONG			IP_t;
typedef USHORT			PacketID_t;
typedef INT				BOOL;
typedef UCHAR			BYTE;




///////////////////////////////////////////////////////////////////////
//�������ݺ궨��
///////////////////////////////////////////////////////////////////////

//IP��ַ���ַ���󳤶�
#define IP_SIZE			24
#define KEY_SIZE			64
#define MAX_WORLDCOUNT			256
#define TIMELENTH			23

//�������ڿ�ִ���ļ��н��Ǻ�����ʾ������ǰҪ�ĳ����KEY��KEY����С��10���ֽ�
#define GAMESERVER_TO_CLIENT_KEY "�������˶Կͻ��˵ķ����Կ"
#define CLIENT_TO_GAMESERVER_KEY "�������˶Կͻ��˵ķ����Կ"
#define LOGIN_TO_CLIENT_KEY      "�������˶Կͻ��˵ķ����Կ"
#define CLIENT_TO_LOGIN_KEY      "�������˶Կͻ��˵ķ����Կ"
#define OSTREAM_KEY              "�ײ�ȫ�ַ����ͨ����Կ"		
//�������ڿ�ִ���ļ��н��Ǻ�����ʾ������ǰҪ�ĳ����KEY��KEY����С��10���ֽ�

#define PACK_COMPART "$-$"//����ָ���
#define PACK_COMPART_SIZE strlen(PACK_COMPART)

#ifndef ENCRYPT
#define ENCRYPT(x,xlen,KEY,BeginPlace)	if( (x)!=NULL ) \
		{ \
		\
			CHAR* t_pBuffer = (x); \
			CHAR* pKey = {KEY}; \
			UINT KeyLen = (UINT)strlen(pKey); \
			for (UINT i = 0; i < (xlen); i++) \
			{ \
				*t_pBuffer ^= pKey[(i+BeginPlace)%KeyLen]; \
				t_pBuffer++; \
			} \
		}
#endif

#ifndef ENCRYPT_HEAD
#define ENCRYPT_HEAD(x,KEY)	if( (x)!=NULL ) \
		{ \
			CHAR* t_pBuffer = (x); \
			CHAR* pKey = {KEY}; \
			UINT KeyLen = (UINT)strlen(pKey); \
			for (UINT i = 0; i < PACKET_HEADER_SIZE; i++) \
			{ \
				*t_pBuffer ^= pKey[i%KeyLen]; \
				t_pBuffer++; \
			} \
		}
#endif

#define OVER_MAX_SERVER 256

//��Ч�ľ��
#define INVALID_HANDLE	-1
//��Ч��IDֵ
#define INVALID_ID		-1
//��
#ifndef TRUE
	#define TRUE 1
#endif
//��
#ifndef FALSE
	#define FALSE 0
#endif
//�ļ�·�����ַ���󳤶�
#ifndef _MAX_PATH
	#define _MAX_PATH 260
#endif

///////////////////////////////////////////////////////////////////////
//����Ԥ����궨��
///////////////////////////////////////////////////////////////////////
#if defined(NDEBUG)
	#define __ENTER_FUNCTION_FOXNET if(1){
	#define __LEAVE_FUNCTION_FOXNET }
#else
	#define __ENTER_FUNCTION_FOXNET if(1){
	#define __LEAVE_FUNCTION_FOXNET }
#endif


#if defined(NDEBUG)
	#define _MY_TRY try
	#define _MY_CATCH catch(...)
#else
	#define _MY_TRY try
	#define _MY_CATCH catch(...)
#endif



#if defined(NDEBUG)
#define Assert(expr) ((VOID)0)
#define AssertEx(expr,msg) ((VOID)0)
#define AssertSpecial(expr,msg) ((VOID)0)
#define MyMessageBox(msg) ((VOID)0)
#elif __LINUX__
#define Assert(expr) {if(!(expr)){__assert__(__FILE__,__LINE__,__PRETTY_FUNCTION__,#expr);}}
#define ProtocolAssert(expr) ((VOID)((expr)?0:(__protocol_assert__(__FILE__,__LINE__,__PRETTY_FUNCTION__,#expr),0)))
#define AssertEx(expr,msg) {if(!(expr)){__assertex__(__FILE__,__LINE__,__PRETTY_FUNCTION__,#expr,msg);}}
#define AssertSpecial(expr,msg) {if(!(expr)){__assertspecial__(__FILE__,__LINE__,__PRETTY_FUNCTION__,#expr,msg);}}
#define AssertExPass(expr,msg) {if(!(expr)){__assertex__(__FILE__,__LINE__,__PRETTY_FUNCTION__,#expr,msg);}}
#define MyMessageBox(msg) ((VOID)0)
#elif __WIN_CONSOLE__ || __WIN32__ || __WINDOWS__
#define Assert(expr) ((VOID)((expr)?0:(__assert__(__FILE__,__LINE__,__FUNCTION__,#expr),0)))
#define AssertEx(expr,msg) ((VOID)((expr)?0:(__assertex__(__FILE__,__LINE__,__FUNCTION__,#expr,msg),0)))
#define AssertSpecial(expr,msg) ((VOID)((expr)?0:(__assertspecial__(__FILE__,__LINE__,__FUNCTION__,#expr,msg),0)))
#define MyMessageBox(msg) __messagebox__(msg)
#elif __MFC__
#define Assert(expr) ASSERT(expr)
#define AssertEx(expr,msg) ((VOID)0)
#define AssertSpecial(expr,msg) ((VOID)0)
#define MyMessageBox(msg) ((VOID)0)
#endif


#if defined(__WINDOWS__)
#if defined(NDEBUG)
#define __ENTER_FUNCTION {try{
#define __LEAVE_FUNCTION }catch(...){AssertSpecial(FALSE,__FUNCTION__);}}
#else
#define __ENTER_FUNCTION {try{
#define __LEAVE_FUNCTION }catch(...){AssertSpecial(FALSE,__FUNCTION__);}}
#endif
#else	//linux
#define __ENTER_FUNCTION {try{
#define __LEAVE_FUNCTION }catch(...){AssertSpecial(FALSE,__PRETTY_FUNCTION__);}}
#endif 


//����ָ��ֵɾ���ڴ�
#ifndef SAFE_DELETE
#define SAFE_DELETE(x)	if( (x)!=NULL ) { delete (x); (x)=NULL; }
#endif
//����ָ��ֵɾ�����������ڴ�
#ifndef SAFE_DELETE_ARRAY
#define SAFE_DELETE_ARRAY(x)	if( (x)!=NULL ) { delete[] (x); (x)=NULL; }
#endif
//����ָ�����free�ӿ�
#ifndef SAFE_FREE
#define SAFE_FREE(x)	if( (x)!=NULL ) { free(x); (x)=NULL; }
#endif
//����ָ�����Release�ӿ�
#ifndef SAFE_RELEASE
#define SAFE_RELEASE(x)	if( (x)!=NULL ) { (x)->Release(); (x)=NULL; }
#endif



#endif
