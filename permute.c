#include <openssl/sha.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

void throwerror(char *err_msg) {
	fprintf(stderr, "%s\n", err_msg);
	exit(1);
}

int array_contains(long arr_len, long arr[arr_len], long el)
{
	long i;
	for (i=0; i<arr_len && arr[i]!=-1; i++) 
		if (arr[i] == el)
			return 1;
	return 0;
}

int main(int argc, char *argv[]) {
	unsigned long lower_bnd, upper_bnd, tot_elem, nb_left, next_idx, i, lim;
	char *tmp;

	if (argc < 4)
		throwerror("error not enough arguments");

  long nb_selected=-1;
  for (i=1; i<argc; i++)
    if (strcmp(argv[i], "-n") == 0) {
      if (i+1>=argc)
        throwerror("missing argument for -n option");
      nb_selected=strtol(argv[i+1], &tmp, 10);
      break;
    }
	
	char sha1_dgst[SHA_DIGEST_LENGTH];
	SHA1(argv[1], sizeof(argv[1]), sha1_dgst);

 	lower_bnd = strtol(argv[2], &tmp, 10);
	upper_bnd = strtol(argv[3], &tmp, 10);

	tot_elem = nb_left = upper_bnd - lower_bnd + 1;
	long range[tot_elem], chosen[tot_elem], to_print[tot_elem]; 

	for (i=0; i<tot_elem;i++)
		range[i]=lower_bnd+i;

	chosen[0]=-1;
	for (i=0; i < tot_elem; i++) {
		unsigned long ith_sha = (unsigned long)sha1_dgst[i];
		next_idx=ith_sha % tot_elem;
		if (array_contains(tot_elem, chosen, next_idx)) {
			do {
				next_idx=(next_idx+1)%tot_elem;
			} while(array_contains(tot_elem, chosen, next_idx));
		}
    to_print[tot_elem-nb_left]=range[next_idx];
		chosen[tot_elem - nb_left] = next_idx;
		if (nb_left > 0)
			chosen[(tot_elem - nb_left) + 1] = -1;
		nb_left--;
	}
  
  lim=tot_elem;
  if (nb_selected <= tot_elem && nb_selected >= 0) 
    lim=nb_selected;

  for (i=0; i<lim; i++)
    printf("%ld\n", to_print[i]);
  
	return 0;
}

