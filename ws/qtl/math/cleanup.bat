del mcdemo\Debug\*.* /q
del mcdemo\Release\*.* /q
attrib mcdemo\mcdemo.sln +R
attrib mcdemo\mcdemo.vcproj +R
del mcdemo\*.* /q
del mcdemo\*.suo /a:h /q
attrib mcdemo\mcdemo.* -R
del ..\..\..\bin\mcdemo.exe