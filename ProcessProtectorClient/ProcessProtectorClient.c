#include <Windows.h>
#include <winioctl.h>
#include <stdio.h>
#include "../ProcessProtector/ProcessProtectorCommon.h"

int main(int argc, char* argv[])
{
	if (argc < 2) {
		printf("Usage: %s <command> [pid]\n", argv[0]);
		printf("Commands:\n");
		printf("  protect <pid>   - Protect the process with the given PID\n");
		printf("  unprotect <pid> - Unprotect the process with the given PID\n");
		printf("  clear           - Clear all protected processes\n");
		return 1;
	}

	HANDLE hDevice = CreateFileW(L"\\\\.\\ProcessProtector", GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if (hDevice == INVALID_HANDLE_VALUE) {
		printf("Failed to open device (0x%08X)\n", GetLastError());
		return 1;
	}

	switch (argv[1][0]) {
	case 'p': {
		if (argc < 3) {
			printf("Usage: %s protect <pid>\n", argv[0]);
			break;
		}
		for (int i = 2; i < argc; ++i) {
			ULONGLONG pid = atoi(argv[i]);
			DeviceIoControl(hDevice, IOCTL_PROCESS_PROTECT_BY_PID, &pid, sizeof(pid), NULL, 0, NULL, NULL);
		}
		break;
	}
	case 'u': {
		if (argc < 3) {
			printf("Usage: %s unprotect <pid>\n", argv[0]);
			break;
		}
		for (int i = 2; i < argc; ++i) {
			ULONGLONG pid = atoi(argv[i]);
			DeviceIoControl(hDevice, IOCTL_PROCESS_UNPROTECT_BY_PID, &pid, sizeof(pid), NULL, 0, NULL, NULL);
		}
		break;
	}
	case 'c': {
		DeviceIoControl(hDevice, IOCTL_PROCESS_PROTECT_CLEAR, NULL, 0, NULL, 0, NULL, NULL);
		break;
	}
	default: {
		printf("Unknown command: %s\n", argv[1]);
		break;
	}
	}

	CloseHandle(hDevice);
	return 0;
}