#include <ntifs.h>
#include "ProcessProtectorCommon.h"

#define DRIVER_PREFIX "ProcessProtector: "

VOID DriverUnload(PDRIVER_OBJECT);
NTSTATUS ProcessProtectorCreateClose(PDEVICE_OBJECT, PIRP);
NTSTATUS ProcessProtectorIoControl(PDEVICE_OBJECT, PIRP);
OB_PREOP_CALLBACK_STATUS OnPreOpenProcess(PVOID, POB_PRE_OPERATION_INFORMATION);

FAST_MUTEX g_ProtectedProcessesMutex;

typedef struct _Globals{
	LIST_ENTRY ProtectedProcessesListHead;
} Globals;

Globals g_Globals;

NTSTATUS DriverEntry(PDRIVER_OBJECT DriverObject, PUNICODE_STRING RegistryPath)
{
	UNREFERENCED_PARAMETER(RegistryPath);
	KdPrint((DRIVER_PREFIX "DriverEntry called\n"));
	// Set up the driver unload routine	
	DriverObject->DriverUnload = DriverUnload;
	DriverObject->MajorFunction[IRP_MJ_CREATE] =
		DriverObject->MajorFunction[IRP_MJ_CLOSE] = ProcessProtectorCreateClose;
	DriverObject->MajorFunction[IRP_MJ_DEVICE_CONTROL] = ProcessProtectorIoControl;
	// Additional initialization code can go here
	OB_OPERATION_REGISTRATION obRegistration = {
		PsProcessType,
		OB_OPERATION_HANDLE_CREATE | OB_OPERATION_HANDLE_DUPLICATE,
		OnPreOpenProcess,
		NULL
	};
	OB_CALLBACK_REGISTRATION obCallbackRegistration = {
		OB_FLT_REGISTRATION_VERSION,
		1
	};

	obCallbackRegistration.OperationRegistration = &obRegistration;
	RtlInitUnicodeString(&obCallbackRegistration.Altitude, L"12345.6171");

	PVOID regHandle;
	ObRegisterCallbacks(&obCallbackRegistration, &regHandle); // Placeholder for actual callback registration

	return STATUS_SUCCESS;
}

VOID DriverUnload(PDRIVER_OBJECT DriverObject)
{
	UNREFERENCED_PARAMETER(DriverObject);
	KdPrint((DRIVER_PREFIX "DriverUnload called\n"));
	// Cleanup code can go here
}

NTSTATUS ProcessProtectorCreateClose(PDEVICE_OBJECT DeviceObject, PIRP Irp)
{
	UNREFERENCED_PARAMETER(DeviceObject);
	KdPrint((DRIVER_PREFIX "ProcessProtectorCreateClose called\n"));
	Irp->IoStatus.Status = STATUS_SUCCESS;
	Irp->IoStatus.Information = 0;
	IoCompleteRequest(Irp, IO_NO_INCREMENT);
	return STATUS_SUCCESS;
}

NTSTATUS ProcessProtectorIoControl(PDEVICE_OBJECT DeviceObject, PIRP Irp)
{
	UNREFERENCED_PARAMETER(DeviceObject);
	KdPrint((DRIVER_PREFIX "ProcessProtectorIoControl called\n"));
	switch (IoGetCurrentIrpStackLocation(Irp)->Parameters.DeviceIoControl.IoControlCode)
	{
	case IOCTL_PROCESS_PROTECT_BY_PID: {
		KdPrint((DRIVER_PREFIX "IOCTL_PROCESS_PROTECT_BY_PID called\n"));
		break;
	}
	case IOCTL_PROCESS_UNPROTECT_BY_PID: {
		KdPrint((DRIVER_PREFIX "IOCTL_PROCESS_UNPROTECT_BY_PID called\n"));
		break;
	}
	case IOCTL_PROCESS_PROTECT_CLEAR: {
		KdPrint((DRIVER_PREFIX "IOCTL_PROCESS_PROTECT_CLEAR called\n"));
		break;
	}
	default: {
		KdPrint((DRIVER_PREFIX "Unknown IOCTL code\n"));
		break;
	}
	}
	Irp->IoStatus.Status = STATUS_SUCCESS;
	Irp->IoStatus.Information = 0;
	IoCompleteRequest(Irp, IO_NO_INCREMENT);
	return STATUS_SUCCESS;
}

OB_PREOP_CALLBACK_STATUS OnPreOpenProcess(PVOID RegistrationContext, POB_PRE_OPERATION_INFORMATION OperationInformation)
{
	UNREFERENCED_PARAMETER(RegistrationContext);
	UNREFERENCED_PARAMETER(OperationInformation);
	KdPrint((DRIVER_PREFIX "OnPreOpenProcess called\n"));
	// Here you would implement the logic to check if the process is protected
	// and deny access if necessary. For now, we just log the event.
	if (OperationInformation->ObjectType == PsProcessType)
	{
		PEPROCESS targetProcess = (PEPROCESS)OperationInformation->Object;
		HANDLE targetPid = PsGetProcessId(targetProcess);
		KdPrint((DRIVER_PREFIX "Attempt to open process with PID: %u\n", (ULONG)(ULONG_PTR)targetPid));
		// Implement your protection logic here

	}
	return OB_PREOP_SUCCESS; // Returning OB_PREOP_SUCCESS means we are not denying access
}