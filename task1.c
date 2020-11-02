#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>

pthread_t tid[2];

pthread_mutex_t lock;

typedef struct Student{
    char name[50];
    double GPA;
    int toeicScore;
    int tinChiTichLuy;
    int noHocPhi;
    char mail[50];
}SinhVien;

SinhVien nQHuy = {"Nguyen Quang Huy",3.0,0,50,2000000,"254huynguyen@gmail.com"};

void *updateInfoStudent(){
    pthread_mutex_lock(&lock);
    printf("ID thread: %ld\n",pthread_self());

    printf("Ho va ten: %s\n",nQHuy.name);
    printf("GPA: %0.2f\n",nQHuy.GPA);
    printf("Diem Toeic: %d\n",nQHuy.toeicScore);
    printf("Tin chi da tich luy: %d\n",nQHuy.tinChiTichLuy);
    printf("No hoc phi: %d\n",nQHuy.noHocPhi);
    printf("\nSuccessfully updated\n");
    //send mail
    char contentMail[]="Thong tin sinh vien da duoc cap nhat:\n";
    char _IDThread[100];
    char _Name[100];
    char _GPA[100];
    char _ToeicScore[100];
    char _TCTichLuy[100];
    char _NoHocPhi[100];
    sprintf(_IDThread,"ID thread: %ld\n",pthread_self());
    sprintf(_Name,"Ho va ten: %s\n",nQHuy.name);
    sprintf(_GPA,"GPA: %0.2f\n",nQHuy.GPA);
    sprintf(_ToeicScore,"Diem Toeic: %d\n",nQHuy.toeicScore);
    sprintf(_TCTichLuy,"Tin chi da tich luy: %d\n",nQHuy.tinChiTichLuy);
    sprintf(_NoHocPhi,"No hoc phi: %d",nQHuy.noHocPhi);
    
    char cmd[100];  // to hold the command.
    char tempFile[100];     // name of tempfile.
    
    strcpy(tempFile,tempnam("/tmp","sendmail")); // generate temp file name.

    FILE *fp = fopen(tempFile,"w"); // open it for writing.
    fprintf(fp,"%s\n",contentMail);        // write body to it.
    fprintf(fp,"%s\n",_IDThread); 
    fprintf(fp,"%s\n",_Name); 
    fprintf(fp,"%s\n",_GPA); 
    fprintf(fp,"%s\n",_ToeicScore); 
    fprintf(fp,"%s\n",_TCTichLuy); 
    fprintf(fp,"%s\n",_NoHocPhi);
    fclose(fp);             // close it.

    sprintf(cmd,"sendmail %s < %s",nQHuy.mail,tempFile); // prepare command.
    system(cmd);     // execute it.

    printf("\nSuccessfully send mail\n");
    

    pthread_mutex_unlock(&lock);
    return 0;
}


int main(){
    nQHuy.tinChiTichLuy=100;
    nQHuy.noHocPhi=100000000;

    int i = 0;
    int err;

    while(i < 2)
    {
        err = pthread_create(&(tid[i]), NULL,&updateInfoStudent,NULL);
        if (err != 0)
            printf("\ncan't create thread :[%s]", strerror(err));
        i++;
    }

    pthread_join(tid[0], NULL);
    pthread_join(tid[1], NULL);

    return 0;
}
