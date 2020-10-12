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

#define MMAP_SIZE   4096
#define BUFFER_SIZE 100
#define PAYLOAD_SIZE 34

using namespace std;

 typedef struct {
     int item_no;          /* number of the item produced */
     unsigned short cksum;    /* 16-bit Internet checksum    */
     char payload[PAYLOAD_SIZE];  /* random generated data */
} item;

int main(int argc, char const *argv[]) {
  /* code */
  //check that argv has 1 argument
  if(argc != 2){
    cout << "Not proper number of arguements" << endl;
    return 1;
  }
  /* name of the shared memory object */
  const char* name = argv[1]; //set name of shared memory to argument

  item   buffer[BUFFER_SIZE];
  int    in  = 0;
  int    out = 0;


  item next_produced;

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
  ptr = mmap(0, MMAP_SIZE, PROT_WRITE, MAP_SHARED, shm_fd, 0);


  int buffCount = 0;

  while (true) {
    cout << "Producer is working" << endl;
    buffCount++;
    //update item numebr
    /* produce an item in next_produced */
    /* 1. Increment the buffer count (item_no)  */
    next_produced.item_no = buffCount;
    /* 3. Generate the payload data   */
    for(int i = 0; i < PAYLOAD_SIZE; ++i){
      //256 available characters
      next_produced.payload[i] = (unsigned char)rand() % 256;
    }

    /* 2. Calculate the 16-bit checksum (cksum) */

    unsigned short check = ip_checksum(next_produced.payload, PAYLOAD_SIZE);
    next_produced.cksum = check;
    //cksum = bsdChecksumFromFile(name)

    //write message to shared memory
    //cout << payload << endl;
    // printf(ptr, "%s", message_0);

    // ptr += strlen(message_0);
    // cout << "Wrote to shared memory" << endl
    // return 0;
  /*    next_produced.payload[n] = (unsigned char) rand() % 256 */


    while (((in + 1) % BUFFER_SIZE) == out){
      sleep(1);
    } /* do nothing but sleep for 1 second */
    buffer[in] = next_produced; /* store next_produced into shared
                                     buffer */
    in = (in + 1) % BUFFER_SIZE;

  }
  return 0;
}
