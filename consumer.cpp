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

 typedef struct {
     int item_no;          /* number of the item produced */
     unsigned short cksum;    /* 16-bit Internet checksum    */
     char payload[PAYLOAD_SIZE];  /* random generated data */
} item;

int main(int argc, char const *argv[]) {
  //check that argv has 1 argument
  if(argc != 2){
    cout << "Not proper number of arguements" << endl;
    return 1;
  }
  /* name of the shared memory object */
  const char* name = argv[1]; //set name of shared memory to argument
  /* shared memory file descriptor */
  int shm_fd;
  /* pointer to shared memory obect */
  void* ptr;
  /* create the shared memory object */
  shm_fd = shm_open(name, O_CREAT | O_RDWR, 0666);
  if(shm_fd == -1){
    //opened wrong
    perror("Error creating shared memory");
    return 1;
  }
  /* configure the size of the shared memory object */
  ftruncate(shm_fd, MMAP_SIZE);
  /* memory map the shared memory object */
  ptr = mmap(0, MMAP_SIZE, PROT_READ, MAP_SHARED, shm_fd, 0);

  item   buffer[BUFFER_SIZE];
  int    in  = 0;
  int    out = 0;

  item next_consumed;
  int num_counter = 0;
  //buffer = (unsigned char *)ptr;

  //cout << "This is the value we got: " +(char*)buffer << endl;


  while (true) {
      while (in == out){
        sleep(1); /* do nothing but sleep for 1 second */
        in++;
      }
      next_consumed = buffer[out];

      //check for no skipped buffers
      if (next_consumed.item_no - num_counter != 1){
        perror("Reading out of order");
        return 1;
      }

      //run checksum again to check if they are equal
      int new_check = ip_checksum(next_consumed.payload, PAYLOAD_SIZE);
      if(new_check == next_consumed.cksum){
        cout << "Error: Checksums aren't the same" << endl;
        return 1;
      }

      out = (out + 1) % BUFFER_SIZE;

      /* consume the item in next_consumed */
      /* 1. Check for no skipped buffers (item_no is contiguous) */
      /* 2. Verify the calculated checksum matches what is stored in
            next_consumed */
      num_counter++;
  }
  return 0;
}
