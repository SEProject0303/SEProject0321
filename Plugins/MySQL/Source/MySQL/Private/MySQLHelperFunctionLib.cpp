// Fill out your copyright notice in the Description page of Project Settings.

#include "MySQLHelperFunctionLib.h"
#include "cppconn/driver.h"
#include "cppconn/connection.h"
#include "cppconn/resultset.h"
#include "cppconn/statement.h"
#include "Engine/Engine.h"
#include "UnrealString.h"
#include "NoExportTypes.h"
#include "Platform.h"
#include "cppconn/exception.h"
#include "cppconn/prepared_statement.h"
#include "LogMacros.h"
#include "StringConv.h"
#include "MySQLConnectionObj.h"
#include "../Public/MySQLHelperFunctionLib.h"
#include "UObject/UnrealType.h"


#if UE_BUILD_DEVELOPMENT
PRAGMA_DISABLE_OPTIMIZATION
#endif // UE_BUILD_DEVELOPMENT

//Blueprint test database connection.
bool UMySQLHelperFunctionLib::MySqlDBConnectTest(const FString& StringIP, const FString& User, const FString& Password, const FString& DatabaseName, const bool ShowErrorMsg)
{
	FString Msg = "";
	sql::Connection *con = nullptr;

	//Connect to the database
	con = MySqlDBConnect(con, StringIP, User, Password, DatabaseName);

	if (!con)
	{	 
		Msg = "The database connection failed.Check whether the SQL statement or the database : [" + DatabaseName + "] exists or not!";
		return false;
	}

	con->close();
	delete con;
	Msg = "Successful connect database : [" + DatabaseName + "] !";

	MySQLExecuteShowMsgOnScreen(Msg, ShowErrorMsg);
	return true;
}

sql::Connection * UMySQLHelperFunctionLib::MySqlDBConnect(sql::Connection *con , const FString& StringIP, const FString& User, const FString& Password, const FString& DatabaseName)
{
	std::string MyStringIP(TCHAR_TO_UTF8(*StringIP));
	std::string MyUser(TCHAR_TO_UTF8(*User));
	std::string MyPassword(TCHAR_TO_UTF8(*Password));
	std::string MyDatabaseName(TCHAR_TO_UTF8(*DatabaseName));
	try
	{
		sql::Driver *driver = nullptr;
		driver = get_driver_instance(); 
		con = driver->connect("tcp://" + MyStringIP + ":3306", MyUser, MyPassword);
		if (MyDatabaseName.length() != 0)
		{
			con->setSchema(MyDatabaseName);
		}
		if (con)
			return con;
		else
			return nullptr;
	}
	catch (sql::SQLException exception)
	{
		UE_LOG(LogCore, Error, TEXT("The database connection failed. Check whether the SQL statement or the database exists or not! "));
		return nullptr;
	}
}

bool UMySQLHelperFunctionLib::MySqlDBInsertSingleData(UMySQLConnectionObj* ConnectionObj, const FString& TableName, const FString& Type, const FString& Value, const bool ShowErrorMsg)
{
	std::string SqlTableName(TCHAR_TO_UTF8(*TableName));
	std::string SqlType(TCHAR_TO_UTF8(*Type));
	std::string SqlValue(TCHAR_TO_UTF8(*Value));
	std::string SqlStrExecut = "INSERT INTO " + SqlTableName + "( " + SqlType + ")" + "VALUES(" + SqlValue + ")";

	bool bIsOK = false;
	FString Msg = "";
	try
	{
		sql::Connection *con = nullptr;
		sql::Statement* stmt;

		if (!ConnectionObj->GetMySQLConnectionStateIsClosed())
		{
			if (!ConnectionObj->ReConnectMySQL())
			{
				Msg = "The database connection failed. Please check whether the IP, user and password are correct!";
				MySQLExecuteShowMsgOnScreen(Msg, ShowErrorMsg);
				return false;
			}
		}
		con = ConnectionObj->con;
	
		stmt = con->createStatement();

		stmt->execute(SqlStrExecut);

		delete stmt;

		Msg = "Successful insertion of data!";

		bIsOK = true;
	}
	catch (sql::SQLException exception)
	{
		Msg = "Insert failed, check whether the SQL statement is Incorrect or database data has already exists!";
	}

	MySQLExecuteShowMsgOnScreen(Msg, ShowErrorMsg);

	return bIsOK;
}

bool UMySQLHelperFunctionLib::MySqlDBDeleteSingleData(UMySQLConnectionObj* ConnectionObj, /*const FString& StringIP, const FString& User, const FString& Password, const FString& DatabaseName,*/ const FString& TableName, const FString& DeleteCondition, /*const TMap<FString, FString>& DeleteCondition,*/ const bool ShowErrorMsg)
{
	std::string SqlTableName(TCHAR_TO_UTF8(*TableName));
	std::string SqlDeleteCondition(TCHAR_TO_UTF8(*DeleteCondition));
	std::string SqlStrExecut = "DELETE FROM " + SqlTableName + " WHERE " + SqlDeleteCondition;
	bool bIsOK = false;
	FString Msg = "";
	try
	{
		sql::Connection *con = nullptr;
		sql::Statement* stmt;

		if (!ConnectionObj->GetMySQLConnectionStateIsClosed())
		{
			if (!ConnectionObj->ReConnectMySQL())
			{
				Msg = "The database connection failed. Please check whether the IP, user and password are correct!";
				MySQLExecuteShowMsgOnScreen(Msg, ShowErrorMsg);
				return false;
			}
		}
		con = ConnectionObj->con;

		stmt = con->createStatement();

		int deleteLine = stmt->executeUpdate(SqlStrExecut);

		delete stmt;

		if (deleteLine != 0)
		{
			Msg = "Delete data successfully!";
			bIsOK = true;
		}
		else
		{
			Msg = "Delete successfully, but 0 rows of data are Deleted!";
			bIsOK = false;
		}
	}
	catch (sql::SQLException exception)
	{
		Msg = "Delete failed, check SQL statement is Incorrect or database whether the data exists!";
	}

	MySQLExecuteShowMsgOnScreen(Msg, ShowErrorMsg);
	return bIsOK;
}

bool UMySQLHelperFunctionLib::MySqlDBUpdateSingleData(UMySQLConnectionObj* ConnectionObj,/*const FString& StringIP, const FString& User, const FString& Password, const FString& DatabaseName,*/ const FString& TableName, const FString& DataSet, /*const TMap<FString, FString>& DataSet,*/ const FString& DataSetCondition, /*const TMap<FString, FString>& DataSetCondition,*/ const bool ShowErrorMsg)
{
	std::string SqlTableName(TCHAR_TO_UTF8(*TableName));
	std::string SqlDataSet(TCHAR_TO_UTF8(*DataSet));
	std::string SqlDataSetCondition(TCHAR_TO_UTF8(*DataSetCondition));
	std::string SqlStrExecut = "UPDATE " + SqlTableName + " SET " + SqlDataSet + " WHERE " + SqlDataSetCondition;
	bool bIsOK = false;
	FString Msg = "";
	try
	{
		sql::Connection *con = nullptr;
		sql::Statement* stmt;

		if (!ConnectionObj->GetMySQLConnectionStateIsClosed())
		{
			//			Msg = "The database connection failed. Please check whether the IP, user and password are correct!";
			if (!ConnectionObj->ReConnectMySQL())
			{
				Msg = "The database connection failed. Please check whether the IP, user and password are correct!";
				MySQLExecuteShowMsgOnScreen(Msg, ShowErrorMsg);
				return false;
			}
		}
		con = ConnectionObj->con;
	
		stmt = con->createStatement();

		int upadataLine = stmt->executeUpdate(SqlStrExecut);

		delete stmt;

		if (upadataLine != 0)
		{
			Msg = "Update data successfully!";
			bIsOK = true;
		}
		else
		{
			Msg = "Update successfully, but 0 rows of data are updated!";
			bIsOK = false;
		}
	}
	catch (sql::SQLException exception)
	{
		Msg = "Update failed, check SQL statement is Incorrect or whether the data is not exists!";
	}

	MySQLExecuteShowMsgOnScreen(Msg, ShowErrorMsg);
	return bIsOK;
}

bool UMySQLHelperFunctionLib::MySqlDBQueryData(UMySQLConnectionObj* ConnectionObj, /*const FString& StringIP, const FString& User, const FString& Password, const FString& DatabaseName,*/ const FString& TableName, TArray<FString>& StrRst, const FString& DataSearchColumnName, /*const TArray<FString>& DataSearchColumnName,*/ /*const TMap<FString, FString>& DataSearchCondition,*/const FString& DataSearchCondition, const bool ShowErrorMsg)
{
	std::string SqlTableName(TCHAR_TO_UTF8(*TableName));
	std::string SqlDataSearchColumnName(TCHAR_TO_UTF8(*DataSearchColumnName));
	std::string SqlDataSearchCondition(TCHAR_TO_UTF8(*DataSearchCondition));
	std::string SqlStrExecut = "SELECT ";
	bool bIsOK = false;
	bool SIZENOTNULL = false;
	FString Msg = "";
	try
	{
		sql::Connection *con = nullptr;
		sql::Statement* stmt;
		sql::ResultSet *res = nullptr;

		if (ConnectionObj->GetMySQLConnectionStateIsClosed())
		{
			if (!ConnectionObj->ReConnectMySQL())
			{
				Msg = "The database connection failed. Please check whether the IP, user and password are correct!";
				MySQLExecuteShowMsgOnScreen(Msg, ShowErrorMsg);
				return false;
			}
		}

		con = ConnectionObj->con;

		if (SqlDataSearchColumnName.length() == 0)
		{
			SqlStrExecut = SqlStrExecut + " * " + " FROM " + SqlTableName;
			if (SqlDataSearchCondition.length() != 0)
			{
				SqlStrExecut = SqlStrExecut + " WHERE " + SqlDataSearchCondition;
			}
		}
		else
		{
			SqlStrExecut = SqlStrExecut + SqlDataSearchColumnName + " FROM " + SqlTableName;
			if (SqlDataSearchCondition.length() != 0)
			{
				SqlStrExecut = SqlStrExecut + " WHERE " + SqlDataSearchCondition;
			}
		}

		stmt = con->createStatement();
		res = stmt->executeQuery(SqlStrExecut); 
		int RowSize = res->getMetaData()->getColumnCount();

		while (res->next())
		{
			FString Str = "";
			SIZENOTNULL = true;
			for (int i = 1; i <= RowSize; i++)
			{
				Str = Str + UTF8_TO_TCHAR(res->getString(i).c_str()) + "/@";
			}
			StrRst.Add(Str);
		}

		delete res;
		delete stmt;

		if (!SIZENOTNULL)
		{
			Msg = "Searched successfully, but 0 rows of data are searched!";
			bIsOK = false;
		}
		else
		{
			Msg = "Searched data successfully!";
			bIsOK = true;
		}
	}
	catch (sql::SQLException exception)
	{
		Msg = "Searched failed, check SQL statement!";
	}

	MySQLExecuteShowMsgOnScreen(Msg, ShowErrorMsg);
	return bIsOK;
}

bool UMySQLHelperFunctionLib::MySqlDBExecute(const FString& StringIP, const FString& User, const FString& Password, const FString& DatabaseName, const FString& SqlStr, const bool ShowErrorMsg)
{
	std::string MySqlStr(TCHAR_TO_UTF8(*SqlStr));
	bool bIsOK = false;
	FString Msg = "";
	try
	{
		sql::Connection *con = nullptr;
		sql::Statement* stmt = nullptr;

		//Connect to the database
		con = MySqlDBConnect(con, StringIP, User, Password, DatabaseName);

		if (!con)
		{
			Msg = "The database connection failed. Please check whether the IP, user and password are correct!";
		}
		stmt = (con->createStatement());

		bIsOK = stmt->execute(MySqlStr);

		delete stmt;
		delete con;
		Msg = "Successful Execution SQL [" + SqlStr + "]!";
		bIsOK = true;
	}
	catch (sql::SQLException exception)
	{
		Msg = "Execution failed, please check SQL [" + SqlStr + "] statement!";
	}

	MySQLExecuteShowMsgOnScreen(Msg, ShowErrorMsg);
	return bIsOK;
}

bool UMySQLHelperFunctionLib::MySqlCreateDB(const FString& StringIP, const FString& User, const FString& Password, const FString& DatabaseNameCreate, const bool ShowErrorMsg)
{
	std::string MyDatabaseName(TCHAR_TO_UTF8(*DatabaseNameCreate));
	bool bIsOK = false;
	FString Msg = "";
	try
	{
		sql::Connection *con = nullptr;

		sql::PreparedStatement* Pstmt = nullptr;
		std::string sqlStr = "CREATE DATABASE ";

		//Connect to the database
		con = MySqlDBConnect(con, StringIP, User, Password, "");

		if (!con)
		{
			Msg = "The database connection failed. Please check whether the IP, user and password are correct!";
		}
		Pstmt = con->prepareStatement(sqlStr + MyDatabaseName);

		Pstmt->executeUpdate();

		delete Pstmt;
		delete con;
		Msg = "Successful database [" + DatabaseNameCreate + "]creation!";
		bIsOK = true;
	}
	catch (sql::SQLException exception)
	{
		Msg = "Execution failed, please check that SQL statement or database [" + DatabaseNameCreate +  "]already exists!";
	}

	MySQLExecuteShowMsgOnScreen(Msg, ShowErrorMsg);
	return bIsOK;
}

bool UMySQLHelperFunctionLib::MySqlCreateDBTable(UMySQLConnectionObj* ConnectionObj,/*const FString& StringIP, const FString& User, const FString& Password, const FString& DatabaseName,*/ const FString& TableNameCreate, const TMap<FString, FString>& TableInfos, const bool ShowErrorMsg)
{
	std::string MyTableNameCreate(TCHAR_TO_UTF8(*TableNameCreate));
	bool bIsOK = false;
	FString Msg = "";
	try
	{
		sql::Connection *con = nullptr;

		sql::PreparedStatement* Pstmt = nullptr;
		std::string sqlStr = "CREATE TABLE " + MyTableNameCreate + "( ";

		if (!ConnectionObj->GetMySQLConnectionStateIsClosed())
		{
			//			Msg = "The database connection failed. Please check whether the IP, user and password are correct!";
			if (!ConnectionObj->ReConnectMySQL())
			{
				Msg = "The database connection failed. Please check whether the IP, user and password are correct!";
				MySQLExecuteShowMsgOnScreen(Msg, ShowErrorMsg);
				return false;
			}
		}
		con = ConnectionObj->con;
		int32 Flag = 0;

		//TMap ergodic
		for (auto iter = TableInfos.CreateConstIterator(); iter; ++iter)
		{
			//Determine whether TMap traverses to the last element.
			if (!(Flag == TableInfos.Num() - 1))
			{
				std::string NameRow(TCHAR_TO_UTF8(*(iter->Key)));
				std::string NameRowType(TCHAR_TO_UTF8(*(iter->Value)));
				sqlStr = sqlStr + NameRow + " " + NameRowType + ",";
			}
			else
			{
				std::string NameRow(TCHAR_TO_UTF8(*(iter->Key)));
				std::string NameRowType(TCHAR_TO_UTF8(*(iter->Value)));
				sqlStr = sqlStr + NameRow + " " + NameRowType + " )";
			}

			++Flag;
		}

		//Execute the SQL statement.
		Pstmt = con->prepareStatement(sqlStr);

		Pstmt->executeUpdate();

		delete Pstmt;
		//delete con;
		Msg = "Successful database table [" + TableNameCreate + "] creation!";
		bIsOK = true;
	}
	catch (sql::SQLException exception)
	{
		Msg = "Execution failed, please check that SQL statement or database table [" + TableNameCreate + "] already exists!";
	}

	MySQLExecuteShowMsgOnScreen(Msg, ShowErrorMsg);
	return bIsOK;
}

bool UMySQLHelperFunctionLib::MySqlDropDB(UMySQLConnectionObj* ConnectionObj,/*const FString& StringIP, const FString& User, const FString& Password,*/ const FString& DatabaseNameDrop, const bool ShowErrorMsg)
{
	std::string MyDropDatabaseName(TCHAR_TO_UTF8(*DatabaseNameDrop));
	bool bIsOK = false;
	FString Msg = "";
	try
	{
		sql::Connection *con = nullptr;

		sql::PreparedStatement* Pstmt = nullptr;
		std::string sqlStr = "Drop DATABASE ";

		if (!ConnectionObj->GetMySQLConnectionStateIsClosed())
		{
			//			Msg = "The database connection failed. Please check whether the IP, user and password are correct!";
			if (!ConnectionObj->ReConnectMySQL())
			{
				Msg = "The database connection failed. Please check whether the IP, user and password are correct!";
				MySQLExecuteShowMsgOnScreen(Msg, ShowErrorMsg);
				return false;
			}
		}
		con = ConnectionObj->con;
		Pstmt = con->prepareStatement(sqlStr + MyDropDatabaseName);

		Pstmt->executeUpdate();

		delete Pstmt;

		Msg = "Successful Drop database [" + DatabaseNameDrop + "]!";

		bIsOK = true;
	}
	catch (sql::SQLException exception)
	{
		Msg = "Execution failed, please check that SQL statement or database [" + DatabaseNameDrop + "]is not exists!";
	}

	MySQLExecuteShowMsgOnScreen(Msg, ShowErrorMsg);
	return bIsOK;
}

bool UMySQLHelperFunctionLib::MySqlDropDBTable(UMySQLConnectionObj* ConnectionObj,/*const FString& StringIP, const FString& User, const FString& Password, const FString& DatabaseName,*/ const FString& TableNameDrop, const bool ShowErrorMsg)
{
	std::string MyTableNameDrop(TCHAR_TO_UTF8(*TableNameDrop));
	bool bIsOK = false;
	FString Msg = "";
	try
	{
		sql::Connection *con = nullptr;

		sql::PreparedStatement* Pstmt = nullptr;
		std::string sqlStr = "Drop TABLE ";

		if (!ConnectionObj->GetMySQLConnectionStateIsClosed())
		{
			//			Msg = "The database connection failed. Please check whether the IP, user and password are correct!";
			if (!ConnectionObj->ReConnectMySQL())
			{
				Msg = "The database connection failed. Please check whether the IP, user and password are correct!";
				MySQLExecuteShowMsgOnScreen(Msg, ShowErrorMsg);
				return false;
			}
		}
		con = ConnectionObj->con;
		Pstmt = con->prepareStatement(sqlStr + MyTableNameDrop);

		Pstmt->executeUpdate();

		delete Pstmt;

		Msg = "Successful Drop database [" + TableNameDrop + "]!";

		bIsOK = true;
	}
	catch (sql::SQLException exception)
	{
		Msg = "Execution failed, please check that SQL statement or table [" + TableNameDrop + "] in database[" + ConnectionObj->MySQLDatabaseName + "] is not exists!";
	}

	MySQLExecuteShowMsgOnScreen(Msg, ShowErrorMsg);
	return bIsOK;
}

void UMySQLHelperFunctionLib::MySQLExecuteShowMsgOnScreen(const FString& Msg, const bool& IsShowOnScreen)
{
	if (IsShowOnScreen)
	{
		if (GEngine)
			GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Red, Msg);
	}

	UE_LOG(LogCore, Error, TEXT("%s"), *Msg);

	return ;
}

bool UMySQLHelperFunctionLib::FillBPStructResultData(FString& StrRst, UScriptStruct * StructClass, void * StructDataAddr)
{
	TArray<FString> StrRstArray;

	const FString Delimiter = "/@";

	StrRst.ParseIntoArray(StrRstArray, *Delimiter, true);

	int i = 0;

	for (TFieldIterator<UProperty> PropIt(StructClass); PropIt; ++PropIt)
	{
		const UProperty* FieldProp = *PropIt;
		void* FieldDataAddr = FieldProp->ContainerPtrToValuePtr<void>(StructDataAddr);
		if (!SetBPStructFieldValue(StrRstArray[i], FieldProp, FieldDataAddr))
		{
			return false;
		}
		i++;
	}

	return true;
}

bool UMySQLHelperFunctionLib::SetBPStructFieldValue(FString& StrRst, const UProperty * FieldProp, void * FieldDataAddr, int32 ArrayIndex)
{
	if (StrRst.IsEmpty() || FieldProp == nullptr || FieldDataAddr == nullptr)
		return false;

	//拿到结构体某一列变量名
	FString FieldUniqueName = FieldProp->GetName();

	const UIntProperty* IntProp = Cast<UIntProperty>(FieldProp);
	if (IntProp)
	{
		int32 Val = 0;
		Val = FCString::Atoi(*StrRst);
		IntProp->SetPropertyValue(FieldDataAddr, Val);
		return true;
	}

#if BLUEPRINT_SUPPORT_ADVANCED_DATA_TYPE
	const UUInt32Property* UIntProp = Cast<UUInt32Property>(FieldProp);
	if (UIntProp)
	{
		uint32 Val = 0;
		Val = FCString::Atoi(*StrRst);
		UIntProp->SetPropertyValue(FieldDataAddr, Val);
		return true;
	}
	const UInt64Property* Int64Prop = Cast<UInt64Property>(FieldProp);
	if (Int64Prop)
	{
		int64 Val = 0;
		Val = FCString::Atoi(*StrRst);
		Int64Prop->SetPropertyValue(FieldDataAddr, Val);
		return true;
	}
	const UUInt64Property* UInt64Prop = Cast<UUInt64Property>(FieldProp);
	if (UInt64Prop)
	{
		uint64 Val = 0;
		Val = FCString::Atoi(*StrRst);
		UInt64Prop->SetPropertyValue(FieldDataAddr, Val);
		return true;
	}
#endif
	const UFloatProperty* FloatProp = Cast<UFloatProperty>(FieldProp);
	if (FloatProp)
	{
		float Val = 0;
		Val = FCString::Atof(*StrRst);
		FloatProp->SetPropertyValue(FieldDataAddr, Val);
		return true;
	}
#if BLUEPRINT_SUPPORT_ADVANCED_DATA_TYPE
	const UDoubleProperty* DoubleProp = Cast<UDoubleProperty>(FieldProp);
	if (DoubleProp)
	{
		double Val = 0;
		Val = FCString::Atod(*StrRst);
		DoubleProp->SetPropertyValue(FieldDataAddr, Val);
		return true;
	}
#endif

	const UStrProperty* StrProp = Cast<UStrProperty>(FieldProp);

	if (StrProp)
	{
		FString& Val = StrRst;
		StrProp->SetPropertyValue(FieldDataAddr, Val);
		return true;
	}

	UE_LOG(LogCore, Error, TEXT("the field type is not support message！"));
	return false;
}

//解析数据库字段数据到结构体
DEFINE_FUNCTION(UMySQLHelperFunctionLib::execUnpackMySQLDataToBPStruct)
{
	P_GET_PROPERTY(UStrProperty, StrRst);
	Stack.StepCompiledIn<UStructProperty>(NULL);
	void* StructDataAddr = Stack.MostRecentPropertyAddress;
	UStructProperty* StructProp = Cast<UStructProperty>(Stack.MostRecentProperty);

	P_FINISH;
	bool bOK = false;
	P_NATIVE_BEGIN;
	bool bSearchOk = false;
	
	if (!StrRst.IsEmpty())
	{
		UScriptStruct* StructClass = StructProp->Struct;

		FString StructName = StructClass->GetName();

		bOK = FillBPStructResultData(StrRst, StructClass, StructDataAddr);
	}
	else
	{
		bOK = false;
		UE_LOG(LogClass, Error, TEXT("resolve data to bpstruct faild!"));
	}

	*(bool*)RESULT_PARAM = bOK;
	P_NATIVE_END;
}

DEFINE_FUNCTION(UMySQLHelperFunctionLib::execUnpackMySQLDataToBPStructArray)
{
}

#if UE_BUILD_DEVELOPMENT
PRAGMA_ENABLE_OPTIMIZATION
#endif // UE_BUILD_DEVELOPMENT