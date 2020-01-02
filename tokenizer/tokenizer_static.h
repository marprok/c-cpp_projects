
#ifndef TOKENIZER_H
#define TOKENIZER_H

#define BUCKET_SIZE 10
#define TOKEN_SIZE 32
typedef struct token_bucket_
{
    char* m_bucket[BUCKET_SIZE];
    char  m_storage[BUCKET_SIZE][TOKEN_SIZE];
    size_t m_size;
} bucket_t;

void reset_bucket(bucket_t *bucket);
int create_bucket(bucket_t *bucket, const char *str, const char* delim);

#endif /* TOKENIZER_H */
