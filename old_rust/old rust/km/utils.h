namespace utils
{
    auto swap_process( PEPROCESS new_process ) -> PEPROCESS
    {
        auto apc_state = *reinterpret_cast< uintptr_t * >( reinterpret_cast< uintptr_t >( KeGetCurrentThread( ) )+ 0x98 );
        auto old_process = *reinterpret_cast< uintptr_t * >( apc_state + 0x20 );
        
        *reinterpret_cast< uintptr_t * >( apc_state + 0x20 ) = reinterpret_cast< uintptr_t>( new_process );

        auto dir_table_base = *reinterpret_cast< uintptr_t* > ( reinterpret_cast< uintptr_t >( new_process ) + 0x28 );
        
        __writecr3( dir_table_base );

        return reinterpret_cast< PEPROCESS > ( old_process );
    }

    template <typename t>
    auto get_module_export( const uintptr_t base, const char *name )
    {
        return reinterpret_cast< t >( RtlFindExportedRoutineByName( reinterpret_cast< void * >( base ), name ) );
    }

    auto get_system_information( SYSTEM_INFORMATION_CLASS information_class ) ->  void *
    {
        unsigned long size = 32;
        char buffer[32];

        ZwQuerySystemInformation( information_class, buffer, size, &size );

        void *info = ExAllocatePoolZero( NonPagedPool, size, 7265746172 );

        if ( !info )
            return nullptr;

        if ( !NT_SUCCESS( ZwQuerySystemInformation( information_class, info, size, &size ) ) )
        {
            ExFreePool( info );
            return nullptr;
        }

        return info;
    }

    auto get_kernel_module( const char *name ) -> uintptr_t
    {
        const PRTL_PROCESS_MODULES info = ( PRTL_PROCESS_MODULES )get_system_information( SystemModuleInformation );

        if ( !info )
        {
            return 0;
        }

        for ( size_t i = 0; i < info->NumberOfModules; ++i )
        {
            const auto &mod = info->Modules[i];

            if ( crt::strcmp( to_lower( ( char * )mod.FullPathName + mod.OffsetToFileName ), name ) == 0 )
            {
                const void *address = mod.ImageBase;
                ExFreePool( info );
                return ( uintptr_t )address;
            }
        }

        ExFreePool( info );

        return 0;
    }

    auto find_pattern( uintptr_t base, size_t range, const char *pattern, const char *mask ) -> uintptr_t
    {
        const auto check_mask = []( const char *base, const char *pattern, const char *mask ) -> bool
        {
            for ( ; *mask; ++base, ++pattern, ++mask )
            {
                if ( *mask == 'x' && *base != *pattern )
                {
                    return false;
                }
            }

            return true;
        };

        range = range - crt::strlen( mask );

        for ( size_t i = 0; i < range; ++i )
        {
            if ( check_mask( ( const char * )base + i, pattern, mask ) )
            {
                return base + i;
            }
        }

        return NULL;
    }

    auto find_pattern( uintptr_t base, const char *pattern, const char *mask ) -> uintptr_t
    {
        const PIMAGE_NT_HEADERS headers = ( PIMAGE_NT_HEADERS )( base + ( ( PIMAGE_DOS_HEADER )base )->e_lfanew );
        const PIMAGE_SECTION_HEADER sections = IMAGE_FIRST_SECTION( headers );

        for ( size_t i = 0; i < headers->FileHeader.NumberOfSections; i++ )
        {
            const PIMAGE_SECTION_HEADER section = &sections[i];

            if ( section->Characteristics & IMAGE_SCN_MEM_EXECUTE )
            {
                const auto match = find_pattern( base + section->VirtualAddress, section->Misc.VirtualSize, pattern, mask );

                if ( match )
                {
                    return match;
                }
            }
        }

        return 0;
    }

    auto find_pattern( uintptr_t module_base, const char* pattern ) -> uintptr_t
    {
        auto pattern_ = pattern;
        uintptr_t first_match = 0;

        if ( !module_base )
        {
            return 0;
        }

        const auto nt = reinterpret_cast< IMAGE_NT_HEADERS * >( module_base + reinterpret_cast< IMAGE_DOS_HEADER * >( module_base )->e_lfanew );

        for ( uintptr_t current = module_base; current < module_base + nt->OptionalHeader.SizeOfImage; current++ )
        {
            if ( !*pattern_ )
            {
                return first_match;
            }

            if ( *( BYTE * )pattern_ == '\?' || *( BYTE * )current == get_byte( pattern_ ) )
            {
                if ( !first_match )
                    first_match = current;

                if ( !pattern_[2] )
                    return first_match;

                if ( *( WORD * )pattern_ == '\?\?' || *( BYTE * )pattern_ != '\?' )
                    pattern_ += 3;

                else
                    pattern_ += 2;
            }
            else
            {
                pattern_ = pattern;
                first_match = 0;
            }
        }

        return 0;
    }

    auto get_module_handle( uintptr_t pid, LPCWSTR module_name ) -> uintptr_t
    {
        PEPROCESS process;
        uintptr_t base = 0;
        
        if ( PsLookupProcessByProcessId( ( HANDLE )pid, &process ) != STATUS_SUCCESS )
        {
            return 0;
        }

        const auto o_process = swap_process( process );

        const auto peb = PsGetProcessPeb( process );
       
        if ( !peb )
        {
            goto end;
        }

        if ( !peb->Ldr || !peb->Ldr->Initialized )
        {
            goto end;
        }

        UNICODE_STRING module_name_unicode;
        RtlInitUnicodeString( &module_name_unicode, module_name );

        for ( LIST_ENTRY* list = peb->Ldr->InLoadOrderModuleList.Flink; list != &peb->Ldr->InLoadOrderModuleList; list = list->Flink )
        {
            LDR_DATA_TABLE_ENTRY *entry = CONTAINING_RECORD( list, LDR_DATA_TABLE_ENTRY, InLoadOrderLinks );
            if ( RtlCompareUnicodeString( &entry->BaseDllName, &module_name_unicode, TRUE ) == 0 ) 
            {
                base = ( uintptr_t )entry->DllBase;
                goto end;
            }
        }

    end:
        swap_process( o_process );

        ObDereferenceObject( process );

        return base;
    }

    auto safe_copy( void* dst, void *src, size_t size ) -> bool
    {
        SIZE_T bytes = 0;

        if ( MmCopyVirtualMemory( IoGetCurrentProcess( ), src, IoGetCurrentProcess( ), dst, size, KernelMode, &bytes ) == STATUS_SUCCESS && bytes == size )
        {
            return true;
        }

        return false;
    }
}