
:: install vcpkg
git clone https://github.com/microsoft/vcpkg.git

cd vcpkg

:: setup for windows
bootstrap-vcpkg.bat

:: install AWS SDK libraries
vcpkg.exe install openssl --recurse

cd ..

pause