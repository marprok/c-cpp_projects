
#ifndef TOKENIZER_H
#define TOKENIZER_H

#define BUCKET_SIZE 10
typedef struct token_bucket_
{
    char* m_bucket[BUCKET_SIZE];
    size_t m_size;
} bucket_t;

void release_bucket(bucket_t *bucket);
int create_bucket(bucket_t *bucket, const char *str, const char* delim);

#endif /* TOKENIZER_H */
