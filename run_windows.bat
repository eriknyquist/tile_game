@echo off

SET "bin_dir=windows_build"
SET "bin_path=%~dp0%bin_dir%"
SET "PATH=%bin_path%;%PATH%"
START %bin_path%\game.exe
