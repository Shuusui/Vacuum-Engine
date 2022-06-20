@ECHO off
SETLOCAL EnableDelayedExpansion
SET /P moduleType="Enter Module type (0 = Engine, 1 = Editor, 2 = Project): "
SET /P moduleName="Enter Module name: "

SET sharpmakeTextFilePath=%~dp0Snippets\CodeSnippets\SharpmakeDefaultText.cs
ECHO !sharpmakeTextFilePath!

FOR /F %%N IN  ('FIND "" /v /c ^< "!sharpmakeTextFilePath!"') DO SET  /A cnt=%%N

SET /A lines[!cnt!]

<!sharpmakeTextFilePath! (
	FOR /L %%N IN (1 1 %cnt%) DO (
	SET /P "str.%%N="
	CALL :strlen len str.%%N
	IF NOT !len!==0 (
		SET temp=!str.%%N!
		ECHO !temp!
		CALL SET temp=%%temp:{ModuleName}=%moduleName%%%
		SET str.%%N=!temp!
		SET lines[%%N] = !temp!
		ECHO !str.%%N!
		)
	)
)
PAUSE

IF %moduleType%==0 (
	SET modulesPath=%~dp0\Protostar-Engine\Source\Modules\
	SET modulePath=!modulesPath!!moduleName!
	echo try create !modulePath!
	if not exist  !modulePath! mkdir !modulePath!
	SET moduleFilePath=!modulePath!\Protostar.!moduleName!.Sharpmake.cs
	echo try create !moduleFilePath!
	
	::copy !sharpmakeTextFilePath! !moduleFilePath!
	
	::FOR /L %%N IN (1 1 %cnt%) DO (
		::ECHO !str.%%N!
		::ECHO !str.%%N!>>!moduleFilePath!
	::)
	SET sourcePath=!modulePath!\Source
	echo try create !sourcePath!
	if not exist !sourcePath! mkdir !sourcePath!
	SET privatePath=!sourcePath!\Private
	SET publicPath=!sourcePath!\Public
	echo try create !privatePath!
	if not exist !privatePath! mkdir !privatePath!
	echo try create !publicPath!
	if not exist !publicPath! mkdir !publicPath!
	)
pause


REM ********* function *****************************
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