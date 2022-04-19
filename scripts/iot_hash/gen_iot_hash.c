

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#define SEED_1 89
#define SEED_2 19
#define PRIME_1 56893
#define PRIME_2 70607

static uint64_t string_hash(const char* s, const uint64_t length, uint64_t seed)
{
	uint64_t hash = seed;
	while (*s) {
		hash = (hash * PRIME_1) ^ (s[0] * PRIME_2);
		s++;
	}
	return hash % length;
}

int main(int argc, char* argv[]){
  if(argc != 4){
    printf("Invalid number of arguments. This pogram must have three argumnets passed.\n");
    return -1;
  }
  const char* str = argv[1];
	const uint64_t length = atol(argv[2]);
	const uint64_t seed = atol(argv[3]);
  uint64_t hash = string_hash(str, length, seed);

  printf("%ld\n", hash);

  return 0;
}
