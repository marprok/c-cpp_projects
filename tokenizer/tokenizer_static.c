#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

/*
  This is the static allocation version.
*/

#define BUCKET_SIZE 10
#define TOKEN_SIZE 32
typedef struct token_bucket_
{
    char* m_bucket[BUCKET_SIZE];
    char  m_storage[BUCKET_SIZE][TOKEN_SIZE];
    size_t m_size;
} bucket_t;

void reset_bucket(bucket_t *bucket)
{
    size_t i;
    /* Reset the bucket state */
    bucket->m_size = 0;
    for (i = 0; i < BUCKET_SIZE; ++i)
        memset(bucket->m_storage[i], 0, sizeof(char)*sizeof(TOKEN_SIZE));
    memset(bucket->m_bucket, 0, sizeof(char*)*BUCKET_SIZE);
}

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

    reset_bucket(bucket);
    
    strcpy(buff, str);
    token = strtok(buff, delim);
    if (!token)
    {
        token_len = strlen(buff);
        if (buff[token_len-1] == '\n')
        {
            buff[token_len-1] = '\0';
            token_len--;
        }
        if (token_len > TOKEN_SIZE)
        {
            fprintf(stderr, "Token[%lu] size exceeds the token size limit!\n", bucket->m_size);
            return 0;
        }
        
        strcpy(bucket->m_storage[bucket->m_size], buff);
        bucket->m_bucket[bucket->m_size] = bucket->m_storage[bucket->m_size];
        bucket->m_size++;
        return 1;
    }
    
    while (token && bucket->m_size < BUCKET_SIZE)
    {
        token_len = strlen(token);
        if (buff[token_len-1] == '\n')
        {
            buff[token_len-1] = '\0';
            token_len--;
        }

        if (token_len > TOKEN_SIZE)
        {
            fprintf(stderr, "Token[%lu] size exceeds the token size limit!\n", bucket->m_size);
            return 0;
        }
        
        strcpy(bucket->m_storage[bucket->m_size], token);
        bucket->m_bucket[bucket->m_size] = bucket->m_storage[bucket->m_size];
        bucket->m_size++;
        token = strtok(NULL, delim);
    }
    return 1;
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
    for (i = 0; bucket.m_bucket[i]; ++i)
        printf("Bucket[%lu] %s\n", i, bucket.m_bucket[i]);
    reset_bucket(&bucket);


    create_bucket(&bucket, "grep \"test\" test.txt", " ");
    for (i = 0; bucket.m_bucket[i]; ++i)
        printf("Bucket[%lu] %s\n", i, bucket.m_bucket[i]);
    reset_bucket(&bucket);
    
    return 0;
}
