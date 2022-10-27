namespace handler
{
	__int64( __fastcall *nt_user_dwm_kernel_shutdown_fn )( void * ) = nullptr;

	__int64 __fastcall nt_user_dwm_kernel_shutdown_hk( void *rcx )
	{
		if ( !rcx || ExGetPreviousMode( ) != UserMode || reinterpret_cast< request_data * >( rcx )->unique != request_unique )
		{
			return nt_user_dwm_kernel_shutdown_fn( rcx );
		}

		DbgPrintEx( 0, 0, "received" );

		const auto request = reinterpret_cast< request_data * >( rcx );

		switch ( request->code )
		{
		case request_text:
		{
			text_request data { 0 };

			if ( !utils::safe_copy( &data, request->data, sizeof( text_request ) ) )
			{
				return 1;
			}

			if ( !win32k::draw_text( data.x, data.y, data.str, data.r, data.g, data.b ) )
			{
				return 1;
			}

			return request_success;
		}
		case request_rect:
		{
			rect_request data { 0 };

			if ( !utils::safe_copy( &data, request->data, sizeof( rect_request ) ) )
			{
				return 1;
			}

			if ( !win32k::draw_rect( data.x, data.y, data.w, data.h, data.t, data.r, data.g, data.b ) )
			{
				return 1;
			}

			return request_success;
		}
		case request_base:
		{
			base_request data { 0 };

			if ( !utils::safe_copy( &data, request->data, sizeof( base_request ) ) )
			{
				return 1;
			}

			if ( !data.name || !data.pid )
			{
				return 1;
			}

			const auto base = utils::get_module_handle( data.pid, data.name );

			if ( !base )
			{
				return 1;
			}

			reinterpret_cast< base_request * > ( request->data )->handle = base;

			return request_success;
		}
		case request_write:
		{
			write_request data { 0 };

			if ( !utils::safe_copy( &data, request->data, sizeof( write_request ) ) )
			{
				return 1;
			}

			if ( data.address >= 0x7FFFFFFFFFFF || data.address <= 0 || !data.pid || !data.buffer || !data.size )
			{
				return 1;
			}

			PEPROCESS process;
			if ( PsLookupProcessByProcessId( ( HANDLE )data.pid, &process ) == STATUS_SUCCESS )
			{
				NTSTATUS status = STATUS_UNSUCCESSFUL;

				const auto process_dirbase = physical::get_process_cr3( process );

				ObDereferenceObject( process );

				size_t cur_offset = 0;
				size_t total_size = data.size;

				while ( total_size )
				{
					int64_t cur_phys_addr = physical::translate_linear_address( process_dirbase, data.address + cur_offset );
					
					if ( !cur_phys_addr )
					{
						return STATUS_UNSUCCESSFUL;
					}

					SIZE_T bytes_written = 0;

					status = physical::write_physical_address( PVOID( cur_phys_addr ), ( void* )( ( uintptr_t )data.buffer + cur_offset ), min( PAGE_SIZE - ( cur_phys_addr & 0xFFF ), total_size ), &bytes_written );
					
					total_size -= bytes_written;
					cur_offset += bytes_written;
					
					if ( status != STATUS_SUCCESS || bytes_written == 0 )
					{
						break;
					}
				}
			}
			else
			{
				return 1;
			}

			return request_success;
		}
		case request_read:
		{
			read_request data { 0 };

			if ( !utils::safe_copy( &data, request->data, sizeof( read_request ) ) )
			{
				return 1;
			}

			if ( data.address >= 0x7FFFFFFFFFFF || data.address <= 0 || !data.pid || !data.buffer || !data.size )
			{
				return 1;
			}

			PEPROCESS process;
			if ( PsLookupProcessByProcessId( ( HANDLE )data.pid, &process ) == STATUS_SUCCESS )
			{
				NTSTATUS status = STATUS_UNSUCCESSFUL;

				const auto process_dirbase = physical::get_process_cr3( process );

				ObDereferenceObject( process );

				size_t cur_offset = 0;
				size_t total_size = data.size;

				while ( total_size )
				{

					int64_t cur_phys_addr = physical::translate_linear_address( process_dirbase, data.address + cur_offset );
					
					if ( !cur_phys_addr )
					{
						return STATUS_UNSUCCESSFUL;
					}

					size_t bytes_read = 0;
					
					status = physical::read_physical_address( PVOID( cur_phys_addr ), ( void* )( ( uintptr_t )data.buffer + cur_offset ), min( PAGE_SIZE - ( cur_phys_addr & 0xFFF ), total_size ), &bytes_read );
					
					total_size -= bytes_read;
					cur_offset += bytes_read;
					
					if ( status != STATUS_SUCCESS || bytes_read == 0 )
					{
						break;
					}
				}
			}
			else
			{
				return 1;
			}

			return request_success;
		}

		default:
			break;
		}

		return 1;
	}
}