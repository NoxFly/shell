#ifndef UTILS_H
#define UTILS_H

/**
 * Returns either s1 and s2 strings are equals or not
 * @param s1 first string
 * @param s2 second string
 * @return 0 means they're equal, other number indicates the index of the first alphabetical change
 */
int isEq(char *s1, char *s2);

/**
 * Similar as str_replace function.
 * Replaces a given substring in the provided source by the given replacement string.
 * @param source source string
 * @param substring the substring to search and replace
 * @param replacement the subtring to replace in
 * @return the new string
 */
char *strrep(char *source, char *substring, char *replacement);

void printArgs(char **command);

#endif // UTILS_H