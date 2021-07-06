#include "record.h"
#include "rb.h"
#include "vector.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <math.h>
#include <time.h>

void usage() {
  printf("Word Count:\n");
  printf("Count then instance of words in a document\n");
  printf("wordcount <filename>\n");
}

void print_words(rb_node_t *node) {
  if (node->left != NULL) {
    print_words(node->left);
  }

  word_record_t *wr = (word_record_t *) node->data;
  printf("Word: %s, Count: %d\n", wr->word, wr->count);

  if (node->right != NULL) {
    print_words(node->right);
  }
}

int main(int argc, char *argv[])
{

  if (argc < 2) {
    usage();
    return 0;
  }

  char *filename = argv[1];
  char *contents = NULL;
  long bufsize;
  FILE *f = fopen(filename, "r");
  if (f == NULL) {
    printf("Could not open file %s\n", filename);
    return -1;
  }

  if (fseek(f, 0L, SEEK_END) == 0) {
    bufsize = ftell(f);
    if (bufsize == -1) {
      printf("Could not get file length\n");
      fclose(f);
      return -1;
    }
    contents = (char *) malloc(sizeof(char) * (bufsize+1));

    if (fseek(f, 0L, SEEK_SET) != 0) {
      printf("Couldn't seek back to beginning of file\n");
      fclose(f);
      return -1;
    }

    size_t newLen = fread(contents, sizeof(char), bufsize, f);
    if (ferror(f) != 0) {
      printf("Error reading file\n");
      fclose(f);
      return -1;
    }

    contents[newLen++] = '\0';
    fclose(f);
  }

  rb_tree_t *words = (rb_tree_t *) malloc(sizeof(rb_tree_t));
  words->root = NULL;
  words->compare = compare_word_records;
  words->find = find_word_record;

  vector_head_t *vector_head = make_vector();

  int power = 0;
  int wordbuffer_size = floor(sizeof(char) * 1024 * pow(2.0, (float) power));
  char *wordbuffer = (char *) malloc(wordbuffer_size);
  memset(wordbuffer, 0, 1024);
  int wordbuffer_count = 0;
  wordbuffer[0] = '\0';
  char *nextword;
  for (int i = 0; i < bufsize; i++) {
    char c = contents[i];
    if (!isalpha(c)) {
      if (wordbuffer_count > 0) {
        wordbuffer[wordbuffer_count] = '\0';
        nextword = (char *) malloc((sizeof(char) * wordbuffer_count) + 1);
        strcpy(nextword, wordbuffer);
        rb_node_t *wr_node = find(words, nextword);
        if (wr_node == NULL) {
          word_record_t *wr = (word_record_t *) malloc(sizeof(word_record_t));
          wr->word = nextword;
          wr->count = 1;
          insert(words, wr);
        } else {
          word_record_t *wr = (word_record_t *) wr_node->data;
          wr->count++;
        }

        word_record_t *wr = vector_find(vector_head, nextword);
        if (wr == NULL) {
          word_record_t vecrec;
          vecrec.word = nextword;
          vecrec.count = 1;
          vector_insert(vector_head, vecrec);
        } else {
          wr->count++;
        }

        memset(wordbuffer, 0, wordbuffer_size);
        wordbuffer[0] = '\0';
        wordbuffer_count = 0;
      }
    } else {
      wordbuffer[wordbuffer_count] = tolower(c);
      wordbuffer_count++;
    }
  }

  printf("Done indexing %d entries.\n", words->count);
  printf("The last word of the vector is %s\n", vector_head->start[vector_head->current_size-1].word);

  if (argc > 2 && strcmp(argv[2], "-i") == 0) {
    printf("Beginning interactive mode\n");
    char in[128];
    clock_t t;
    double elapsed;
    while (1) {
      printf("Enter a word to search.\n");
      gets(in);
      printf("RedBlack Search for %s\n", in);
      t = clock();
      rb_node_t *node = find(words, in);
      t = clock() - t;
      elapsed = ((double) t) / CLOCKS_PER_SEC;
      if (node != NULL) {
        word_record_t *record = (word_record_t *) node->data;
        printf("RB Count: %d\n", record->count);
      } else {
        printf("Could not find word: %s in rbtree\n", in);
      }
      printf("RB Search took %f seconds\n", elapsed);

      t = clock();
      word_record_t *vector_wr = vector_find(vector_head, in);
      t = clock() - t;
      elapsed = ((double) t) / CLOCKS_PER_SEC;
      if (vector_wr != NULL) {
        printf("Vector Count: %d\n", vector_wr->count);
      } else {
        printf("Could not find word: %s in vector\n", in);
      }
      printf("Vector Search took %f seconds\n", elapsed);
    }
  }

}