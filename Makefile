all: writecred.exe

writecred.exe: writecred.cpp
	cl -O1 -EHsc writecred.cpp -link advapi32.lib

