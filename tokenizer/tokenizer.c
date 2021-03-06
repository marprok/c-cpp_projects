#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include "tokenizer.h"

int create_bucket(bucket_t *bucket, const char *str, const char* delim)
{
    char buff[256];
    char *token = NULL;
    size_t token_len = 0;

    if (!bucket || !str || !delim)
    {
        fprintf(stderr, "Error: NULL argumests!\n");
        return 0;
    }
    
    bucket->m_size = 0;
    memset(bucket->m_bucket, 0, sizeof(char*)*BUCKET_SIZE);
    strcpy(buff, str);
    token = strtok(buff, delim);
    while (token && bucket->m_size < BUCKET_SIZE)
    {
        token_len = strlen(token);
        if (token[token_len-1] == '\n')
        {
            token[token_len-1] = '\0';
            token_len--;
        }
        bucket->m_bucket[bucket->m_size] = malloc(sizeof(char)*token_len + 1);
        if (!bucket->m_bucket[bucket->m_size])
        {
            fprintf(stderr, "Error: %s\n", strerror(errno));
            return 0;
        }
        strcpy(bucket->m_bucket[bucket->m_size], token);
        bucket->m_size++;
        token = strtok(NULL, delim);
    }
    return 1;
}

void release_bucket(bucket_t *bucket)
{
    size_t i = 0;
    if (!bucket)
        return;
    for (; i < bucket->m_size; ++i)
    {
        free(bucket->m_bucket[i]);
        bucket->m_bucket[i] = NULL;
    }
}

int main(int argc, char **argv)
{
    bucket_t bucket;
    size_t i = 0;
    if (argc != 3)
    {
        fprintf(stderr, "USAGE: ./a.out string delim\n");
        exit(EXIT_FAILURE);
    }

    create_bucket(&bucket, argv[1], argv[2]);
    for (; i < bucket.m_size; ++i)
        printf("Bucket[%lu] %s\n", i, bucket.m_bucket[i]);
    release_bucket(&bucket);

    create_bucket(&bucket, "banana apple", " ");
    for (i = 0; bucket.m_bucket[i]; ++i)
        printf("Bucket[%lu] %s\n", i, bucket.m_bucket[i]);
    release_bucket(&bucket);
    return 0;
}
