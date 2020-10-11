#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/shm.h>
#include <sys/stat.h>

#define MMAP_SIZE   4096
#define BUFFER_SIZE 100
#define PAYLOAD_SIZE 34

 typedef struct {
     int item_no;          /* number of the item produced */
     unsigned short cksum;    /* 16-bit Internet checksum    */
     unsigned char payload[PAYLOAD_SIZE];  /* random generated data */
} item;

item   buffer[BUFFER_SIZE];
int    in  = 0;
int    out = 0;


item next_produced;

while (true) {
  /* produce an item in next_produced */
  //get a the POSIX file
  /* name of the shared memory object */
  const char* name = "OS";
  /* shared memory file descriptor */
  int shm_fd;

  /* pointer to shared memory obect */
  void* ptr;

  /* create the shared memory object */
  shm_fd = shm_open(name, O_CREAT | O_RDWR, 0666);

  /* configure the size of the shared memory object */
  ftruncate(shm_fd, MMAP_SIZE);

  /* memory map the shared memory object */
  ptr = mmap(0, SIZE, PROT_WRITE, MAP_SHARED, shm_fd, 0);
  /* 1. Increment the buffer count (item_no)  */
  item_no += 1
  /* 2. Calculate the 16-bit checksum (cksum) */
  //cksum = bsdChecksumFromFile(name)
  /* 3. Generate the payload data             */
  for(int i = 0; i < sizeof(payload); ++i){
    payload[i] = "a"
  }
  //write message to shared memory
  cout << payload << endl;
  // printf(ptr, "%s", message_0);

  ptr += strlen(message_0);
  cout << "Wrote to shared memory" << endl
  return 0;
/*    next_produced.payload[n] = (unsigned char) rand() % 256 */


    // while (((in + 1) % BUFFER_SIZE) == out)
    //     sleep(1); /* do nothing but sleep for 1 second */
    // buffer[in] = next_produced; /* store next_produced into shared
    //                                buffer */
    // in = (in + 1) % BUFFER_SIZE;
}


//checksum method
int bsdChecksumFromFile(FILE *fp) /* The file handle for input data */
{
    int checksum = 0;             /* The checksum mod 2^16. */

    for (int ch = getc(fp); ch != EOF; ch = getc(fp)) {
        checksum = (checksum >> 1) + ((checksum & 1) << 15);
        checksum += ch;
        checksum &= 0xffff;       /* Keep it within bounds. */
    }
    return checksum;
}
