// Fill out your copyright notice in the Description page of Project Settings.

#include "MySQLConnectionObj.h"
#include "MySQLHelperFunctionLib.h"

bool UMySQLConnectionObj::ConnectMySQL(const FString& MySQLHostIP, const FString& MySQLUser, const FString& MySQLPassword, const FString& MySQLDatabaseName, const bool ShowErrorMsg)
{
	this->MySQLHostIP = MySQLHostIP;
	this->MySQLUser = MySQLUser;
	this->MySQLPassword = MySQLPassword;
	this->MySQLDatabaseName = MySQLDatabaseName;

	bool bIsOK = false;
	FString Msg = "";

	con = UMySQLHelperFunctionLib::MySqlDBConnect(con, MySQLHostIP, MySQLUser, MySQLPassword, MySQLDatabaseName);

	if (con)
	{
		Msg = "The database connect Successful! ";
		bIsOK = true;
	}
	else
	{
		Msg = "The database connection failed. Check whether the SQL statement or the database exists or not! ";
		bIsOK = false;
	}

	UMySQLHelperFunctionLib::MySQLExecuteShowMsgOnScreen(Msg, ShowErrorMsg);
	return bIsOK;
}

void UMySQLConnectionObj::DisConnectMySQL()
{
	if (con)
	{
		con->close();
		delete con;
	}	
}

bool UMySQLConnectionObj::GetMySQLConnectionStateIsClosed()
{
	if (con && con->isClosed() )
		return true;
	else
		return false;
}

bool UMySQLConnectionObj::ReConnectMySQL()
{
	if (con && con->reconnect())
		return true;
	else
		return false;
}
