@echo off

REM Mongo C Driver uninstall program, generated with CMake

REM Copyright 2018-present MongoDB, Inc.
REM
REM Licensed under the Apache License, Version 2.0 (the "License");
REM you may not use this file except in compliance with the License.
REM You may obtain a copy of the License at
REM
REM   http://www.apache.org/licenses/LICENSE-2.0
REM
REM Unless required by applicable law or agreed to in writing, software
REM distributed under the License is distributed on an "AS IS" BASIS,
REM WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
REM See the License for the specific language governing permissions and
REM limitations under the License.



REM Windows does not handle a batch script deleting itself during
REM execution.  Copy the uninstall program into the TEMP directory from
REM the environment and run from there so everything in the installation
REM is deleted and the copied program deletes itself at the end.
if /i "%~dp0" NEQ "%TEMP%\" (
   copy "%~f0" "%TEMP%\mongoc-%~nx0" >NUL
   "%TEMP%\mongoc-%~nx0" & del "%TEMP%\mongoc-%~nx0"
)

pushd C:\Program\

