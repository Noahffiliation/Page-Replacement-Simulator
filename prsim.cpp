#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <string.h>
#include <time.h>

// Data structure to store page numbers as a long array
class Array {
    public:
        // Set maxSize based on num-pages and initialize local data
        Array(int maxSize) {
            this->maxSize = maxSize;
            curSize = 0;
            index = 0;
            items = new long[maxSize];
        }

        // Check to see if the array is full
        bool isFull() {
            return curSize == maxSize;
        }

        // Linearly search for a specified value in the array
        bool search(long value) {
            for (int i = 0; i < curSize; i++) {
                if (items[i] == value) {
                    return true;
                }
            }

            return false;
        }

        // Add the new item to array and remove the oldest item if full
        void addfifo(long newItem) {
            items[index] = newItem;
            index++;
            curSize++;
            if (curSize > maxSize) {
                curSize--;
            }
            index %= maxSize;
        }

        // If full, randomly pick an index to replace
        void addrandom(long newItem) {
            if (isFull()) {
                int idx = rand() % maxSize;
                this->replace(newItem, idx);
            } else {
                items[curSize] = newItem;
                curSize++;
            }
        }

        // Put an item into a specified index in the array
        void replace(long replaceItem, int idx) {
            items[idx] = replaceItem;
        }

        int maxSize; // Maximum array size
        int curSize; // Current size of indeces used
        int index; // Used for FIFO insertion
        long *items; // Long pointer to hold page numbers
};

// Does page replacement on a random index in the array
unsigned int rrandom(int n) {
    Array *list = new Array(n);
    unsigned int num_faults = 0;
    char a;
    long mem_address;
    int size;
    // Scan for the trace instructions from traces
    while (scanf("%c %lx, %d\n", &a, &mem_address, &size) > 0) {
        // Check for junk instructions and skip them
        if (a == '=') {
            scanf("%*s[\n]");
            continue;
        }

        // Extract the page number from a 64-bit address
        long page_num = mem_address >> 12;
        // If the page isn't in the array, remove the first in and increase page faults
        if (list->search(page_num) == false) {
            list->addrandom(page_num);
            num_faults++;
        }
        // Check the next instruction to see if it's on the same page as before
        // and skip if they're the same page
        mem_address = mem_address + size - 1;
        if (mem_address >> 12 != page_num) {
            page_num = mem_address >> 12;
            if (list->search(page_num) == false) {
                list->addrandom(page_num);
                num_faults++;
            }
        }
    }

    return num_faults;
}

// Does page replacement with a First In First Out algorithm
unsigned int fifo(int n) {
    Array *list = new Array(n);
    unsigned int num_faults = 0;
    char a;
    long mem_address;
    int size;
    // Scan for the trace instructions from traces
    while (scanf("%c %lx, %d\n", &a, &mem_address, &size) > 0) {
        // Check for junk instructions and skip them
        if (a == '=') {
            scanf("%*s[\n]");
            continue;
        }

        // Extract the page number from a 64-bit address
        long page_num = mem_address >> 12;
        // If the page isn't in the array, remove the first in and increase page faults
        if (list->search(page_num) == false) {
            list->addfifo(page_num);
            num_faults++;
        }
        // Check the next instruction to see if it's on the same page as before
        // and skip if they're the same page
        mem_address = mem_address + size - 1;
        if (mem_address >> 12 != page_num) {
            page_num = mem_address >> 12;
            if (list->search(page_num) == false) {
                list->addfifo(page_num);
                num_faults++;
            }
        }
    }

    return num_faults;
}

// Runs a page replacement simulator that does memory page replacement
// with a specified number of pages to use and a specified algorithm of replacing pages
int main(int argc, char **argv) {
    // Seed randomness
    srand(time(NULL));
    // Struct for allowing --num-pages and --policy arguments
    static struct option long_options[] = {
          {"num-pages", required_argument, 0, 'n'},
          {"policy", required_argument, 0, 'p'},
          {0, 0, 0, 0}
    };

    int n; // Store num-pages
    char *p; // Store policy
    int option_index = 0; // Index through long_options
    int c; // Store current argument option
    // Parse command line for number of pages and policy
    while ((c = getopt_long(argc, argv, "n:p:", long_options, &option_index)) != -1) {
        switch (c) {
            case 'n':
                // Read in num-pages
                n = atoi(optarg);
                break;
            case 'p':
                // Read in the desired page replacement policy
                p = optarg;
                break;
            default:
                printf("Default\n");
                break;
        };
    }

    // Unsigned int for holding up to 3 billion as a number
    unsigned int num_faults;
    // Captitalize 'random' to match output with 'prsim-good'
    char *tmp;
    if (strcmp(p, "random") == 0) {
        num_faults = rrandom(n);
        tmp = (char *) "Random";
    } else if (strcmp(p, "FIFO") == 0) {
        num_faults = fifo(n);
        tmp = (char *) "FIFO";
    } else {
        printf("Error: choose another policy\n");
    }

    // Print results
    printf("%s %d: %u\n", tmp, n, num_faults);

    return 0;
}
