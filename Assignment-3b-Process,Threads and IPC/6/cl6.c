#include <stdio.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/stat.h>
#include <time.h>
#include<unistd.h>
#include <sys/wait.h>
#include <string.h>

void printFileProperties(struct stat stats);

struct msg_buffer {
   long msg_type;
   char msg[100];
   struct stat stats;
} message;
int main() {
   key_t my_key;
   int msg_id;
   my_key = ftok("progfile", 65);
   msg_id = msgget(my_key, 0666 | IPC_CREAT);
   message.msg_type = 1;
   printf("Enter File path: ");
   fgets(message.msg, 100, stdin);
   msgsnd(msg_id, &message, sizeof(message), 0);
   msgrcv(msg_id, &message, sizeof(message), 1, 0);
   printFileProperties(message.stats);
   return 0;
}

void printFileProperties(struct stat stats)
{
    struct tm dt;

    printf("\nFile access: ");
    if (stats.st_mode & R_OK)
        printf("read ");
    if (stats.st_mode & W_OK)
        printf("write ");
    if (stats.st_mode & X_OK)
        printf("execute");

    printf("\nFile size: %ld", stats.st_size);

    dt = *(gmtime(&stats.st_ctime));
    printf("\nCreated on: %d-%d-%d %d:%d:%d", dt.tm_mday, dt.tm_mon, dt.tm_year + 1900,
                                              dt.tm_hour, dt.tm_min, dt.tm_sec);

    dt = *(gmtime(&stats.st_mtime));
    printf("\nModified on: %d-%d-%d %d:%d:%d \n", dt.tm_mday, dt.tm_mon, dt.tm_year + 1900,
                                              dt.tm_hour, dt.tm_min, dt.tm_sec);
