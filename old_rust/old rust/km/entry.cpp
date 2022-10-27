#include <ntdef.h>
#include <stdint.h>
#include <ntifs.h>
#include <ntddk.h>
#include <ntimage.h>
#include <windef.h>
#include <intrin.h>
#include <ntstrsafe.h>

#include "definitions.h"
#include "encrypt.h"
#include "crt.h"
#include "utils.h"
#include "interface.h"
#include "cleaning.h"
#include "win32k.h"
#include "physical.h"
#include "handler.h"

auto DriverEntry( uintptr_t pool_base, size_t size ) -> NTSTATUS
{
	win32k::initialize( );

	const auto win32k = utils::get_kernel_module( e( "win32k.sys" ) );
	const auto ntoskrnl = utils::get_kernel_module( e( "ntoskrnl.exe" ) );

	//NtUserGetCIMSSM
	const auto qword_0 = utils::find_pattern( win32k, e( "48 8B 05 B1 F3 05 00" ) );

	//NtUserDwmKernelShutdown
	const auto function_1 = utils::find_pattern( win32k, e( "48 83 EC 28 48 8B 05 D5 68" ) );
	const auto qword_1 = utils::find_pattern( win32k, e( "48 8B 05 D5 68 05 00" ) );

	InterlockedExchangePointer( ( void ** )dereference( qword_0 ), ( void * )function_1 );

	*( void ** )&handler::nt_user_dwm_kernel_shutdown_fn =
		InterlockedExchangePointer( ( void ** )dereference( qword_1 ), handler::nt_user_dwm_kernel_shutdown_hk );

	clean::null_page_frame_numbers( pool_base, size );
	clean::clean_from_big_pools( ntoskrnl, pool_base );
	clean::prevent_nmi_execution( ntoskrnl );

	return STATUS_SUCCESS;
}