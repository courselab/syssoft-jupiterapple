#include <stdlib.h>
#include <stdio.h>
#include <string.h>

int main(void)
{
  int verified = 0;
  char user_key[10];

  /*
   * Read user's credentials safely
   * using fgets() instead of scanf() to prevent buffer overflows.
   * It will read at most sizeof(user_key) - 1 characters, leaving
   * space for the null terminator.
   */
  printf("Enter password: ");
  if (fgets(user_key, sizeof(user_key), stdin) == NULL)
  {
    // Handle potential input error if fgets fails
    printf("Input error.\n");
    exit(1);
  }

  /*
   * Remove the trailing newline character read by fgets(), if it exists.
   * strcspn() finds the index of the first newline character.
   * We replace it with a null terminator to make the string suitable
   * for strcmp().
   */
  user_key[strcspn(user_key, "\n")] = 0;

  /* Verify credentials. */
  if (!strcmp(user_key, "foo"))
    verified = 1;

  if (!verified)
  {
    printf("Access denied\n");
    exit(1);
  }

  printf("Access granted.\n");

  /* Priviledged code follows... */
  return 0;
}