@REM ******************************************************************
@REM * FILE PURPOSE: Check environment setup
@REM ******************************************************************
@REM * FILE NAME: checkenv.bat
@REM *
@REM * DESCRIPTION:
@REM *  Checks the build environment and sets PATH for mmWave R4 Testbench.
@REM *
@REM ******************************************************************
@echo off

REM -------------------------------------------------------------------
REM Sanity Check: Ensure that all the required variables are defined
REM -------------------------------------------------------------------
if [%R4_TB_PATH%] == [] (
@echo Error: R4_TB_PATH [mmWave R4 TB Location] needs to be defined
@echo For Example:
@echo   set R4_TB_INSTALL_PATH=C:/work/r4_tb
goto fail
)
if [%R4_CODEGEN_INSTALL_PATH%] == [] (
@echo Error: R4F_CODEGEN_INSTALL_PATH [R4F Toochain] needs to be defined
@echo For Example:
@echo   set R4F_CODEGEN_INSTALL_PATH=C:/ti/ti-cgt-arm_18.12.2.LTS
goto fail
)

if [%XDC_INSTALL_PATH%] == [] (
@echo Error: XDC_INSTALL_PATH needs to be defined.
@echo For Example:
@echo   set XDC_INSTALL_PATH=c:/ti/xdc
goto fail
)


REM ----------------------------------------------------------
REM Setup the PATH configuration:
REM Please do not change anything below this
REM ----------------------------------------------------------

REM XDC Tools including gmake
REM Add XDC to path if not already in PATH
echo "%PATH%" | find /i "%XDC_INSTALL_PATH%">nul || set "PATH=%XDC_INSTALL_PATH%;%XDC_INSTALL_PATH%/bin;%PATH%"

if NOT ["%GRAPHVIZ_INSTALL_PATH%"] == [""] (
    echo "%PATH%" | find /i "%GRAPHVIZ_INSTALL_PATH%\bin">nul || set "PATH=%PATH%;%GRAPHVIZ_INSTALL_PATH%\bin"
)
if NOT ["%MIKTEX_INSTALL_PATH%"] == [""] (
    echo "%PATH%" | find /i "%MIKTEX_INSTALL_PATH%">nul || set "PATH=%PATH%;%MIKTEX_INSTALL_PATH%"
)
if NOT ["%GHOSTSCRIPT_INSTALL_PATH%"] == [""] (
    echo "%PATH%" | find /i "%GHOSTSCRIPT_INSTALL_PATH%\bin">nul || set "PATH=%PATH%;%GHOSTSCRIPT_INSTALL_PATH%\bin"
)

@echo -----------------------------------------------
@echo mmWave R4 Testbench Build Environment Configured
@echo -----------------------------------------------

REM Set the Title Window appropriately.
Title mmWave R4 TB Build Environment
goto end

:fail
@echo Error: Failed to setup mmWave R4 TB Build Environment !
Title Error: Failed to setup mmWave R4 TB Build Environment
REM Return error
EXIT /B 1

:end
REM Return success
EXIT /B 0
