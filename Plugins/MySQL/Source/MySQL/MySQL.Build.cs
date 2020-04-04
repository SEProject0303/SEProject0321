// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

using System.IO;

public class MySQL : ModuleRules
{
    private string ModulePath
    {
        get { return ModuleDirectory; }
    }

    public MySQL(ReadOnlyTargetRules Target) : base(Target)
    {
        bEnableExceptions = true;

        PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicIncludePaths.AddRange(
            new string[] {
				// ... add public include paths required here ...
			}
            );

        PrivateIncludePaths.AddRange(
            new string[] {
				// ... add other private include paths required here ...
			}
            );

        PublicDependencyModuleNames.AddRange(
            new string[]
            {
                "Core",
				// ... add other public dependencies that you statically link with here ...
			}
            );

        PrivateDependencyModuleNames.AddRange(
            new string[]
            {
                "CoreUObject",
                "Engine",
                "Slate",
                "SlateCore",
				// ... add private dependencies that you statically link with here ...	
			}
            );

        DynamicallyLoadedModuleNames.AddRange(
            new string[]
            {
				// ... add any modules that your module loads dynamically here ...
			}
            );

        string MySQLConnectorIncludePath = ModulePath + "/MySQLSDK/include/jdbc/";
        string MySQLConnectorLibraryPath = ModulePath + "/MySQLSDK/lib64/";
        string MySQLConnectorImportLibraryName = Path.Combine(MySQLConnectorLibraryPath, "vs14/mysqlcppconn.lib");
        string MySQLConnectorDLLName = Path.Combine(MySQLConnectorLibraryPath, "mysqlcppconn-7-vs14.dll");

        if (!File.Exists(MySQLConnectorImportLibraryName))
        {
            throw new BuildException(string.Format("{0} could not be found.", MySQLConnectorImportLibraryName));
        }

        if (!File.Exists(MySQLConnectorDLLName))
        {
            throw new BuildException(string.Format("{0} could not be found.", MySQLConnectorDLLName));
        }

        PublicIncludePaths.Add(MySQLConnectorIncludePath);
        PublicLibraryPaths.Add(MySQLConnectorLibraryPath);
        PublicLibraryPaths.Add(MySQLConnectorDLLName);
        PublicAdditionalLibraries.Add(MySQLConnectorImportLibraryName);
    }
}
