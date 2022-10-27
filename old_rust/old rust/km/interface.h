typedef enum _request_codes
{
	request_base = 0x119,
	request_read = 0x129,
	request_write = 0x139,
	request_text = 0x163,
	request_rect = 0x173,
	request_success = 0x91a,
	request_unique = 0x92b,
}request_codes, *prequest_codes;

typedef struct _read_request
{
	uint32_t pid;
	uintptr_t address;
	void *buffer;
	size_t size;
} read_request, *pread_request;

typedef struct _rect_request
{
	uint32_t x;
	uint32_t y;
	uint32_t w;
	uint32_t h;
	uint8_t t;
	uint8_t r;
	uint8_t g;
	uint8_t b;
}rect_request, *prect_request;

typedef struct _text_request
{
	uint32_t x;
	uint32_t y;
	LPCWSTR str;
	uint8_t r;
	uint8_t g;
	uint8_t b;
}text_request, *ptext_request;

typedef struct _write_request {
	uint32_t pid;
	uintptr_t address;
	void *buffer;
	size_t size;
} write_request, *pwrite_request;

typedef struct _base_request {
	uint32_t pid;
	uintptr_t handle;
	WCHAR name[260];
} base_request, *pbase_request;

typedef struct _request_data
{
	uint32_t unique;
	request_codes code;
	void *data;
}request_data, *prequest_data;