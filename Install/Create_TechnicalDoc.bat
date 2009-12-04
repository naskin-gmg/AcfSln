echo Creation of technical documentation...

echo Create doxyfile...
"%ACFDIR%/bin/Debug%COMPILER_EXT%/Acf.exe" Create_Doxyfile.arx -config ../Config/Core.xpc

cd TempExcl

doxygen

cd ..

echo Technical documentation created
