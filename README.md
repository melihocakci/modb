## Installing Berkeley DB

cd build_unix
../dist/configure --enable-cxx
make
sudo make install

## For debugging:

Run build_debug.sh. Binaries will be placed under debug/bin. 
You can then start debugging using vscode. Edit .vscode/launch.json if needed.