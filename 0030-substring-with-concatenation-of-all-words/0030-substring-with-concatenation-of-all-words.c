#include <stdlib.h>
#include <string.h>

static unsigned long hashWord(const char *s, int len) {
    unsigned long h = 2166136261u;

    for (int i = 0; i < len; i++) {
        h ^= (unsigned char)s[i];
        h *= 16777619u;
    }

    return h;
}

int* findSubstring(char* s, char** words, int wordsSize,
                   int* returnSize) {
    *returnSize = 0;

    int n = strlen(s);

    int* result = malloc((n + 1) * sizeof(int));

    if (!result)
        return NULL;

    if (wordsSize == 0) {
        return result;
    }

    int wordLen = strlen(words[0]);
    int totalLen = wordLen * wordsSize;

    if (n < totalLen || wordLen == 0) {
        return result;
    }

    int tableSize = 1;
    while (tableSize < wordsSize * 4)
        tableSize *= 2;

    int* table = calloc(tableSize, sizeof(int));
    int* need = calloc(wordsSize, sizeof(int));
    int* have = calloc(wordsSize, sizeof(int));

    if (!table || !need || !have) {
        free(table);
        free(need);
        free(have);
        free(result);
        return NULL;
    }

    // Build a hash table of unique words.
    for (int i = 0; i < wordsSize; i++) {
        unsigned long h = hashWord(words[i], wordLen);
        int slot = (int)(h & (tableSize - 1));

        while (table[slot] != 0) {
            int rep = table[slot] - 1;

            if (strncmp(words[rep], words[i], wordLen) == 0)
                break;

            slot = (slot + 1) & (tableSize - 1);
        }

        if (table[slot] == 0)
            table[slot] = i + 1;

        need[table[slot] - 1]++;
    }

    // Check a substring-sized chunk against the hash table.
    // Return the representative index, or -1 if absent.
    // The same probing rules are used in both places.
    for (int offset = 0; offset < wordLen; offset++) {
        int left = offset;
        int count = 0;

        for (int right = offset;
             right + wordLen <= n;
             right += wordLen) {

            unsigned long h = hashWord(s + right, wordLen);
            int slot = (int)(h & (tableSize - 1));
            int id = -1;

            while (table[slot] != 0) {
                int rep = table[slot] - 1;

                if (strncmp(s + right, words[rep],
                            wordLen) == 0) {
                    id = rep;
                    break;
                }

                slot = (slot + 1) & (tableSize - 1);
            }

            if (id == -1) {
                while (left < right) {
                    unsigned long lh = hashWord(s + left, wordLen);
                    int ls = (int)(lh & (tableSize - 1));
                    int oldId = -1;

                    while (table[ls] != 0) {
                        int rep = table[ls] - 1;

                        if (strncmp(s + left, words[rep],
                                    wordLen) == 0) {
                            oldId = rep;
                            break;
                        }

                        ls = (ls + 1) & (tableSize - 1);
                    }

                    if (oldId != -1)
                        have[oldId]--;

                    left += wordLen;
                }

                left = right + wordLen;
                count = 0;
                continue;
            }

            have[id]++;
            count++;

            while (have[id] > need[id]) {
                unsigned long lh = hashWord(s + left, wordLen);
                int ls = (int)(lh & (tableSize - 1));

                while (table[ls] != 0) {
                    int rep = table[ls] - 1;

                    if (strncmp(s + left, words[rep],
                                wordLen) == 0) {
                        have[rep]--;
                        break;
                    }

                    ls = (ls + 1) & (tableSize - 1);
                }

                left += wordLen;
                count--;
            }

            if (count == wordsSize) {
                result[(*returnSize)++] = left;

                unsigned long lh = hashWord(s + left, wordLen);
                int ls = (int)(lh & (tableSize - 1));

                while (table[ls] != 0) {
                    int rep = table[ls] - 1;

                    if (strncmp(s + left, words[rep],
                                wordLen) == 0) {
                        have[rep]--;
                        break;
                    }

                    ls = (ls + 1) & (tableSize - 1);
                }

                left += wordLen;
                count--;
            }
        }

        // Clear counts before processing the next offset.
        memset(have, 0, wordsSize * sizeof(int));
    }

    free(table);
    free(need);
    free(have);

    return result;
}