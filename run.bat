@echo off

REM Get the port number from the command line
IF "%~1"=="" (
    set "PORT=0"
) ELSE (
    set "PORT=%~1"
)

REM Get the image name from the file
set /p IMAGE=<.docker/image_name

REM Check if the image is already available, and pull if needed
docker image inspect %IMAGE% >nul 2>&1
IF %ERRORLEVEL% NEQ 0 (
    echo Image not found locally. Pulling %IMAGE%...
    docker pull %IMAGE%
    IF %ERRORLEVEL% NEQ 0 (
        echo Failed to pull image %IMAGE%.
        exit /b 1
    )
    echo:
    echo:
    echo:
)

REM Copy the run script from the image
FOR /F %%i IN ('docker create %IMAGE%') DO SET CID=%%i
docker cp %CID%:/interface.ps1 .interface.ps1 >nul
docker rm -v %CID% >nul

REM Run the image's interface script
REM powershell -ExecutionPolicy Bypass -File .interface.ps1 %IMAGE%
IF %PORT% NEQ 0 (
    powershell -ExecutionPolicy Bypass -File .interface.ps1 -image %IMAGE% -port %PORT%
) ELSE (
    powershell -ExecutionPolicy Bypass -File .interface.ps1 -image %IMAGE%
)

pause
