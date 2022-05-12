#include <check.h>

int filecount=0, dircount=0;
void printDir(char* dirname, bool arr[], int depth);

int main(int argc, char* argv[]){
    ARGS_CHECK(argc,2);
    DIR* pdir=opendir(argv[1]);
    ERROR_CHECK(pdir,NULL,"opendir");
    bool arr[100]={0};

    chdir(argv[1]);
    printf("%s\n",argv[1]);
    printDir(argv[1],arr,0);

    printf("\n%d directories, %d files\n",dircount,filecount);
    return 0;
}

void printDir(char* dirname,bool arr[],int depth){
    //open dir
    DIR* pdir=opendir(dirname);
    //ERROR_CHECK(pdir,NULL,"opendir");
    if(pdir==NULL) return;
    
    //if next equals NULL, entry point to the final dirent
    struct dirent* entry, *next=NULL;
    do {
        entry=readdir(pdir);
    
    }while(entry!=NULL && entry->d_name[0]=='.');

    //search all entries
    while(entry!=NULL){

        //ignore hidden entries
        do{
            next=readdir(pdir);
        }while(next!=NULL&&next->d_name[0]=='.');
        
        if(entry->d_name[0]=='.'){
            entry=next;
            continue;
        }
        
        //for directory, search deeper entries
        if(entry->d_type==DT_DIR){
            
            dircount++;

            for(int i=0;i<depth;i++){
                if(arr[i]) printf("|  ");
                else printf("   ");
            }
            if(next!=NULL) printf("|--%s\n",entry->d_name);
            else printf("`--%s\n",entry->d_name);
            
            arr[depth]=(next!=NULL);

            char* path=(char*)calloc(1,strlen(entry->d_name)+strlen(dirname)+2);
            sprintf(path,"%s/%s",dirname,entry->d_name);
        
            printDir(path,arr,depth+1);

            free(path);
        }

        //other type files
        else {

            filecount++;

            for(int i=0;i<depth;i++){
                if(arr[i]) printf("|  ");
                else printf("   ");
            }
            if(next!=NULL) printf("|--%s\n",entry->d_name);
            else printf("`--%s\n",entry->d_name);
        }
        entry=next;
    }
    closedir(pdir);
}
