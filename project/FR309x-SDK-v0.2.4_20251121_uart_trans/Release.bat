@echo off
REM 获取最新的 Git 标签
for /f "tokens=*" %%i in ('git describe --tags') do set VERSION=%%i

REM 检查是否成功获取版本号
if "%VERSION%"=="" (
    echo No tags found in the repository. Please create a tag before releasing.
    exit /b 1
)

REM 生成 VERSION 文件
echo %VERSION% > VERSION
echo VERSION file created with version %VERSION%

REM 生成 version.h 文件
echo /* Auto-generated version header file */ > version.h
echo #ifndef __VERSION_H__ >> version.h
echo #define __VERSION_H__ >> version.h
echo #define SDK_VERSION "%VERSION%" >> version.h
echo #endif /* __VERSION_H__ */ >> version.h
echo version.h file created with version %VERSION%

REM 使用 git archive 打包项目，并包含 VERSION 和 version.h 文件
git archive --format=zip --output=FR309x-SDK-%VERSION%.zip --add-file=VERSION --add-file=version.h %VERSION%

REM 删除临时生成的 VERSION 和 version.h 文件
del VERSION
del version.h

echo Project archived as FR309x-SDK-%VERSION%.zip