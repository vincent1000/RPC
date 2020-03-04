// rpc_cplus.cpp 
//
#include <iostream>
#include <Winsock2.h>
#include <stdio.h>
#include <string>
#include <vector>
#include <Sddl.h>
#include <rpcdce.h>
#include "hello_h.h"
using namespace std;

#pragma comment(lib, "rpcrt4.lib")
extern "C" {
	void Hello(handle_t IDL_handle, const unsigned char* psz) {
		printf("server:%s\n", psz);
	}
}

void* __RPC_USER MIDL_user_allocate(size_t len)
{
	return (malloc(len));
}

void __RPC_USER MIDL_user_free(void* ptr)
{
	free(ptr);
}

inline ULONG BOOL_TO_ERROR(BOOL f)
{
	return f ? NOERROR : GetLastError();
}

ULONG InitRpcServer()
{
	PSECURITY_DESCRIPTOR SecurityDescriptor;

	// generic all for SDDL_ALL_APP_PACKAGES + SDDL_EVERYONE

	ULONG dwError = BOOL_TO_ERROR(ConvertStringSecurityDescriptorToSecurityDescriptorW(
		L"D:P(A;;GA;;;WD)(A;;GA;;;AC)(A;;GA;;;S-1-15-2-2)S:(ML;;;;;LW)", SDDL_REVISION, &SecurityDescriptor, 0));

	if (dwError == ERROR_SUCCESS)
	{
		dwError = RpcServerRegisterIf3(hello_v1_0_s_ifspec,
			NULL, NULL, RPC_IF_ALLOW_CALLBACKS_WITH_NO_AUTH,
			RPC_C_LISTEN_MAX_CALLS_DEFAULT, 0x10000, 0, SecurityDescriptor);

		if (dwError == RPC_S_OK)
		{
			dwError = RpcServerUseProtseqEpW(
				(RPC_WSTR)L"ncalrpc",
				RPC_C_PROTSEQ_MAX_REQS_DEFAULT,
				(RPC_WSTR)L"myname",
				SecurityDescriptor);

			if (dwError == RPC_S_OK)
			{
				dwError = RpcServerListen(1, RPC_C_LISTEN_MAX_CALLS_DEFAULT, false);
				std::cout << "Client is comming!\n";
			}
		}

		LocalFree(SecurityDescriptor);
	}

	return dwError;
}

int main()
{
	InitRpcServer();
    std::cout << "Hello World!\n";
}

