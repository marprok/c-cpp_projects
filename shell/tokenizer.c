#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include "tokenizer.h"

/*
  This is the static allocation version.
*/
void reset_bucket(bucket_t *bucket)
{
    size_t i;
    /* Reset the bucket state */
    bucket->m_size = 0;
    for (i = 0; i < BUCKET_SIZE; ++i)
        memset(bucket->m_storage[i], 0, sizeof(char)*TOKEN_SIZE);
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
    while (token && bucket->m_size < BUCKET_SIZE)
    {
        token_len = strlen(token);
        if (token[token_len-1] == '\n')
        {
            token[token_len-1] = '\0';
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
