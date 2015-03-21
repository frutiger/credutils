all: writecred.exe readcred.exe delcred.exe

writecred.exe: writecred.cpp
	cl -O1 -EHsc writecred.cpp -link advapi32.lib

readcred.exe: readcred.cpp
	cl -O1 -EHsc readcred.cpp -link advapi32.lib

delcred.exe: delcred.cpp
	cl -O1 -EHsc delcred.cpp -link advapi32.lib

