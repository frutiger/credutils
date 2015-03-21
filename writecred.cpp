// writecred.cpp

#include <fstream>
#include <iostream>
#include <string>
#include <vector>

#include <windows.h>
#include <wincred.h>

int main(int argc, char *argv[])
{
    if (3 > argc) {
        std::cerr << "Usage: " << argv[0] << " <credential-name> "
                                             " ( <filename> | - )\n";
        return -1;                                                    // RETURN
    }

    std::streambuf *input;
    std::filebuf    fileInput;
    if (argv[2][0] == '-' && argv[2][1] == 0) {
        input = std::cin.rdbuf();
    }
    else {
        fileInput.open(argv[2], std::ios_base::binary | std::ios_base::in);
        if (!fileInput.is_open()) {
            std::cerr << "Failed to open " << argv[2] << '\n';
            return -1;                                                // RETURN
        }
        input = &fileInput;
    }

    int utf8Len  = std::char_traits<char>::length(argv[1]);
    int utf16Len = ::MultiByteToWideChar(CP_UTF8,
                                         MB_ERR_INVALID_CHARS,
                                         argv[1],
                                         utf8Len,
                                         0,
                                         0);
    if (0 == utf16Len) {
        std::cerr << "Failed to encode credential name\n";
        return -1;                                                    // RETURN
    }

    if (utf16Len == 0xFFFD) {
        std::cerr << "Invalid UTF-8 credential name\n";
        return -1;                                                    // RETURN
    }

    std::wstring utf16Target;
    utf16Target.resize(utf16Len + 1);
    ::MultiByteToWideChar(CP_UTF8,
                          MB_ERR_INVALID_CHARS,
                          argv[1],
                          utf8Len,
                          &utf16Target[0],
                          utf16Target.length());

    std::vector<char> data;
    std::copy(std::istreambuf_iterator<char>(input),
              std::istreambuf_iterator<char>(),
              std::back_inserter(data));


    CREDENTIALW credential        = {};
    credential.Type               = CRED_TYPE_GENERIC;
    credential.TargetName         = &utf16Target[0];
    credential.CredentialBlobSize = data.size();
    credential.CredentialBlob     = reinterpret_cast<unsigned char *>(
                                                                  data.data());
    credential.Persist            = CRED_PERSIST_LOCAL_MACHINE;
    credential.UserName           = L"credutils";
    credential.Comment            = L"Saved by credutils";

    if (FALSE == ::CredWriteW(&credential, 0)) {
        std::cerr << "Could not store credential\n";
        return -1;                                                    // RETURN
    }
    return 0;
}

