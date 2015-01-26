#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <ctype.h>

struct holeNode
{
        int holeSize;
		int startloc;
		int printed;
        struct holeNode *next;
};


struct processNode
{
   struct processNode *next;
   int startloc;
   int pid;
   int size;
   int isFree;
};


void MemoryManager();
int allocate();
int deallocate();

int has_initialized = 0;
int amount_of_processes = 0;
int amount_of_holes = 1;
void *managed_memory_start;
int currentProcess = 0;


int last_valid_address = 0;
void printMemoryState();

 struct holeNode *holeRoot; /* This will be the unchanging first holeNode */
 struct processNode *processRoot;
    /* This will point to each holeNode as it traverses the list */
    struct holeNode *holeConductor;  
    struct processNode *processConductor;  
 
  
   int memorySize;
   int allocatedBytes;
   int freeBytes;

int main(int argc, char *argv[])
{


 if(argc != 2)
   {
    fprintf(stderr, "Needs exactly 1 command line argument, file to lookup, number\n");
    return 1;
   }





  struct stat StatBufWrite; 

  if ((stat(argv[1],  &StatBufWrite)) == -1) 
   {
    exit(1);
   }
  FILE * fpWrite = fopen(argv[1], "r");
  int VECLENWrite = StatBufWrite.st_size+1;
  char bufferWrite[ VECLENWrite];
      
  fread(bufferWrite, VECLENWrite, 1, fpWrite);
     
    
	
	int i;
	bufferWrite[VECLENWrite-1] = '\0';
	for (i = 0; i < VECLENWrite; i++)
	 {
	 
	  if (isspace(bufferWrite[i])) 
		 bufferWrite[i] = '\0';
		
	 }
	 
   int wordcountWrite = 0;
   int lettercountWrite = 0;
   long c; //iterator
   char **mywordsWrite;
  
  
		 mywordsWrite = (char**) calloc( VECLENWrite, sizeof(char*));
      for ( c = 0; c < VECLENWrite; c++ )
      { mywordsWrite[c] = (char*) calloc(  50, sizeof(char));}
	     

   for (c = 0; c <  VECLENWrite; c++)
	 {
       if (bufferWrite[c] != '\0')
	   {
	    mywordsWrite[wordcountWrite][lettercountWrite] = bufferWrite[c];
	    lettercountWrite++;
         if((bufferWrite[c+1] == '\0') && (lettercountWrite <= 50))
          { 
		  	 wordcountWrite++;
		     lettercountWrite = 0;
		  }
	   }
	  }
	  
	  
	  //by this point we should have the file in a char**
	    memorySize = 0;
	   
	   int memManage =  atoi(mywordsWrite[0]);
	   MemoryManager(memManage);
	  printf("%d\n", memManage );
	  int q = 0;
	  for (q = 0; q < VECLENWrite; q++)
	  {
	  // if A
      if (mywordsWrite[q][0] == 65)
	  {
	    int bytes = atoi(mywordsWrite[++q]);
		int pid = atoi( mywordsWrite[++q]);
	    allocate(bytes, pid);
	 
	  
	  }
	  
	  
	  //if D
	  if (mywordsWrite[q][0] == 68)
	  {
		int pid = atoi( mywordsWrite[++q]);
	    deallocate(pid);
	  }
	 
	 
	  if (mywordsWrite[q][0] == 80)
	  {
	
	    printMemoryState();
	  }
       
	   
	  
	  
	  }
	  

}





void MemoryManager(int bytes)
{  



   
    //allocating space for the holeRoot   
    holeRoot = malloc(sizeof(struct holeNode));
 
    holeRoot->next = 0;
    holeRoot->holeSize = bytes;
	holeRoot->startloc = 0;
 
    holeConductor = holeRoot;
	
	
	  //allocating space for the processRoot   
    processRoot = malloc(sizeof(struct processNode));
 
    processRoot->next = 0;
	processRoot->isFree = 0;
   
 
    processConductor = processRoot;
 
/*
//variables for Hole
    holeList->next = malloc(10000) ;
    
	 holeList->holeSize = 0;
	
//variables for process
    processList->next = malloc(1000);
    processList->startloc = 0;
    processList->pid = 0;
	*/
// intialize memory with these many bytes.
memorySize = bytes; //remain constant
freeBytes = bytes; //decreases as we allocate, increases as we deallocate

}

int allocate(int bytes, int pid)
{ 

  
  //finding correct amount of space for holes
  int holesizeflag = 0;
  //traverse through all holes,
  //is there a hole big enough for the space desired
  holeConductor = holeRoot;
   //get to the end
      if ( holeConductor != 0 )  /* Makes sure there is a place to start */
    {
        while(holeConductor != 0)
        { 
           
			//of all the nodes, there has to be one 
			//that is greater than or equal to the bytes desires
			//if there is space for the bytes, set the flag
			//so in other words, we want the flag to be set
			if (holeConductor->holeSize >= bytes)
			  holesizeflag = 1; 
			  
			  holeConductor = holeConductor->next;
			
        }
    }
	//if there was never any space big enough to the the bytes
	//print message and return
	
	
	
	
	
	
	
	//all error checks
	 if (bytes > memorySize)
  {
  printf("A\n");
  printf("%d ", bytes);
  printf("%d\n", pid);
  printf("Error: Memory exceeds available free memory.\n");
  printf("%d\n", memorySize);
   
   return -1;
  }
	
	 else if (pid < currentProcess+1)
   {
   printf("A\n");
   printf("%d ", bytes);
   printf("%d\n", pid);
   printf("Error: Process already exists\n");
   return -1;
   }
	
	else if (holesizeflag == 0)
	{
	 printf("A\n");
     printf("%d ", bytes);
     printf("%d\n", pid);
	 printf("Error: Memory not available due to external ");
	 printf("fragmentation, can't allocate memory space to ");
     printf("process %d\n", pid);
	 return -1;
	}
   
  else if (pid > currentProcess+1)
   {
   printf("A\n");
   printf("%d ", bytes);
   printf("%d\n", pid);
   printf("Error: Process not in memory\n");
   return -1;
   }

  
  //if we are good
  

  
  
                amount_of_processes++;
			   currentProcess++;


	
	 processConductor = processRoot;
     int alreadyMallocd = 0;
	 if ( processConductor != 0 ) 
	  {
       while(processConductor->next != 0)
        {  
		  
           if (processConductor->isFree == 1)
		    {
		      processConductor->size = bytes;
		      processConductor->isFree = 0;
		      alreadyMallocd = 1;
			   processConductor->pid = pid;
			  
		   }
		   processConductor = processConductor->next;
        }
    }
	if (alreadyMallocd == 0)
	{
	           
	            /* Creates a ProcessNode at the end of the list */
                processConductor->next = malloc( sizeof(struct processNode) ); 
 
                /*checking that memory is allocated*/
                if ( processConductor == 0 )
                {
                  printf( "Out of memory" );
                  return 0;
                } 
 
                processConductor = processConductor->next;
                processConductor->size = bytes;
				processConductor->startloc = last_valid_address;
				processConductor->pid = pid;
				processConductor->isFree = 0;
    
 

                 //marking the last as NULL
                 processConductor->next = 0;
	
	}

  freeBytes -=  bytes;
  allocatedBytes += bytes;

  
  
  
  printf("A\n");
  printf("%d ", bytes);
  printf("%d\n", pid);
		 
		 
  
 //last ting is increase the start loc on the processlist
 last_valid_address += bytes;
 
 holeRoot->startloc = last_valid_address;
 holeRoot->holeSize = memorySize - last_valid_address;
 return 1;
}

int deallocate(int pid)
{ 
   
   
   if (pid > currentProcess+1)
   {
    printf("D\n");
   printf("%d\n", pid);
   printf("Error: Process not in memory\n");
   return -1;
   }
   
   
   //freeBytes +=  bytes;
   //allocatedBytes -= bytes;
   printf("D\n");
   printf("%d\n", pid);
   amount_of_processes--;
   
   holeConductor = holeRoot;
   //get to the end
      if ( holeConductor != 0 )  /* Makes sure there is a place to start */
    {
        while(holeConductor->next != 0)
        { 
		
		    
            holeConductor = holeConductor->next;
        }
    }
                   /* Creates a holeNode at the end of the list */
                holeConductor->next = malloc( sizeof(struct holeNode) ); 
                
                /*checking that memory is allocated*/
                if ( holeConductor == 0 )
                {
                  printf( "Out of memory" );
                  return 0;
                } 
 
                holeConductor = holeConductor->next;
                
				
				
				//holeConductor->holeSize = pid;
    
 
    //marking the last as NULL
    holeConductor->next = 0;
   
   
   	processConductor = processRoot;
    if ( processConductor != 0 )  /* Makes sure there is a place to start */
    {
        while(processConductor->next != 0)
        { 
           // printf(" [%d]-> \n",processConductor->holeSize);
            processConductor = processConductor->next;
			
			if (processConductor->pid == pid)
			  {
			    //before freeing, allocate the hole size
			    holeConductor->holeSize = processConductor->size;
				holeConductor->startloc = processConductor->startloc;
				freeBytes +=  processConductor->size;
                allocatedBytes -= processConductor->size;
				
				
			    processConductor->isFree = 1;
                processConductor->size = 0; 
			  }
        }
    }
   
   
//deallocate memory allocated to this process
  // return 1 if successful, -1 otherwise with an error message
amount_of_holes++;
return 1;
}


void printMemoryState()
{
  printf("P\n");
  printf("Memory size = %d bytes, ", memorySize);
  printf("allocated bytes = %d, ", allocatedBytes);
  printf("free = %d\n", freeBytes);
  
  printf("There're currently %d holes ", amount_of_holes);
  printf("and %d processes:\n", amount_of_processes);
  
    //holeConductor points to holeRoot again
    holeConductor = holeRoot;
	processConductor = processRoot;
    
	
	
	
	

	printf("Hole list:\n");
	int i = 0;
	 if ( holeConductor != 0 )  /* Makes sure there is a place to start */
    {
        while(holeConductor != 0)
        { 
		 i++;
		 printf("hole %d: ",i);
		 printf("start location=%d, ", holeConductor->startloc);
		 printf("size=%d\n", holeConductor->holeSize);
            holeConductor = holeConductor->next;
        }
    }
	 
	
	printf("Process list:\n");
	int x = 0;
	//processConductor = processConductor->next;
	 if ( processConductor != 0 )  
	 {
        while(processConductor->next != 0)
        {
		    //iterate
			processConductor = processConductor->next;
		    //printing
			if (processConductor->isFree == 0)
			{

		    x++;
			printf("process %d: ", x);
            printf("id=%d: ",processConductor->pid);
			printf("start location=%d, ",processConductor->startloc);
			printf("size=%d\n", processConductor->size);
            }
			
        }
	
    }
	
 // print out current state of memory 
  // Example: 
  // Memory size = 1024 bytes, allocated bytes = 24, free = 1000
  // There are currently 10 holes and 3 active process
  // Hole list:
  // hole 1: start location = 0, size = 202
  // ...
  // Process list:
  // process  id=34, start location=203, size=35
  // ...

}