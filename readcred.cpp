// readcred.cpp

#include <iostream>
#include <string>

#include <windows.h>
#include <wincred.h>

int main(int argc, char *argv[])
{
    if (2 > argc) {
        std::cerr << "Usage: " << argv[0] << " <credential-name>\n";
        return -1;                                                    // RETURN
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

    std::wstring utf16Target;
    utf16Target.resize(utf16Len + 1);
    ::MultiByteToWideChar(CP_UTF8,
                          MB_ERR_INVALID_CHARS,
                          argv[1],
                          utf8Len,
                          &utf16Target[0],
                          utf16Target.length());

    CREDENTIALW *credential;
    if (FALSE == ::CredReadW(utf16Target.c_str(),
                             CRED_TYPE_GENERIC,
                             0,
                             &credential)) {
        std::cerr << "Could not find credential\n";
        return -1;                                                    // RETURN
    }

    std::cout.write(reinterpret_cast<const char *>(credential->CredentialBlob),
                    credential->CredentialBlobSize);

    ::CredFree(&credential);
    return 0;
}

