@ECHO off
SETLOCAL EnableDelayedExpansion
SET /P moduleType="Enter Module type (0 = Engine, 1 = Editor, 2 = Project): "

SET sharpmakeTextFilePath=%~dp0Snippets\CodeSnippets\SharpmakeDefaultText.cs
ECHO Create files for module type: !moduleType!
SET "modulesPath="
IF !moduleType!==0 (
	SET modulesPath=%~dp0Protostar-Engine\Source\Modules\
) 
IF !moduleType!==1 (
	SET modulesPath=%~dp0Protostar-Editor\Source\Modules\
)
IF !moduleType!==2 (
	::implement project based modules
	ECHO Project based modules are currently not implemented.
	PAUSE 
	EXIT /b 1
)

SET /A checkRange=0
IF !moduleType! GEQ 3 SET /A checkRange=1
IF !moduleType! LSS 0  SET /A checkRange=1

 IF !checkRange!==1 (
	ECHO Module type !moduleType! not supported.
	PAUSE
	EXIT /b 1
)

SET /P moduleName="Enter Module name: "

SET modulePath=!modulesPath!!moduleName!
ECHO try create !modulePath!
IF NOT EXIST !modulePath! mkdir !modulePath!
SET moduleFilePath=!modulePath!\Protostar.!moduleName!.Sharpmake.cs
IF EXIST !moduleFilePath! (
	SET /P overWrite="Do you want to overwrite the existing sharpmake file? (Y/N)"
	IF !overWrite!==Y (
		DEL /Q "!moduleFilePath!"
		CALL :createSharpmakeFile moduleFilePath
	)
) ELSE (
	CALL :createSharpmakeFile moduleFilePath
)
ECHO try create !moduleFilePath!

SET sourcePath=!modulePath!\Source
ECHO try create !sourcePath!
IF NOT EXIST !sourcePath! (
	mkdir !sourcePath!
	ECHO Sourcepath successfully created
) ELSE (
	ECHO Sourcepath already exists
)

SET privatePath=!sourcePath!\Private
SET publicPath=!sourcePath!\Public
ECHO try create !privatePath!
IF NOT EXIST !privatePath! (
	mkdir !privatePath!
	ECHO Private source path successfully created
) ELSE (
	ECHO Private source path already exists
)
ECHO try create !publicPath!
IF NOT EXIST !publicPath! ( 
	mkdir !publicPath!
	ECHO Public source path successfully created
) ELSE (
	ECHO Public source path already exists
)

SET /P callBuildBatch="Do you want to run Build.bat? (Y/N)"

IF !callBuildBatch!==Y (
	CALL "Build.bat"
) ELSE (
	PAUSE
	EXIT /b %ERRORLEVEL%
)



REM *********  strlen function *****************************
:strlen <resultVar> <stringVar>
(   
    setlocal EnableDelayedExpansion
    (set^ tmp=!%~2!)
    if defined tmp (
        set "len=1"
        for %%P in (4096 2048 1024 512 256 128 64 32 16 8 4 2 1) do (
            if "!tmp:~%%P,1!" NEQ "" ( 
                set /a "len+=%%P"
                set "tmp=!tmp:~%%P!"
            )
        )
    ) ELSE (
        set len=0
    )
)
(
    endlocal
    set "%~1=%len%"
    exit /b
)

REM *********  createSharpmakeFile function *****************************
:createSharpmakeFile <filePath>
FOR /F %%N IN  ('FIND "" /v /c ^< "!sharpmakeTextFilePath!"') DO SET  /A cnt=%%N

SET /A lines[!cnt!]

<!sharpmakeTextFilePath! (
	FOR /L %%N IN (1 1 %cnt%) DO (
		SET /P "str.%%N="
		
		SET lines[%%N]=!str.%%N!
		CALL :strlen len lines[%%N]
		IF NOT !len!==0 (
			CALL SET lines[%%N]=%%lines[%%N]:{ModuleName}=%moduleName%%%
			ECHO !lines[%%N]!>>!moduleFilePath!
		) ELSE (
			ECHO.>>!moduleFilePath!
		)
	)
)
EXIT /b

