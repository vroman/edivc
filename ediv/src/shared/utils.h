#ifndef __UTILS_H
#define __UTILS_H

#ifdef _WIN32 /* Codigo windowsero */
int strcasecmp(const char *s1, const char *s2);
int strncasecmp2(const char *s1, const char *s2, int n);
#endif /* _WIN32 */

#ifdef _LINUX
char *strlwr(char *str);
#endif

#endif /* __UTILS_H */ 