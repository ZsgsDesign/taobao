#pragma once

#ifndef __MYSQL_INTERFACE_H__  
#define __MYSQL_INTERFACE_H__  

#include <winsock.h>

#pragma comment(lib, "libmysql.lib")

#define ERROR_QUERY_FAIL -1


struct MySQLConInfo
{
	char* server;
	char* user;
	char* password;
	char* database;
	int port;
};

class MySQLInterface
{
	public:
		MySQLInterface();
		virtual ~MySQLInterface();
		void SetMySQLConInfo(char* server, char* username, char* password, char* database, int port);		// ����������Ϣ
		bool Open();																						// ������
		void Close();																						// �ر�����
		bool Select(const std::string& Querystr, std::vector<std::vector<std::string> >& data);				// ��ȡ����
		bool Query(const std::string& Querystr);															// ��������
		int GetInsertID(const std::string& Querystr);														// ���벢��ȡ�����ID,����Զ�����ID
		void ErrorIntoMySQL();																				// ������Ϣ

	public:
		int ErrorNum;																						// �������  
		const char* ErrorInfo;      																		// ������ʾ  

	private:
		MySQLConInfo MysqlConInfo;																			// ������Ϣ
		MYSQL MysqlInstance;																				// MySQL����
		MYSQL_RES *Result;																					// ���ڴ�Ž�� 
};


#endif  
