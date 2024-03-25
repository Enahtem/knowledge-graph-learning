Set-Location ..
$env:PATH += ";$(Get-Location)\lib"

# Compile the source file
g++ -o ./build/demo.exe ./src/main.c `
    -I ./include `
    -L ./lib `
    -lmingw32 `
    -lSDL2main `
    -lSDL2 `
    -luser32 `
    -lgdi32

# Check if compilation was successful
if ($LASTEXITCODE -eq 0) {
    Write-Output "Compilation successful. Running the executable..."
    Start-Process -FilePath "./build/demo.exe" -NoNewWindow
} else {
    Write-Output "Compilation failed. Please check for errors."
}
