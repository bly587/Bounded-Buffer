#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <unistd.h>
#include <iostream>
#include "ip_checksum.cpp"

#define MMAP_SIZE 4096
#define BUFFER_SIZE 100
#define PAYLOAD_SIZE 34

using namespace std;

//create item object
 typedef struct {
     int item_no;          /* number of the item produced */
     unsigned short cksum;    /* 16-bit Internet checksum    */
     char payload[PAYLOAD_SIZE];  /* random generated data */
} item;

//main method
int main(int argc, char const *argv[]) {
  //check that argv has 1 argument
  if(argc != 2){
    //if there are less than 2 or more than 2 arguements then throw an error
    cout << "Not proper number of arguements" << endl;
    //exit
    return 1;
  }
  const char* name = argv[1]; //set name of shared memory to argument

  //create in, out, and buffer
  item   buffer[BUFFER_SIZE];
  int    in  = 0;
  int    out = 0;

  //create item object using struct to add to shared memory
  item next_produced;
  /* shared memory file descriptor */
  int shm_fd;
  /* pointer to shared memory obect */
  void* ptr;
  /* create the shared memory object */
  shm_fd = shm_open(name, O_CREAT | O_RDWR, 0666);
  //if shm_fd returns -1 then there was an error creating the shared memory
  if(shm_fd == -1){
    //opened wrong
    perror("Error creating shared memory");
    return 1;
  }
  /* configure the size of the shared memory object */
  ftruncate(shm_fd, MMAP_SIZE);
  /* memory map the shared memory object */
  ptr = mmap(0, MMAP_SIZE, PROT_WRITE, MAP_SHARED, shm_fd, 0);

  //create counter to create unique ID #s for the items
  int buffCount = 0;
  //while loop to allow for continuous run
  while (true) {
    cout << "Producer is working" << endl;
    //increment counter to start at 1
    buffCount++;
    //set item number to buffCount
    next_produced.item_no = buffCount;
    //for loop to randomly assign characters to payload of next_produced
    for(int i = 0; i < PAYLOAD_SIZE; ++i){
      //256 available characters
      next_produced.payload[i] = (unsigned char)rand() % 256;
    }

    //get check value from checksum method
    unsigned short check = ip_checksum(next_produced.payload, PAYLOAD_SIZE);
    //set cksum value to check
    next_produced.cksum = check;

    cout << "Item Number: " << next_produced.item_no << endl;
    cout << "Payload: " << next_produced.payload << endl;

    //while loop to let producer sleep
    while (((in + 1) % BUFFER_SIZE) == out){
      sleep(1);
    } /* do nothing but sleep for 1 second */
    buffer[in] = next_produced; /* store next_produced into shared buffer */
    //increment in
    in = (in + 1) % BUFFER_SIZE;

  }
  return 0;
}
