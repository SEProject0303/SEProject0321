#pragma once

#include "CoreMinimal.h"
#include "MySQL.h"
#include "UnrealString.h"
#include "cppconn/connection.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "MySQLHelperFunctionLib.generated.h"

#define BLUEPRINT_SUPPORT_ADVANCED_DATA_TYPE 1

//All structure can use
USTRUCT(BlueprintInternalUseOnly)
struct FCustomStructValue
{
	GENERATED_USTRUCT_BODY()
	FCustomStructValue() { }
};

using namespace sql;
/**
 * FunctionLib
 */
class MySQLConnectionObj;

UCLASS()
class MYSQL_API UMySQLHelperFunctionLib : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, category = "MySQLHelperFunctionLib", meta = (ToolTip = "DBConnectTest", DisplayName = "DBConnectTest"))
	static bool MySqlDBConnectTest(const FString& StringIP, const FString& User, const FString& Password, const FString& DatabaseName, const bool ShowErrorMsg);

	static sql::Connection * MySqlDBConnect(sql::Connection* con, const FString& StringIP, const FString& User, const FString& Password, const FString& DatabaseName = "");

	UFUNCTION(BlueprintCallable, category = "MySQLHelperFunctionLib", meta = (ToolTip = "DBInsertSingleData", DisplayName = "DBInsertSingleData"))
	static bool MySqlDBInsertSingleData(UMySQLConnectionObj* ConnectionObj, const FString& TableName, const FString& Type, const FString& Value, const bool ShowErrorMsg);

	UFUNCTION(BlueprintCallable, category = "MySQLHelperFunctionLib", meta = (ToolTip = "DBDeleteSingleData", DisplayName = "DBDeleteSingleData"))
	static bool MySqlDBDeleteSingleData(UMySQLConnectionObj* ConnectionObj, /*const FString& StringIP, const FString& User, const FString& Password, const FString& DatabaseName,*/ const FString& TableName, const FString& DeleteCondition, /*const TMap<FString, FString>& DeleteCondition,*/ const bool ShowErrorMsg);

	UFUNCTION(BlueprintCallable, category = "MySQLHelperFunctionLib", meta = (ToolTip = "DBUpdateSingleData", DisplayName = "DBUpdateSingleData"))
	static bool MySqlDBUpdateSingleData(UMySQLConnectionObj* ConnectionObj,/*const FString& StringIP, const FString& User, const FString& Password, const FString& DatabaseName,*/ const FString& TableName, const FString& DataSet, /*const TMap<FString, FString>& DataSet,*/ const FString& DataSetCondition, /*const TMap<FString, FString>& DataSetCondition,*/ const bool ShowErrorMsg);

	UFUNCTION(BlueprintCallable, category = "MySQLHelperFunctionLib", meta = (ToolTip = "DBQueryData", DisplayName = "DBQueryData"))
	static bool MySqlDBQueryData(UMySQLConnectionObj* ConnectionObj, /*const FString& StringIP, const FString& User, const FString& Password, const FString& DatabaseName,*/ const FString& TableName, TArray<FString>& StrRst, const FString& DataSearchColumnName, /*const TArray<FString>& DataSearchColumnName,*/ /*const TMap<FString, FString>& DataSearchCondition,*/const FString& DataSearchCondition, const bool ShowErrorMsg);

	UFUNCTION(BlueprintCallable, category = "MySQLHelperFunctionLib", meta = (ToolTip = "DBExecute", DisplayName = "DBExecute"))
	static bool MySqlDBExecute(const FString& StringIP, const FString& User, const FString& Password, const FString& DatabaseName, const FString& SqlStr, const bool ShowErrorMsg);

	UFUNCTION(BlueprintCallable, category = "MySQLHelperFunctionLib", meta = (ToolTip = "CreateDB", DisplayName = "CreateDatabase"))
	static bool MySqlCreateDB(const FString& StringIP, const FString& User, const FString& Password, const FString& DatabaseNameCreate, const bool ShowErrorMsg);

	UFUNCTION(BlueprintCallable, category = "MySQLHelperFunctionLib", meta = (ToolTip = "CreateDBTable", DisplayName = "CreateDBTable"))
	static bool MySqlCreateDBTable(UMySQLConnectionObj* ConnectionObj,/*const FString& StringIP, const FString& User, const FString& Password, const FString& DatabaseName,*/ const FString& TableNameCreate, const TMap<FString,FString>& TableInfos, const bool ShowErrorMsg);

	UFUNCTION(BlueprintCallable, category = "MySQLHelperFunctionLib", meta = (ToolTip = "DropDB", DisplayName = "DropDB"))
	static bool MySqlDropDB(UMySQLConnectionObj* ConnectionObj,/*const FString& StringIP, const FString& User, const FString& Password,*/ const FString& DatabaseNameDrop, const bool ShowErrorMsg);

	UFUNCTION(BlueprintCallable, category = "MySQLHelperFunctionLib", meta = (ToolTip = "DropDBTable", DisplayName = "DropDBTable"))
	static bool MySqlDropDBTable(UMySQLConnectionObj* ConnectionObj,/*const FString& StringIP, const FString& User, const FString& Password, const FString& DatabaseName,*/ const FString& TableNameDrop, const bool ShowErrorMsg);

	static void MySQLExecuteShowMsgOnScreen(const FString& Msg, const bool& IsShowOnScreen);

	//动态解析MySQL数据集到蓝图结构体.
	UFUNCTION(BlueprintCallable, CustomThunk, category = "MySQLHelperFunctionLib", meta = (CustomStructureParam = "Value", AutoCreateRefTerm = "Value", DisplayName = "Unpack MySQLData To BPStruct"))
	static bool UnpackMySQLDataToBPStruct(const FString StrRst, FCustomStructValue& Value);
	DECLARE_FUNCTION(execUnpackMySQLDataToBPStruct);

	static bool FillBPStructResultData(FString& StrRst, UScriptStruct* StructClass, void* StructDataAddr);

	//使用查询结果集填充蓝图结构体.
	static bool SetBPStructFieldValue(FString& StrRst, const UProperty* FieldProp, void* FieldDataAddr, int32 ArrayIndex = 0);

	//动态解析MySQL数据集到蓝图结构体数组.
	UFUNCTION(BlueprintCallable, CustomThunk, category = "MySQLHelperFunctionLib", meta = (CustomStructureParam = "ValueArray", AutoCreateRefTerm = "ValueArray", DisplayName = "Unpack MySQLData To BPStructArray", ArrayParm = "ValueArray"))
	static bool UnpackMySQLDataToBPStructArray(const FString StrRst, TArray<FCustomStructValue>& ValueArray);
	DECLARE_FUNCTION(execUnpackMySQLDataToBPStructArray);

};
