//
// Usage  MultiFileOpenDialog.exe "Text"  "*.txt"  "All Files"  "*.*"
//
//

#include <windows.h>
#include <shobjidl.h> 
#include <stdio.h> 
#include <iostream>
using namespace std;


int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow)
{
    int argc;
    LPTSTR* argv = CommandLineToArgvW(pCmdLine, &argc);

    //MessageBoxW(0, pCmdLine, TEXT("lpCmdLine"), MB_OK);
    //for (int i = 0; i < argc; ++i) {
      //  MessageBoxW(0, argv[i], TEXT("argv"), MB_OK);
    //}
    
    // Check for a valid number of arguments! The arguments should be validity checked in the ruby code.
    // pCmdLine should point to the command line arguments but if this program is spawned without arguments
    // then pCmdLine points to the executable path.
    // See also: Chris Wellons, wellons@nullprogram.com. 'The wild west of Windows command line parsing'
    // https://nullprogram.com/blog/2022/02/18/
    if (argc <= 2) {
        cerr << "Wrong number of Filter Spec pairs:" << argc << " found\n";
        return 1;
    }
    if (argc > 20) {
        cerr << "Maximum number of Filter Spec pairs exceeded:" << argc << " found\n";
        return 1;
    }

    // Create Filter Spec aray
    // example: COMDLG_FILTERSPEC ComDlgFS[] = { {L"LAZ/LAS", L"*.laz;*.las"}, {L"Text",L"*.txt"} , {L"All Files",L"*.*"} };
    int fs_count = 0;
    COMDLG_FILTERSPEC ComDlgFS[10] = {};
    for (int i = 0; i < argc;) {
        COMDLG_FILTERSPEC ComDlgFSi = { argv[i++], argv[i++] };
        ComDlgFS[fs_count++] = ComDlgFSi;
    }

    HRESULT hr = CoInitializeEx(NULL, COINIT_APARTMENTTHREADED |
        COINIT_DISABLE_OLE1DDE);
    if (SUCCEEDED(hr))
    {
        IFileOpenDialog* pFileOpen;

        // Create the FileOpenDialog object.
        hr = CoCreateInstance(CLSID_FileOpenDialog, NULL, CLSCTX_ALL,
            IID_IFileOpenDialog, reinterpret_cast<void**>(&pFileOpen));

        if (SUCCEEDED(hr))
        {
            // Set options for a filesystem picker dialog.
            FILEOPENDIALOGOPTIONS opt{};
            pFileOpen->GetOptions(&opt);
            pFileOpen->SetOptions(opt | FOS_ALLOWMULTISELECT);
            pFileOpen->SetFileTypes(fs_count, ComDlgFS);

            // Show the Open dialog box.
            hr = pFileOpen->Show(NULL);

            // Get the file names from the dialog box.
            if (SUCCEEDED(hr))
            {
                IShellItemArray* pItemArray;
                hr = pFileOpen->GetResults(&pItemArray);

                if (SUCCEEDED(hr))
                {
                    DWORD dwItemCount = 0;

                    hr = pItemArray->GetCount(&dwItemCount);
                    if (SUCCEEDED(hr))
                    {
                        IShellItem* pItem;
                        PWSTR pszFilePath;

                        for (DWORD dwItem = 0; dwItem < dwItemCount; dwItem++)
                        {
                            hr = pItemArray->GetItemAt(dwItem, &pItem);
                            if (SUCCEEDED(hr))
                            {
                                hr = pItem->GetDisplayName(SIGDN_FILESYSPATH, &pszFilePath);

                                // 
                                if (SUCCEEDED(hr))
                                {
                                    //MessageBoxW(NULL, pszFilePath, L"File Path", MB_OK);
                                    // ALSO fprintf( stderr, "my %s has %d chars\n", "string format", 30);
                                    wcerr << pszFilePath << "\n";
                                    CoTaskMemFree(pszFilePath);
                                }

                            }
                            pItem->Release();
                        }
                    }
                    pItemArray->Release();
                }
            }
            pFileOpen->Release();
        }
        CoUninitialize();
    }
    return 0;
}
