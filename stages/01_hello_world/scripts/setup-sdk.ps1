# 採用Windows SDKのminimal bundleとGNU Armだけを導入する。
[CmdletBinding()]
param(
    [string]$Workspace = 'C:\Users\grace\zephyrproject-mcxn947',
    [string]$SdkDir = 'C:\Users\grace\zephyr-sdk-1.0.1'
)
$ErrorActionPreference = 'Stop'
if ((Split-Path $SdkDir -Leaf) -ne 'zephyr-sdk-1.0.1') {
    throw 'SdkDir must end with zephyr-sdk-1.0.1 (the archive root).'
}
$sevenZip = 'C:\Program Files\7-Zip\7z.exe'
$downloadDir = Join-Path $Workspace 'downloads'
New-Item -ItemType Directory -Force -Path $downloadDir | Out-Null
$packages = @(
    @{ Name = 'zephyr-sdk-1.0.1_windows-x86_64_minimal.7z'; Sha256 = '17a557e7c6cea5e5c589508fe45a3f26aca4fa2c6153fe3e7b03aeb4b93fea4b'; Root = 'zephyr-sdk-1.0.1'; Destination = (Split-Path $SdkDir -Parent); Marker = (Join-Path $SdkDir 'sdk_version') },
    @{ Name = 'toolchain_gnu_windows-x86_64_arm-zephyr-eabi.7z'; Sha256 = 'd08df2e83378dc8d28d0c67ab72d021ca633344b7a2bc053ababf9b8e677aa8d'; Root = 'arm-zephyr-eabi'; Destination = (Join-Path $SdkDir 'gnu'); Marker = (Join-Path $SdkDir 'gnu/arm-zephyr-eabi/bin/arm-zephyr-eabi-gcc.exe') }
)
$records = foreach ($package in $packages) {
    $archive = Join-Path $downloadDir $package.Name
    $url = 'https://github.com/zephyrproject-rtos/sdk-ng/releases/download/v1.0.1/' + $package.Name
    if (-not (Test-Path -LiteralPath $archive)) { Invoke-WebRequest -Uri $url -OutFile $archive }
    $hash = (Get-FileHash -LiteralPath $archive -Algorithm SHA256).Hash.ToLowerInvariant()
    if ($hash -ne $package.Sha256) { throw "SHA-256 mismatch: $archive" }
    if (-not (Test-Path -LiteralPath $package.Marker)) {
        $listing = & $sevenZip l -slt $archive
        if ($LASTEXITCODE -ne 0) { throw 'Cannot list SDK archive.' }
        $entryPaths = @($listing | Select-String '^Path = ' | ForEach-Object { $_.Line.Substring(7) }) | Select-Object -Skip 1
        foreach ($entryPath in $entryPaths) {
            if (($entryPath -ne $package.Root -and -not $entryPath.StartsWith($package.Root + '\')) -or $entryPath -match '(^|\\)\.\.(\\|$)') {
                throw "Unexpected archive path: $entryPath"
            }
        }
        & $sevenZip x $archive "-o$($package.Destination)" -y -bso0 -bsp0
        if ($LASTEXITCODE -ne 0) { throw "SDK extraction failed: $archive" }
    }
    [pscustomobject]@{ File = $package.Name; Url = $url; Sha256 = $hash; Verified = $true }
}
$records | ConvertTo-Json | Set-Content -Encoding utf8 (Join-Path $downloadDir 'sdk-sha256.json')
if ((Get-Content (Join-Path $SdkDir 'sdk_version') -Raw).Trim() -ne '1.0.1') { throw 'Unexpected SDK version.' }
. (Join-Path $PSScriptRoot 'env.ps1') -Workspace $Workspace -SdkDir $SdkDir
& (Join-Path $SdkDir 'setup.cmd') /t arm-zephyr-eabi /h /c
if ($LASTEXITCODE -ne 0) { throw 'SDK setup failed.' }
& (Join-Path $SdkDir 'gnu/arm-zephyr-eabi/bin/arm-zephyr-eabi-gcc.exe') --version
if ($LASTEXITCODE -ne 0) { throw 'SDK compiler failed.' }
