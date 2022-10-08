@ECHO off

SETLOCAL EnableDelayedExpansion

FOR /r "%~dp0Protostar-Engine\" %%a IN (*.Sharpmake.cs) DO (
	SET filepath=%%~nxa
	FOR /F "tokens=1,2 delims=." %%i IN ("!filepath!") DO (
		SET after_underscore=%%j
		ECHO Update module sharpmake file of !after_underscore!
		
		FOR /F %%N IN ('FIND "" /v /c ^<"%%a"") DO SET /A moduleFileCnt=%%N
		
		SET /A moduleFileLines[!moduleFileCnt!]
		
		SET /A moduleType=0
		
		SET "sharpmakeTextFilePath="

		FOR /F %%N IN  ('FIND "" /v /c ^< "!sharpmakeTextFilePath!"') DO SET /A cnt=%%N

		SET /A lines[!cnt!]

		<!sharpmakeTextFilePath! (
			FOR /L %%N IN (1 1 %cnt%) DO (
				SET /P "str.%%N="
				
				SET lines[%%N]=!str.%%N!
				CALL :strlen len lines[%%N]
				IF NOT !len!==0 (
					CALL SET lines[%%N]=%%lines[%%N]:{ModuleName}=!%~3!%%
					ECHO !lines[%%N]!>>!%~1!
				) ELSE (
					ECHO.>>!%~1!
				)
			)
		)
	)
)

PAUSE

ENDLOCAL