#include <stdio.h>
#include <stdlib.h>
// Structure to represent a node in a linked list used for storing key data
typedef struct lst_node
{
    char *data; // Character array to store key data
    int length; // Length of the key data
    int alloc;  // Allocated memory for key data
} lst_node;

// Structure to represent an entry in the hash map
typedef struct MapEntry
{
    char *key;               // Public: Key for the hash map entry
    int value;               // Public: Value associated with the key
    struct MapEntry *__prev; // Private: Pointer to the previous hash map entry
    struct MapEntry *__next; // Private: Pointer to the next hash map entry
} MapEntry;

// Structure to represent an iterator for iterating over hash map entries
typedef struct MapIterator
{
    int __buckets;                               // Number of buckets in the hash map
    struct Map *__map;                           // Pointer to the hash map
    MapEntry *__current;                         // Pointer to the current MapEntry
    MapEntry *(*next)(struct MapIterator *self); // Function pointer for getting the next MapEntry
    void (*del)(struct MapIterator *self);       // Function pointer for deleting the MapIterator

} MapIterator;

// Structure to represent a hash map
typedef struct Map
{
    // Private attributes
    int __buckets;              // Number of buckets in the hash map
    struct MapEntry *__head[8]; // Head of the hash map entry linked list
    struct MapEntry *__tail[8]; // Tail of the hash map entry linked list
    int __count;                // Count of hash map entries

    // Public Methods
    void (*put)(struct Map *self, const char *key, int value);        // Add or update a hash map entry
    int (*get)(struct Map *self, const char *key, int default_value); // Get the value associated with a key
    MapEntry *(*find)(struct Map *self, const char *key, int bucket); // Find a hash map entry by key
    int (*size)(struct Map *self);                                    // Get the size of the hash map
    void (*print)(struct Map *self);                                  // Print the hash map entries
    void (*del)(struct Map *self);                                    // Delete the hash map
    lst_node *(*new_key_node)(const char *key);                       // Create a new key node
    MapIterator *(*iter)(struct Map *self);                           // Create a new iterator
} Map;

// Function prototype declarations
Map *Map_new();                                                      // Implementation of a new hash map
void __Map_put(struct Map *self, const char *key, int value);        // Implementation of put method
int __Map_get(struct Map *self, const char *key, int default_value); // Implementation of get method
MapEntry *__Map_find(struct Map *self, const char *key, int bucket); // Implementation of find method
lst_node *__Map_new_key_node(const char *key);                       // Implementation of new_key_node method
int __Map_size(struct Map *self);                                    // Implementation of size method
void __Map_del(struct Map *self);                                    // Implementation of del method
void __Map_print(struct Map *self);                                  // Implementation of print method
MapIterator *__Map_iter(struct Map *self);                           // Implementation of iter method
MapEntry *__MapIter_next(MapIterator *self);                         // Implementation of next method
void __MapIter_del(MapIterator *self);                               // Implementation of deleting the MapIterator
int getBucket(const char *str, int buckets);                         // Function to calculate the bucket index for a given key in the hash map

// Main function
int main()
{
    printf("\nHash Map\n");
    Map *map = Map_new(); // Create a new hash map
    map->print(map);      // Print the initial hash map
    map->put(map, "a", 2);
    map->put(map, "d", 7);
    map->print(map); // Print the hash map after adding entries
    map->put(map, "a", 6);
    map->print(map); // Print the hash map after updating an entry
    map->put(map, "g", 8);
    map->put(map, "l", 9);
    map->print(map); // Print the hash map after adding more entries

    printf("\nPrint value of key, else print default value\n");
    printf("a = %d\n", map->get(map, "a", -1));
    printf("a = %d\n", map->get(map, "m", -1));

    printf("\nIterate\n");
    MapIterator *iter = map->iter(map); // Create a new iterator
    while (1)
    {
        MapEntry *current = iter->next(iter); // Get the next hash map entry
        if (current == NULL)
        {
            break;
        }
        else
        {
            printf("%s = %d\n", current->key, current->value);
        }
    }

    iter->del(iter); // Delete the iterator
    map->del(map);   // Delete the hash map

    return 0;
}

// Function to create a new Map instance
Map *Map_new()
{
    Map *newMap = (Map *)malloc(sizeof(Map));
    if (newMap == NULL)
    {
        printf("Could not allocate memory!");
        return newMap;
    }
    newMap->__buckets = 8;
    for (int i = 0; i < newMap->__buckets; i++)
    {
        newMap->__head[i] = NULL;
        newMap->__tail[i] = NULL;
    }

    newMap->__count = 0;

    // Assigning function pointers to methods
    newMap->put = &__Map_put;
    newMap->get = &__Map_get;
    newMap->find = __Map_find; // You don't need the & operator for function pointers.
    newMap->size = &__Map_size;
    newMap->del = &__Map_del;
    newMap->print = &__Map_print;
    newMap->new_key_node = __Map_new_key_node; // You don't need the & operator for function pointers.
    newMap->iter = __Map_iter;                 // You don't need the & operator for function pointers.

    return newMap;
}

// Implementation of the put method to add or update a hash map entry
void __Map_put(struct Map *self, const char *key, int value)
{
    int bucket = getBucket(key, self->__buckets);

    // If the hash map is empty
    if (self->__head[bucket] == NULL)
    {
        // Create a new hash map entry
        MapEntry *newEntry = (MapEntry *)malloc(sizeof(MapEntry));

        // Check if memory allocation for the new entry was successful
        if (newEntry == NULL)
        {
            printf("Could not allocate memory!");
            self->del(self); // Delete the hash map to avoid memory leaks
            return;
        }

        // Initialize the new hash map entry
        self->__head[bucket] = newEntry;
        self->__tail[bucket] = self->__head[bucket];
        lst_node *new_node = self->new_key_node(key);
        newEntry->key = new_node->data;
        newEntry->value = value;
        newEntry->__prev = NULL;
        newEntry->__next = NULL;
        self->__count++;
    }
    else
    {
        // Check if the key already exists in the hash map
        MapEntry *duplicate = self->find(self, key, bucket);
        if (duplicate)
        {
            // Update the value if the key exists
            duplicate->value = value;
        }
        else
        {
            // Create a new hash map entry
            MapEntry *newEntry = (MapEntry *)malloc(sizeof(MapEntry));

            // Check if memory allocation for the new entry was successful
            if (newEntry == NULL)
            {
                printf("Could not allocate memory!");
                self->del(self); // Delete the hash map to avoid memory leaks
            }

            // Link the new hash map entry to the tail
            self->__tail[bucket]->__next = newEntry;
            MapEntry *oldTail = self->__tail[bucket];
            self->__tail[bucket] = newEntry;
            self->__tail[bucket]->__prev = oldTail;
            self->__tail[bucket]->__next = NULL;

            // Initialize the new hash map entry
            lst_node *new_node = self->new_key_node(key);
            self->__tail[bucket]->key = new_node->data;
            self->__tail[bucket]->value = value;
            self->__count++;
        }
    }
}

// Implementation of the new_key_node method
lst_node *__Map_new_key_node(const char *key)
{
    // Allocate memory for a new list node
    lst_node *new_node = (lst_node *)malloc(sizeof(lst_node));

    // Check if memory allocation was successful
    if (new_node == NULL)
    {
        printf("Could not allocate memory!");
        return NULL;
    }

    // Initialize the new node attributes
    new_node->alloc = 10;
    new_node->length = 0;
    new_node->data = (char *)calloc(new_node->alloc, sizeof(char));

    // Check if memory allocation for node data was successful
    if (new_node->data == NULL)
    {
        printf("Could not allocate memory!");
        free(new_node); // Free previously allocated memory for the node
        return NULL;
    }

    // Copy characters from the key to the node's data
    if (key != NULL)
    {
        for (int i = 0; key[i] != '\0'; i++)
        {
            if (new_node->length < (new_node->alloc - 1))
            {
                // Move NULL character from array to the next position
                new_node->data[new_node->length + 1] = new_node->data[new_node->length];
                // Place the new character where the "NULL" character used to be
                new_node->data[new_node->length] = key[i];
                // Increment length of array
                new_node->length++;
            }
            else
            {
                // If all positions are filled in the array, copy the array to a new array with "10" more spaces
                new_node->data = (char *)realloc(new_node->data, new_node->alloc + 10);

                // Check if realloc was successful
                if (new_node->data == NULL)
                {
                    printf("Not enough memory!");
                    free(new_node); // Free previously allocated memory for the node
                    return NULL;    // Return an error indicator
                }

                new_node->alloc += 10;
                // Move NULL character from array to the next position
                new_node->data[new_node->length + 1] = new_node->data[new_node->length];
                // Place the new character where the "NULL" character used to be
                new_node->data[new_node->length] = key[i];
                // Increment length of array
                new_node->length++;
            }
        }
    }
    return new_node;
}
// Return the count of elements in the Hash Map
int __Map_size(struct Map *self)
{
    return self->__count;
}

/// Get the value associated with the given key; return default_value if key is not found
int __Map_get(struct Map *self, const char *key, int default_value)
{
    int bucket = getBucket(key, self->__buckets);

    // Find the MapEntry associated with the key
    MapEntry *retrieve = __Map_find(self, key, bucket);

    // If key is not found, return the default_value
    if (retrieve == NULL)
    {
        return default_value;
    }
    // If key is found, return the value associated with the key
    else
    {
        return retrieve->value;
    }
}

// Find and return the MapEntry associated with the given key in the hash map
MapEntry *__Map_find(struct Map *self, const char *key, int bucket)
{
    // Start from the head of the hash map entry linked list
    MapEntry *ptr = self->__head[bucket];

    // Iterate through the linked list
    while (ptr != NULL)
    {
        // Check if the current node is not NULL and has valid data
        if (ptr != NULL && ptr->key[0] != '\0')
        {
            int i;
            // Iterate through the characters of both the input string and the node's key
            for (i = 0; key[i] && ptr->key[i]; i++)
            {
                // If characters don't match, break the loop
                if (key[i] != ptr->key[i])
                {
                    break;
                }
            }
            // If both strings match until the end, return the pointer to the node
            if (key[i] == '\0' && ptr->key[i] == '\0')
            {
                return ptr;
            }
        }
        // Move to the next node in the hash map
        ptr = ptr->__next;
    }
    // Return NULL if the key is not found in the hash map
    return NULL;
}

// Delete the hash map along with its entries
void __Map_del(struct Map *self)
{
    // Check if the hash map is not NULL
    if (self != NULL)
    {
        MapEntry *current, *next;

        // Iterate through the hash map buckets
        for (int i = 0; i < self->__buckets; i++)
        {
            // Iterate through the linked list in each bucket
            for (current = self->__head[i]; current != NULL; current = next)
            {
                next = current->__next;
                free(current);
            }
        }

        // Free memory allocated for the hash map itself
        free(self);
        self = NULL;
    }
    else
    {
        // Print a message if there is no hash map to delete
        printf("No hash map to delete!\n");
    }
}

// Print the contents of the hash map
void __Map_print(struct Map *self)
{

    if (self == NULL)
    {
        // Print hash map count and an empty dictionary if it's empty
        printf("\nHash Map Count: %d\n{}\n", __Map_size(self));
        return;
    }

    MapEntry *current;

    // Print hash map count
    printf("\nHash Map Count: %d\nBuckets: %d\n", self->__count, self->__buckets);

    // Iterate through the hash map entry linked list and print key-value pairs
    for (int i = 0; i < self->__buckets; i++)
        for (current = self->__head[i]; current != NULL; current = current->__next)
        {
            printf("%s = %d [%d]\n", current->key, current->value, i);
        }
}

// Implementation of creating a new hash map iterator
MapIterator *__Map_iter(Map *self)
{
    // Allocate memory for a new hash map iterator
    MapIterator *iter = (MapIterator *)malloc(sizeof(MapIterator));

    // Check if memory allocation was successful
    if (iter == NULL)
    {
        printf("Could not allocate memory!"); // Print an error message
        self->del(self);                      // Delete the hash map to avoid memory leaks
        return NULL;                          // Return NULL as an indicator of failure
    }

    iter->__map = self;
    iter->__buckets = 0;

    // Set the current hash map entry to the head of the hash map
    iter->__current = self->__head[iter->__buckets];

    // Set function pointers for hash map iterator operations
    iter->next = &__MapIter_next;
    iter->del = &__MapIter_del;

    return iter; // Return the newly created hash map iterator
}

// Implementation of getting the next hash map entry using the hash map iterator
MapEntry *__MapIter_next(MapIterator *self)
{
    MapEntry *retrieve;
    while (self->__current == NULL)
    {
        self->__buckets++;
        if (self->__buckets >= self->__map->__buckets)
        {
            return NULL;
        }
        self->__current = self->__map->__head[self->__buckets];
    }
    retrieve = self->__current;
    if (self->__current != NULL)
    {
        self->__current = self->__current->__next;
    }

    return retrieve;
}

// Implementation of deleting the hash map iterator
void __MapIter_del(MapIterator *self)
{
    free(self); // Free the memory allocated for the hash map iterator
}

// Function to calculate the bucket index for a given key in the hash map
int getBucket(const char *str, int buckets)
{
    unsigned int hash = 123456;
    if (str == NULL)
        return 0;
    for (; *str; str++)
    {
        hash = (hash << 3) ^ *str;
    }
    return hash % buckets;
}
