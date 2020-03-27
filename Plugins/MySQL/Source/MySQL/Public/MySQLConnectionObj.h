// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MySQL.h"
#include "cppconn/connection.h"
#include "UObject/NoExportTypes.h"
#include "MySQLConnectionObj.generated.h"

UCLASS(BlueprintType, Blueprintable, config = MySQLConnectionObjs, meta = (ShortTooltip = "MySQLConnectionObj"))
class MYSQL_API UMySQLConnectionObj : public UObject
{
	GENERATED_BODY()
	
public:
	//数据库主机的IP
	FString MySQLHostIP;

	//数据库端口号MySQLHostPort
	FString MySQLHostPort;

	//数据库用户名MySQLUser
	FString MySQLUser;

	//数据库密码MySQLPassword
	FString MySQLPassword;

	//数据库名MySQLDatabaseName
	FString MySQLDatabaseName;

	//数据库连接对象Connection
	sql::Connection *con;

public:
	//连接MySQL数据库.
	UFUNCTION(BlueprintCallable, category = "MySQLHelperFunctionLib|MySQLConnect", meta = (DisplayName = "ConnectMySQL"))
	bool ConnectMySQL(const FString& MySQLHostIP, const FString& MySQLUser, const FString& MySQLPassword, const FString& MySQLDatabaseName, const bool ShowErrorMsg);

	//断开MySQL数据库.
	UFUNCTION(BlueprintCallable, category = "MySQLHelperFunctionLib|MySQLConnect", meta = (DisplayName = "DisConnectMySQL"))
	void DisConnectMySQL();

	//判断MySQL数据库连接是否关闭.
	UFUNCTION(BlueprintCallable, category = "MySQLHelperFunctionLib|MySQLConnect", meta = (DisplayName = "GetMySQLConnectionState"))
	bool GetMySQLConnectionStateIsClosed();

	//重新连接MySQL数据库.
	UFUNCTION(BlueprintCallable, category = "MySQLHelperFunctionLib|MySQLConnect", meta = (DisplayName = "ReConnectMySQL"))
	bool ReConnectMySQL();
};
