#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "Item.h"
#include "URLNode.h"
#include "readData.h"

#define MAX_DISPLAYED_PAGE 30

int main(int argc, char *argv[]) {
	int i = 1;
	SortedListPtr searchTerms = NULL;
	for (; i < argc; i++) {
		SLaddByValue(&searchTerms, trim(argv[i]));
	}

	int numberOfSearchTerm = SLsize(searchTerms);
	if (numberOfSearchTerm <= 0) {
		return 0;
	}

    SortedListPtr corpus = getCollection("collection.txt");
    int corpusSize = SLsize(corpus);

	FILE* ptr = fopen("invertedIndex.txt", "r");
	if (ptr == NULL) {
		printf("%s: no such file.", "invertedIndex.txt");
		return 0;
	}

	SortedListPtr* wordToURLs = malloc(
			numberOfSearchTerm * sizeof(SortedListPtr));
	for (i = 0; i < numberOfSearchTerm; i++) {
		wordToURLs[i] = NULL;
	}

	char line[1000];
	while (fgets(line, 1000, ptr) != NULL) {
		Item word = NULL;
		int index = -1;
		char* pch = strtok(line, BLANK_TOKENS);
		if (pch != NULL) {
			word = ItemCopy(pch);
			index = SLindexOf(searchTerms, word);
			//only parse this line if the word is a search term
			while (pch != NULL && index != -1) {
				pch = strtok(NULL, BLANK_TOKENS);
				//if word from inverted index is one of search term
				//then store its related urls
				if (pch != NULL) {
					SLaddByValue(&wordToURLs[index], pch);
				}
			}
		}
		if (word != NULL) {
			free(word);
		}
	}
	fclose(ptr);

	SortedListPtr urls = NULL;
	for (i = 0; i < numberOfSearchTerm; i++) {
		SortedListPtr iter = wordToURLs[i];
		while (iter != NULL) {
			Item url = iter->val;
			SLaddByValue(&urls, url);
			iter = iter->next;
		}
	}

	int numberOfUrls = SLsize(urls);

    int j;

    //this matix to keep frequecy of a search term
    //in a particular url (document)
    URLNode** m = malloc(numberOfUrls*sizeof(URLNode*));
    for (i=0;i<numberOfUrls;i++) {
    	m[i] = malloc(numberOfSearchTerm*sizeof(URLNode));
    }
    for (i=0;i<numberOfUrls;i++) {
        for (j=0;j<numberOfSearchTerm;j++) {
        	m[i][j].index = i;
        	m[i][j].rank = 0.0;
        	m[i][j].score = 0;
        }
    }

    int* urlWordCount = malloc(numberOfUrls*sizeof(URLNode));
    for (i=0;i<numberOfUrls;i++){
        urlWordCount[i] = 0;
    }
    SortedListPtr iterator = urls;

    i = 0;
    while (iterator != NULL) {
        Item url = iterator->val;

		char* fn = concat2(url, ".txt");

		FILE* ptr = fopen(fn, "r");
		if (ptr == NULL) {
			printf("%s: no such file.", fn);
			return 1;
		}

		char buf[1000];
		//skip #start Section-1
		fscanf(ptr, "%*s %*s %s ", buf);

		while (strcmp("#end", buf)) {
			if (fscanf(ptr, "%s", buf) != 1) {
				break;
			}
		}

		//skip Section-1 #start Section-2
		fscanf(ptr, "%*s %*s %*s %s", buf);
		while (strcmp("#end", buf)) {
			Item word = trim(buf);
            int wordIndex = SLindexOf(searchTerms, word);
            if(wordIndex != -1) {
                m[i][wordIndex].score++;
            }
            urlWordCount[i]++;
			if (fscanf(ptr, "%s", buf) != 1) {
				break;
			}
		}
		fclose(ptr);
//		printf("File %s Total Word count: %d\n", fn, urlWordCount[i]);
        iterator = iterator->next;
        i++;
    }
    //calculate tf for each word per document
    for (i=0;i<numberOfUrls;i++) {
        for (j=0;j<numberOfSearchTerm;j++) {
//        	printf("URL: %s Search Term: %s Score: %d\n", SLgetByIndex(corpus, i), SLgetByIndex(searchTerms, j), m[i][j].score);
            if (urlWordCount[i] > 0) {
                double tfperdoc = (double)m[i][j].score/(double)urlWordCount[i];
                double tfpercorpus = log10((double)corpusSize/(double)SLsize(wordToURLs[j]));
                m[i][j].rank = tfperdoc*tfpercorpus;
            } else {
                m[i][j].rank = 0.0;
            }
        }
    }

	URLNodePtr urlNodes = malloc(numberOfUrls * sizeof(URLNode));
	for (i = 0; i < numberOfUrls; i++) {
		urlNodes[i].index = 0;
		urlNodes[i].rank = 0.0;
		urlNodes[i].url = NULL;
		urlNodes[i].score = 0;
	}

	//loop through all word->urls matrix to update
	//page score (number of terms found in a page)
	for (i = 0; i < numberOfSearchTerm; i++) {
		SortedListPtr iter = wordToURLs[i];
		while (iter != NULL) {
			Item url = iter->val;
			int index = SLindexOf(urls, url);
			if (urlNodes[index].url == NULL) {
				urlNodes[index].url = ItemCopy(url);
				urlNodes[index].index = index;
				double tfidfSum = 0.0;
				for (j = 0; j < numberOfSearchTerm; j++) {
                    tfidfSum = tfidfSum + m[index][j].rank;
				}
				urlNodes[index].rank = tfidfSum;
			}
			urlNodes[index].score++;
			iter = iter->next;
		}
	}

	int* indices = malloc(numberOfUrls * sizeof(int));

	for (i = 0; i < numberOfUrls; i++) {
		indices[i] = i;
	}
    /* top 30 pages in descending order of number of search terms found and then within each group
       descending order of summation of tf-idf values of all search terms found (copied from spec)
    */
	quickSort1(indices, 0, numberOfUrls - 1, urlNodes);

	for (i = 0; i < numberOfUrls && i < MAX_DISPLAYED_PAGE; i++) {
		printf("%s, %.7f\n", urlNodes[indices[i]].url,
				urlNodes[indices[i]].rank);

	}

	// Free all memories used
    for (i=0;i<numberOfUrls;i++) {
    	free(m[i]);
    }
    free(m);
	SLfree(searchTerms);
	for (i = 0; i < numberOfSearchTerm; i++) {
		SLfree(wordToURLs[i]);
	}
	SLfree(corpus);
	free(wordToURLs);
	SLfree(urls);
	free(urlNodes);
	free(indices);
	return 0;
}

