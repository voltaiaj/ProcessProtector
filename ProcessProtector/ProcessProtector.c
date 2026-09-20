#include <ntifs.h>
#include "ProcessProtectorCommon.h"

#define DRIVER_PREFIX "ProcessProtector: "

VOID DriverUnload(PDRIVER_OBJECT);
NTSTATUS ProcessProtectorCreateClose(PDEVICE_OBJECT, PIRP);
NTSTATUS ProcessProtectorIoControl(PDEVICE_OBJECT, PIRP);

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