@ECHO off
SETLOCAL EnableDelayedExpansion
SET /P moduleType="Enter Module type (0 = Engine, 1 = Editor, 2 = Project): "
SET /P moduleName="Enter Module name: "

SET sharpmakeTextFilePath=%~dp0Snippets\CodeSnippets\SharpmakeDefaultText.cs
ECHO !sharpmakeTextFilePath!

ECHO Create files for module type: !moduleType! 
PAUSE
SET modulesPath=%~dp0Protostar-Engine\Source\Modules\
SET modulePath=!modulesPath!!moduleName!
ECHO try create !modulePath!
PAUSE
IF NOT EXIST !modulePath! mkdir !modulePath!
SET moduleFilePath=!modulePath!\Protostar.!moduleName!.Sharpmake.cs
IF EXIST !moduleFilePath! (
	SET /P overWrite="Do you want to overwrite the existing sharpmake file? (Y/N)"
	IF !overWrite!==Y (
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
IF NOT EXIST !privatePath! mkdir !privatePath!
ECHO try create !publicPath!
IF NOT EXIST !publicPath! mkdir !publicPath!

PAUSE


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
			ECHO !lines[%%N]!
			ECHO !lines[%%N]!>>!moduleFilePath!
		) ELSE (
			ECHO.
			ECHO.>>!moduleFilePath!
		)
	)
)
EXIT /b

