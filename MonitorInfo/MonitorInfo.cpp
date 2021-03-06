// MonitorINnfo.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <windows.h>
#include <string>
#include <iostream>

DISPLAYCONFIG_TOPOLOGY_ID Topology_IDs[] = { DISPLAYCONFIG_TOPOLOGY_INTERNAL, DISPLAYCONFIG_TOPOLOGY_CLONE, DISPLAYCONFIG_TOPOLOGY_EXTEND, DISPLAYCONFIG_TOPOLOGY_EXTERNAL };
const WCHAR * Topology_NAMES[] = { L"Internal", L"clone", L"extended", L"external" };

int main()
{
    LONG                        ret;
    UINT32                      stage = 0;
    UINT32                      NumPathArrayElements;
    DISPLAYCONFIG_PATH_INFO   * pPathInfoArray;
    UINT32                      NumModeInfoArrayElements;
    DISPLAYCONFIG_MODE_INFO   * pModeInfoArray;
    DISPLAYCONFIG_TARGET_DEVICE_NAME TargetName;
    char                        ch;

    ret = GetDisplayConfigBufferSizes(QDC_DATABASE_CURRENT, &NumPathArrayElements, &NumModeInfoArrayElements);
    if (ret == ERROR_SUCCESS) {
        stage++;
        pPathInfoArray = ((DISPLAYCONFIG_PATH_INFO*)malloc(sizeof(DISPLAYCONFIG_PATH_INFO) * NumPathArrayElements));
        pModeInfoArray = ((DISPLAYCONFIG_MODE_INFO*)malloc(sizeof(DISPLAYCONFIG_MODE_INFO) * NumModeInfoArrayElements));

        ZeroMemory(pPathInfoArray, sizeof(DISPLAYCONFIG_PATH_INFO) * NumPathArrayElements);
        ZeroMemory(pModeInfoArray, sizeof(DISPLAYCONFIG_MODE_INFO) * NumModeInfoArrayElements);

        for (unsigned t = 0; t < 4; t++) {
            ret = QueryDisplayConfig(QDC_DATABASE_CURRENT, &NumPathArrayElements, pPathInfoArray, &NumModeInfoArrayElements, pModeInfoArray, &Topology_IDs[t]);
            if (ret == ERROR_SUCCESS) {
                stage++;
                for (unsigned int i = 0; i < NumPathArrayElements; i++) {
                    TargetName.header.type = DISPLAYCONFIG_DEVICE_INFO_GET_TARGET_NAME;
                    TargetName.header.size = sizeof(DISPLAYCONFIG_TARGET_DEVICE_NAME);
                    TargetName.header.adapterId = pPathInfoArray[i].targetInfo.adapterId;
                    TargetName.header.id = pPathInfoArray[i].targetInfo.id;

                    ret = DisplayConfigGetDeviceInfo(&TargetName.header);
                    if (ret == ERROR_SUCCESS) {
                        stage = 100;
                        //We should have the target names here
                        std::wcout << TargetName.monitorFriendlyDeviceName;
                        std::wcout << L" topology: " << Topology_NAMES[t] << "\n";

                    }
                }
            }

        }
    }
    if (stage < 100) {
        std::cout << "stage " << stage << " failed with " << ret;
    }

    std::cout << "Enter return to exit";
    system("pause");
    return 0;
}

