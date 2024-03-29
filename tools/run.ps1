# Set directory to root
Set-Location ..

# Making all dlls in lib accessible
$directory = "$(Get-Location)\lib"
$subdirectories = Get-ChildItem -Path $directory -Directory -Recurse
foreach ($subdir in $subdirectories) {
    $env:PATH += ";" + $subdir.FullName
}

# Compile the source file
gcc -o ./build/demo.exe ./src/main.c `
    -I ./include `
    -L ./lib/* `
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
