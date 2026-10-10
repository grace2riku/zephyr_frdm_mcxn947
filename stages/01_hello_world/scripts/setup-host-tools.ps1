# 指定版を専用の配置先へ導入する。PowerShell 7で実行する。
[CmdletBinding()]
param(
    [string]$Workspace = 'C:\Users\grace\zephyrproject-mcxn947',
    [string]$PythonDir = "$env:LOCALAPPDATA\Programs\Python\Python312"
)
$ErrorActionPreference = 'Stop'
$toolsDir = Join-Path $Workspace 'tools'
$downloadDir = Join-Path $Workspace 'downloads'
New-Item -ItemType Directory -Force -Path $toolsDir, $downloadDir | Out-Null
$packages = @(
    @{ Name = 'python-3.12.10-amd64.exe'; Url = 'https://www.python.org/ftp/python/3.12.10/python-3.12.10-amd64.exe'; Sha256 = '67b5635e80ea51072b87941312d00ec8927c4db9ba18938f7ad2d27b328b95fb' },
    @{ Name = 'cmake-3.31.8-windows-x86_64.zip'; Url = 'https://github.com/Kitware/CMake/releases/download/v3.31.8/cmake-3.31.8-windows-x86_64.zip'; Sha256 = '81aa9964dbabd71fe02e7ec50472fd3ad56138c49944515ece9001efbff8d719' },
    @{ Name = 'ninja-win.zip'; Url = 'https://github.com/ninja-build/ninja/releases/download/v1.13.1/ninja-win.zip'; Sha256 = '26a40fa8595694dec2fad4911e62d29e10525d2133c9a4230b66397774ae25bf' },
    @{ Name = 'gperf-3.1-win64_x64.zip'; Url = 'https://github.com/oss-winget/oss-winget-storage/raw/96ea1b934342f45628a488d3b50d0c37cf06012c/packages/gperf/3.1/gperf-3.1-win64_x64.zip'; Sha256 = '5cf7adac20cdcc1ac2ca31816b0e2e261781d88c3d57049f7eae1fb85119f237' },
    @{ Name = 'dtc-msys2-1.6.1-x86_64.zip'; Url = 'https://github.com/oss-winget/oss-winget-storage/raw/96ea1b934342f45628a488d3b50d0c37cf06012c/packages/dtc/1.6.1/dtc-msys2-1.6.1-x86_64.zip'; Sha256 = '7aac366f989fd2450d5e641e118734653ea29d0ddb7dbfa33521d57afe852ae3' }
)
$records = foreach ($package in $packages) {
    $archive = Join-Path $downloadDir $package.Name
    if (-not (Test-Path -LiteralPath $archive)) {
        Write-Host "Download: $($package.Name)"
        Invoke-WebRequest -Uri $package.Url -OutFile $archive
    }
    $actualHash = (Get-FileHash -LiteralPath $archive -Algorithm SHA256).Hash.ToLowerInvariant()
    if ($actualHash -ne $package.Sha256) { throw "SHA-256 mismatch: $archive" }
    [pscustomobject]@{ File = $package.Name; Url = $package.Url; Sha256 = $actualHash; Verified = $true }
}
$records | ConvertTo-Json | Set-Content -Encoding utf8 (Join-Path $downloadDir 'host-tools-sha256.json')

$pythonExe = Join-Path $PythonDir 'python.exe'
if (-not (Test-Path -LiteralPath $pythonExe)) {
    $installer = Join-Path $downloadDir 'python-3.12.10-amd64.exe'
    $signature = Get-AuthenticodeSignature -FilePath $installer
    if ($signature.Status -ne 'Valid') { throw 'Python installer signature is not valid.' }
    Write-Host "Install Python to $PythonDir"
    $installArgs = @('/quiet', 'InstallAllUsers=0', "TargetDir=`"$PythonDir`"", 'PrependPath=0', 'Include_launcher=0', 'Include_test=0', 'AssociateFiles=0', 'Shortcuts=0')
    $installProcess = Start-Process -FilePath $installer -ArgumentList $installArgs -Wait -PassThru -WindowStyle Hidden
    if ($installProcess.ExitCode -ne 0) { throw "Python installer exit: $($installProcess.ExitCode)" }
}
$pythonVersion = & $pythonExe --version
if ($LASTEXITCODE -ne 0 -or $pythonVersion -ne 'Python 3.12.10') { throw "Unexpected Python: $pythonVersion" }
Write-Host $pythonVersion

if (-not (Test-Path (Join-Path $toolsDir 'cmake-3.31.8-windows-x86_64/bin/cmake.exe'))) {
    Expand-Archive -LiteralPath (Join-Path $downloadDir 'cmake-3.31.8-windows-x86_64.zip') -DestinationPath $toolsDir
}
if (-not (Test-Path (Join-Path $toolsDir 'ninja-1.13.1/ninja.exe'))) {
    Expand-Archive -LiteralPath (Join-Path $downloadDir 'ninja-win.zip') -DestinationPath (Join-Path $toolsDir 'ninja-1.13.1')
}
if (-not (Test-Path (Join-Path $toolsDir 'gperf-3.1/gperf.exe'))) {
    Expand-Archive -LiteralPath (Join-Path $downloadDir 'gperf-3.1-win64_x64.zip') -DestinationPath (Join-Path $toolsDir 'gperf-3.1')
}
if (-not (Test-Path (Join-Path $toolsDir 'dtc-1.6.1/usr/bin/dtc.exe'))) {
    Expand-Archive -LiteralPath (Join-Path $downloadDir 'dtc-msys2-1.6.1-x86_64.zip') -DestinationPath (Join-Path $toolsDir 'dtc-1.6.1')
}
& (Join-Path $toolsDir 'cmake-3.31.8-windows-x86_64/bin/cmake.exe') --version
if ($LASTEXITCODE -ne 0) { throw 'CMake failed.' }
& (Join-Path $toolsDir 'ninja-1.13.1/ninja.exe') --version
if ($LASTEXITCODE -ne 0) { throw 'Ninja failed.' }
& (Join-Path $toolsDir 'gperf-3.1/gperf.exe') --version
if ($LASTEXITCODE -ne 0) { throw 'gperf failed.' }
& (Join-Path $toolsDir 'dtc-1.6.1/usr/bin/dtc.exe') --version
if ($LASTEXITCODE -ne 0) { throw 'DTC failed.' }
