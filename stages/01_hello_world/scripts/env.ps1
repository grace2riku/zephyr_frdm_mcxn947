# ドットソースで読み込む: . .\stages\01_hello_world\scripts\env.ps1
# 変数とPATHは現在のPowerShellプロセスにだけ設定する。
[CmdletBinding()]
param(
    [string]$Workspace = 'C:\Users\grace\zephyrproject-mcxn947',
    [string]$SdkDir = 'C:\Users\grace\zephyr-sdk-1.0.1',
    [string]$LinkServerDir = 'C:\nxp\LinkServer_1.4.85'
)
$env:VIRTUAL_ENV = Join-Path $Workspace '.venv'
$env:ZEPHYR_BASE = Join-Path $Workspace 'zephyr'
$env:ZEPHYR_TOOLCHAIN_VARIANT = 'zephyr'
$env:ZEPHYR_SDK_INSTALL_DIR = $SdkDir
$env:PYTHONUTF8 = '1'
$sessionPaths = @(
    (Join-Path $env:VIRTUAL_ENV 'Scripts'),
    (Join-Path $Workspace 'tools/cmake-3.31.8-windows-x86_64/bin'),
    (Join-Path $Workspace 'tools/ninja-1.13.1'),
    (Join-Path $Workspace 'tools/gperf-3.1'),
    (Join-Path $Workspace 'tools/dtc-1.6.1/usr/bin'),
    $LinkServerDir,
    'C:\Program Files\7-Zip'
)
$env:Path = (($sessionPaths + ($env:Path -split ';')) | Select-Object -Unique) -join ';'
$zephyrWorkspace = $Workspace
$learningRepo = Split-Path (Split-Path (Split-Path $PSScriptRoot -Parent) -Parent) -Parent
$helloBuild = Join-Path $Workspace 'build/01_hello_world'
$helloDebugBuild = Join-Path $Workspace 'build/01_hello_world_debug'
