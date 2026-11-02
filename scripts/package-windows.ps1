param([string]$BuildDir = 'build', [string]$OutputDir = 'dist')
$ErrorActionPreference = 'Stop'
$exe = Join-Path $BuildDir 'Kassa.exe'
if (-not (Test-Path $exe)) { throw "Application missing: $exe" }
$package = Join-Path $OutputDir 'Kassa'
if (Test-Path $package) { Remove-Item $package -Recurse -Force }
New-Item $package -ItemType Directory -Force | Out-Null
Copy-Item $exe $package
& windeployqt --release --compiler-runtime --no-translations --no-opengl-sw $package/Kassa.exe
if ($LASTEXITCODE -ne 0) { throw 'windeployqt failed' }
# Ship the app-local x64 CRT, so users do not need to run vc_redist.
if (-not $env:VCToolsRedistDir) { throw 'Run from an x64 MSVC developer shell' }
$crt = Join-Path $env:VCToolsRedistDir 'x64/Microsoft.VC143.CRT'
if (-not (Test-Path $crt)) { throw "MSVC runtime directory missing: $crt" }
Copy-Item "$crt/*.dll" $package
if (-not (Test-Path "$package/platforms/qwindows.dll")) { throw 'Qt Windows platform plugin missing' }
Copy-Item README.md "$package/README.md"
$zip = Join-Path $OutputDir 'Kassa-windows-x64.zip'
if (Test-Path $zip) { Remove-Item $zip }
Compress-Archive -Path $package -DestinationPath $zip
