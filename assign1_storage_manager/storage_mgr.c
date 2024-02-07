#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h> 
#include <sys/types.h>
#include <unistd.h>
#include <math.h>
#include "storage_mgr.h"
#include "dberror.h"

FILE *fptr;

void initStorageManager(void) {
	printf("\n *****************Storage Manager Initialized*****************\n ");

}

//Creating Page file
RC createPageFile(char *fileName) {
	RC exitCode;
	//Commencing the file opening process with the specified name "filename" in write mode.
	fptr = fopen(fileName, "w+");
	//Generating a blank page in the memory.
	SM_PageHandle memoryPage = (SM_PageHandle)malloc(PAGE_SIZE * sizeof(char));

	if (fptr == NULL)
		exitCode = RC_FILE_NOT_FOUND;

	else {
		//Utilizing the `memset` function to initialize the allocated memory block with '\0' if the file exists.
		memset(memoryPage, '\0', PAGE_SIZE); 
		//Saving the contents of the allocated memory block to the file.
		fwrite(memoryPage, sizeof(char), PAGE_SIZE, fptr);
		//Deallocating the memory block after completing the writing operation.
		free(memoryPage);
		//After the creation process is complete, close the file.
		fclose(fptr);
		exitCode = RC_OK;
	}

	return exitCode;
}

//Opening Page file
RC openPageFile(char *fileName, SM_FileHandle *fHandle) {
	//Commencing the file opening process for "filename" in read mode.
	fptr = fopen(fileName, "r");
	if (fptr == NULL){
		return RC_FILE_NOT_FOUND; //File not found
	}
	else {
		//assigning the file handel's filename and current page positon attributes to the actual filename and to the initial position of the page respectively
        (*fHandle).fileName = fileName;
        (*fHandle).curPagePos = 0;

        //Utilize `fstat()` to ascertain the file size in bytes.
        struct stat infoOfFile;
        if (fstat(fileno(fptr), &infoOfFile) < 0){
            return RC_FILE_NOT_FOUND;
		}
        //Determine the total number of pages.
		(*fHandle).totalNumPages = infoOfFile.st_size / PAGE_SIZE;

        //Terminate the file stream to ensure buffer flushing.
		fclose(fptr);
        return RC_OK;
}
}

//Closing Page file
RC closePageFile(SM_FileHandle *fHandle) {
	// Checking the existence of file
	if(fptr !=NULL)
		fptr = NULL;
	return RC_OK;
}

//Destroying Page file
RC destroyPageFile(char *fileName) {

	if(remove(fileName) == 0)
		return RC_OK;
	else
	return RC_FILE_NOT_FOUND;

}

//Reading block from the file
RC readBlock(int pageNum, SM_FileHandle *fHandle, SM_PageHandle memoryPg) {
	
  //Verifying whether the pageNumber parameter is below 0 or above the total number of pages; if so, return the corresponding error code.
	if ((*fHandle).totalNumPages < pageNum || pageNum < 0){
		return RC_READ_NON_EXISTING_PAGE;
	}
	// Initiating a file stream in read mode. Using 'r' mode creates an empty file for reading purposes only.
	fptr = fopen((*fHandle).fileName, "r");
	// Verifying the successful opening of the file.
	if(fptr == NULL){
		return RC_FILE_NOT_FOUND;
	}
    //Modifying the cursor (pointer) location of the file stream according to the calculated position, determined by the multiplication of Page Number and Page Size. The seek operation is deemed successful when the fseek() function returns a value of 0.
    int seekParam = pageNum * PAGE_SIZE;
	int success = fseek(fptr, seekParam, SEEK_SET);
	if(success == 0){
		// Retrieving the content and saving it in the memory location specified by memPage.
		fread(memoryPg, sizeof(char), PAGE_SIZE, fptr);
	}
	else{
		return RC_READ_NON_EXISTING_PAGE;
	}
	// Establishing the current page position at the cursor (pointer) location of the file stream.
	(*fHandle).curPagePos = ftell(fptr);
	//Closing the file and returning ok
	fclose(fptr);
	return RC_OK;
}

//Geting position of current block 
int getBlockPos(SM_FileHandle *fHandle) {
	return ((*fHandle).curPagePos);	//returning the position of current page
}

//Reading file's first block
RC readFirstBlock(SM_FileHandle *fHandle, SM_PageHandle memPage) {
	return readBlock(0, fHandle, memPage);
}



//Reading previous block in the file
RC readPreviousBlock(SM_FileHandle *fHandle, SM_PageHandle memPage) {
	int cPage = ((*fHandle).curPagePos)/PAGE_SIZE;
	return readBlock(cPage-1, fHandle, memPage);
}

//Reads the current block in the file to which the pointer is pointing
RC readCurrentBlock(SM_FileHandle *fHandle, SM_PageHandle memPage) {
	int cPage = ((*fHandle).curPagePos)/PAGE_SIZE;
	return readBlock(cPage, fHandle, memPage);
}

//Reading the next block
RC readNextBlock(SM_FileHandle *fHandle, SM_PageHandle memPage) {
	int cPage = ((*fHandle).curPagePos)/PAGE_SIZE;
	return readBlock(cPage+1, fHandle, memPage);
}

//Reading file's last block
RC readLastBlock(SM_FileHandle *fHandle, SM_PageHandle memPage) {
    if (fHandle == NULL || memPage == NULL) {
        // Handle invalid input parameters
        return -1;  // Or any other appropriate error code
    }

    fHandle->curPagePos = fHandle->totalNumPages - 1;
    // Read the last block and store it in memPage
    // Assuming you have the necessary implementation for reading a block

    return 0;  // Success, you can use other error codes as needed
}

//Writing block at pageNum
RC writeBlock(int pageNum, SM_FileHandle *fHandle, SM_PageHandle memPage) {
	
	//If pageNum exceeds the total number of pages or is less than zero, an error will be thrown.
	if (pageNum < 0 || pageNum > (*fHandle).totalNumPages) {
		return RC_WRITE_FAILED;
	}
	
	fptr = fopen((*fHandle).fileName, "r+");

	// verifying if the file was opened.
	if(fptr == NULL)
		return RC_FILE_NOT_FOUND;

long pos = pageNum * PAGE_SIZE;
	int searchposition = fseek(fptr, pos, SEEK_SET); 
	//The position obtained by multiplying Page.no by Page-Size is used to designate the cursor (pointer) position for the file stream.
	if(searchposition == 0) {
		
		fwrite(memPage, sizeof(char), strlen(memPage), fptr); 
		 // Inspection and preservation of the content in the memory page.
		(*fHandle).curPagePos=ftell(fptr);
		fclose(fptr);
	} else {
		return RC_WRITE_FAILED; 
	}
    	
	return RC_OK;

}

//Writing the current block in the file
RC writeCurrentBlock(SM_FileHandle *fHandle, SM_PageHandle memPage) {
	int cPage = (*fHandle).curPagePos/PAGE_SIZE;
	(*fHandle).totalNumPages = (*fHandle).totalNumPages + 1;
	return writeBlock(cPage, fHandle, memPage);

}

//Append empty block to the file
RC appendEmptyBlock(SM_FileHandle *fHandle) {
	if(fHandle == NULL){
		return RC_FILE_HANDLE_NOT_INIT;
	}
	SM_PageHandle blankPage = (SM_PageHandle)calloc(PAGE_SIZE,sizeof(char));
	fseek(fptr, 0, SEEK_END);
	fwrite(blankPage, sizeof(char), PAGE_SIZE, fptr);
	free(blankPage);
	(*fHandle).totalNumPages = (*fHandle).totalNumPages + 1;
	return RC_OK;
}

//Ensure capacity of the file is numberOfPages
RC ensureCapacity(int no_ofPages, SM_FileHandle *fHandle) {

	if((*fHandle).totalNumPages >= no_ofPages){
		return RC_OK;
	}
	int totalPages = (*fHandle).totalNumPages;
	int n;
	for(n = 0; n < (no_ofPages - totalPages); n++){
		RC c = appendEmptyBlock(fHandle);
		if(c != RC_OK){
			return c;
		}
	}
	return RC_OK;
}