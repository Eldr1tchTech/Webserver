@ECHO OFF
REM Build Everything

ECHO "Building everything..."


@REM PUSHD server
@REM CALL build.bat
@REM POPD
@REM IF %ERRORLEVEL% NEQ 0 (echo Error:%ERRORLEVEL% && exit)

@REM PUSHD instance
@REM CALL build.bat
@REM POPD
@REM IF %ERRORLEVEL% NEQ 0 (echo Error:%ERRORLEVEL% && exit)

REM Server
make -f "Makefile.server.windows.mak" all
IF %ERRORLEVEL% NEQ 0 (echo Error:%ERRORLEVEL% && exit)

REM Instance
make -f "Makefile.instance.windows.mak" all
IF %ERRORLEVEL% NEQ 0 (echo Error:%ERRORLEVEL% && exit)

ECHO "All assemblies built successfully."