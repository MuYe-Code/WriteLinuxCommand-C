#include "check.h"

#define LENGTH 4096

void cpy(char* src, char* dest);

int main(int argc, char* argv[]){
    //check two directories
    ARGS_CHECK(argc,3);
    DIR* psrc=opendir(argv[1]);
    ERROR_CHECK(psrc,NULL,"open psrc");
    DIR* pdest=opendir(argv[2]);
 
    //dir1 => dir2
    if(pdest==NULL){
        cpy(argv[1],argv[2]);
    }
    //dir1 => dir2/dir1
    else{
        //get the dirname of argv[1]
        char* destpath=(char*)malloc(strlen(basename(argv[1]))+strlen(argv[2])+2);
        sprintf(destpath,"%s/%s",argv[2],basename(argv[1]));
        cpy(argv[1],destpath);
        free(destpath);
    }

    closedir(psrc);
    closedir(pdest);
    return 0;
}

void cpy(char* src, char* dest){
    DIR* psrc=opendir(src);
    ERROR_CHECK(psrc,NULL,"open psrc");

    //if destdir is not exist, create a directory
    DIR* pdest=opendir(dest);
    int retVal=0;
    if(pdest==NULL){
        retVal=mkdir(dest,0777);
        ERROR_CHECK(retVal,-1,"mkdir");
        pdest=opendir(dest);
        ERROR_CHECK(pdest,NULL,"open pdest");
    }
    
    struct dirent* entry;
    do{
        entry=readdir(psrc);
    
        //ignore . and ..
        if(entry==NULL || strcmp(entry->d_name,".")==0 || strcmp(entry->d_name,"..")==0)
            continue;
        
        //get the absolute path of src and dest
        char* srcpath=(char*)malloc(strlen(entry->d_name)+strlen(src)+2);
        char* destpath=(char*)malloc(strlen(entry->d_name)+strlen(dest)+2);
        sprintf(srcpath,"%s/%s",src,entry->d_name);
        sprintf(destpath,"%s/%s",dest,entry->d_name);
        
        //if current entry is a directory, we should copy deeper
        if(entry->d_type==DT_DIR){
            cpy(srcpath,destpath);
        }
        
        //other type
        else{
            int fdr=open(srcpath,O_RDONLY);
            ERROR_CHECK(fdr,-1,"open pdr");
            int fdw=open(destpath,O_CREAT|O_RDWR|O_TRUNC,0666);
            ERROR_CHECK(fdw,-1,"open pdw");
            
            //copy by mmap
            struct stat statbuf;
            fstat(fdr,&statbuf);
            
            ftruncate(fdw,statbuf.st_size);
            off_t fsize=0;
            int cp_size=LENGTH;

            while(fsize < statbuf.st_size){
                if(statbuf.st_size-fsize < LENGTH) { 
                    cp_size = statbuf.st_size - fsize;     
                 }

                char* srcmap = (char*)mmap(NULL,cp_size,PROT_READ,MAP_SHARED,fdr,fsize);
                ERROR_CHECK(srcmap,MAP_FAILED,"srcmap");
                char* destmap = (char*)mmap(NULL,cp_size,PROT_READ|PROT_WRITE,MAP_SHARED,fdw,fsize);
                ERROR_CHECK(destmap,MAP_FAILED,"destmap");

                memcpy(destmap,srcmap,cp_size);
                munmap(srcmap,cp_size);
                munmap(destmap,cp_size);

                fsize+=cp_size;
            }
        }
        free(srcpath);
        free(destpath);
    }while(entry!=NULL);

    closedir(psrc);
    closedir(pdest);
}
