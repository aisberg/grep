//
//  main.c
//  grep
//
//  Created by Макс on 24.03.14.
//  Copyright (c) 2014 Максим Никитин. All rights reserved.
//

#include <stdio.h>
#include <sys/types.h>
#include <dirent.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <string.h>
#include <limits.h>

int count = 0;

#define BUF     64

int * prefixFunction (const char *s) {
	int n = (int)strlen(s);
    int *pi;
    pi=(int *)malloc(n*sizeof(int));
    int i;
	for (i = 1; i < n; ++i) {
		int j = pi[i - 1];
        
		while (j > 0 && s[i] != s[j])
			j = pi[j - 1];
        
		if (s[i] == s[j])  ++j;
		pi[i] = j;
	}
    
    
	return pi;
}


void kmp(const char *text, const char *str) {
    
    int *pref;
    pref = prefixFunction(str);
   
    int k = 0;
    int i;
    for (i = 0; i < strlen(text); ++i) {
        
        while (k > 0 && (k >= strlen(str) || str[k] != text[i]) )
            k = pref[k - 1];
        
        if (text[i] == str[k]) k++;
        //printf("OK\n");
        if (k == strlen(str)){count++; printf("%s\n", text); return; }
        
    }
}

void grep(const char *pattern, const char *path){
   
    DIR *dir1;
    struct dirent *entry;
    struct stat inform;
    char path1[PATH_MAX + 1];
    
    dir1 = opendir(path);
    
    if (!dir1)
    {
        fprintf ( stderr, "Cannot open file '%s'\n", path );
        exit(1);
    }
    
    while (( entry = readdir( dir1 )) != NULL )
    {
    
        if ( entry->d_name[0] == '.')
            continue;
        
        printf("FILENAME: %s\n",entry->d_name);
        
        strncpy( path1, path, PATH_MAX );
        strncat( path1, "/", PATH_MAX );
        strncat( path1, entry->d_name, PATH_MAX );

        FILE *fp;
        if ((fp = fopen(path1, "r")) == NULL) {
            printf("He удается открыть файл.\n");
            continue;
        }
        
        
        
        if (stat( path1, &inform ) < 0)
        {
            fprintf ( stderr, "ls: Cannot open file\n");
            exit(1);
        }
        
        if (inform.st_mode & S_IFREG){
           char *string [BUF + 1];
           while(fgets(string, BUF, fp) != NULL){
               kmp(string, pattern);
           }
        }
        if (inform.st_mode & S_IFDIR)
            grep(pattern, path1);
        
        
 
        
    }
    closedir(dir1);
    if(!count) printf("Empty!\n");
    else printf("EXELENT\n");
    
}

int main(int argc, const char * argv[])
{
    struct stat inform;
    //printf("%d\n", (int)strlen(argv[1]));
    
    

    if ( argc < 3 )
	{
		fprintf (stderr, "Too few arguments\n");
		exit (1);
	}

    grep(argv[1], argv[2]);
  
    
}

