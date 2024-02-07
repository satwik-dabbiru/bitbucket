RUNNING THE SCRIPT:

1) Navigate to the Project root (assign1) using Terminal.

2) execute "make clean" to delete old .o files if any were conplied earlier.

3) execute "make" to compile all the storage manager files consisting of  "test_assign1_1.c" file too

4) Finally execute test1.exe and execute the application in the terminal



FUNCTIONS DESCRIPTION:


Once the file is created including the File management functions i.e. create, open and close files , we can use these methods for the storage managers functionality.

initStorageManager( ) --> This function initializes the storage manager.

createPageFile( ) --> The createPageFile function is designed to create a new page file with a specified name and initialize it with a blank page in memory. 

Fopen() Opens or creates a file with the specified name in write mode ("w+").
malloc() Allocates memory for a blank page.
memset() Initializes the allocated memory block with null characters.
fwrite() Writes the contents of the memory block to the file.
Frees the allocated memory block.
fclose() Closes the file after the writing operation.
If there has been an issue accessing the file, we return RC_FILE_NOT_FOUND; or we return RC_OK.

openPageFile( ) --> we an existing file with the specified name in read mode ("r") using fopen().
later we Assign the file handle's filename and current page position attributes to the actual filename and the initial position of the page, respectively.
Utilizing fstat() we determine the file size in bytes.
following that we Compute the total number of pages based on the file size and page size.
finally we close the file stream to ensure buffer flushing and will 
Return an appropriate RC code based on the success or failure of the file opening process.

closePageFile( ) --> The pointer to the page file will be set to NULL.

destroyPageFile( ) --> The file is erased from memory using the remove() function.



Multiple pieces of data are read from the page file onto the disc (memory) using the read-related functions. 
The C functions such as fread(), ftell() and fseek() are utilized to construct them.

readBlock( ) --> The purpose of the readBlock function is to read a particular block, or page, from an open page file and put its contents in the memory page that is supplied.

It checks to see if the pageNum argument falls into the allowed range, which is 0 to the total number of pages minus 1 then opens the file in read-only mode and verifies that it opened successfully.
It uses fseek() to position the cursor (pointer) in the file stream at the start of the designated block.
reads the block's contents from the file and saves them on the memory page that is specified.changes the file handle's current page position later shuts off the file stream.
Depending on whether the read operation was successful or unsuccessful, returns the relevant RC code.

getBlockPos( ) --> The function obtains the current page position from FileHandle's curPagePos and returns it.


readFirstBlock( ) --> By passing 0 as the pageNum argument, we invoke the readBlock() function.

readPreviousBlock( ) --> By passing the pageNum argument as (curPagePos - 1), we invoke the readBlock() function.

readCurrentBlock( ) --> By passing the pageNum argument as (curPagePos), we invoke the readBlock() function.

readNextBlock( ) --> By passing the pageNum argument as (curPagePos + 1), we invoke the readBlock() function.

readLastBlock( ) --> By passing the pageNum argument as (totalNumPages - 1), we invoke the readBlock() function.


The write related functions are used to write blocks of data from the disk (memory) to the page file.
C functions - fseek(..) and fwrite(..) are used.

writeBlock( )
--> The writeBlock function uses an open page file and writes the contents of a memory page to a designated block (page).

It checks to see if the pageNum argument falls into the allowed range, which is 0 to the total number of pages minus 1.
then opens the file in read/write mode and verifies that it was opened successfully.
Afterwards, it uses fseek() to position the cursor (pointer) in the file stream at the start of the designated block before using fwrite() to write the contents of the memory page to the file.
Lastly, it modifies the file handle's current page position before continuing. shuts off the file stream.



writeCurrentBlock( )--> We call writeBlock( ) We invoke the writeBlock() function, passing in pageNum = current page position as an argument and increment the total no of pages

appendEmptyBlock( ) --> The appendEmptyBlock function appends an empty block (page) to the end of an open page file. This function is crucial for dynamically extending the file size and accommodating additional data within a larger data management system.
 
Initially Allocates memory for an empty page using calloc. It Positions the cursor (pointer) at the end of the file using fseek.
Writes the contents of the empty page to the file, effectively appending an empty block. it uses free()  to Free the allocated memory for the empty page. Updates the total number of pages in the file handle.

Returns an appropriate RC code based on the success or failure of the append operation.


ensureCapacity( ) --> We are determining if the number of required pages is greater than the total number of pages, indicating the need for additional pages.
This means that we check whether the number of pages needed exceeds the current total, which would require adding more pages.

MakeFile was made ensuring proper memory management while also making this storage manager by closing the file stream whenever required and freeing any reserved space.

