/******************************************************************************
                        a Light weight hash table in c
                            Author: manu madanan
                            Version: 1.0.0
*******************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

// increasing this value will took more memory 
// decreasing this value will increase the hash collision 
#define HASH_MAP_ROOT_SIZE 1000

#define foreach_map(map, map_dt)                                    \
        for(int i=0;i<HASH_MAP_ROOT_SIZE;i++){                      \
        if(map->map_list[i].object!=NULL){                          \
            struct hash_map_item* xmap_item = &map->map_list[i];    \
             while(xmap_item!=NULL){                                \
                map_dt =xmap_item->object;                       

#define end_map                                                     \
                xmap_item = xmap_item->sub_item;                    \
            }                                                       \
        }                                                           \
    }
    

struct hash_map_item{
    char key[100];
    void* object;
    struct hash_map_item* sub_item;
};

struct hash_map{
    int object_size;
    struct hash_map_item map_list[HASH_MAP_ROOT_SIZE];
    void (*put)(struct hash_map* map, char* key, void* obj);
    void* (*get)(struct hash_map* map, char* key);
    int (*get_hash_value)(char* key);
};

// The hash function
static int the_hash_function(char* key){
    //1. finding the ascii sums
    int key_digit = 0;
    while(*key){
        key_digit = key_digit + *key; 
        key++;
    }
    //printf("the raw hash key is: %d\r\n", key_digit);
    key_digit = key_digit % (HASH_MAP_ROOT_SIZE - 1);
    return key_digit;
}

void destroy_map(struct hash_map* map){
    
}

static void push_hashmap_value(struct hash_map* map, char* key, void* obj){
    //printf("the data is pushing to the map\r\n");
    int hash_key = the_hash_function(key);
    if(map->map_list[hash_key].object==NULL){
        strcpy(map->map_list[hash_key].key, key);
        map->map_list[hash_key].object = calloc(1, map->object_size);
        memcpy(map->map_list[hash_key].object, obj, map->object_size);
    }else{ // for hash collision
        struct hash_map_item* obj_ptr = &map->map_list[hash_key];
        struct hash_map_item* obj_prev = obj_ptr;
        // case 1 serach if same key found, replace the data
        while(obj_ptr!=NULL){
            obj_prev = obj_ptr;
            // key collision - so replace the valuse
            if(strcmp(obj_ptr->key, key)==0){
                //printf("Key collison found@@@@\r\n");
                memcpy(obj_ptr->object, obj, map->object_size);
                return;
            }
            obj_ptr = obj_ptr -> sub_item;
        }
        
        // case 2 (hash collision only)insert to the last of the collided linked list
        obj_prev->sub_item = calloc(1, sizeof(struct hash_map_item));
        obj_prev->sub_item->object = calloc(1, map->object_size);
        obj_prev->sub_item->sub_item = NULL;
        strcpy(obj_prev->sub_item->key, key);
        memcpy(obj_prev->sub_item->object, obj, map->object_size);
    }
}

static void* get_hashmap_value(struct hash_map* map, char* key){
    int hash_key = the_hash_function(key);
    if(map->map_list[hash_key].object==NULL){
        return NULL; 
    }
    // searching in the sublist for the match
    struct hash_map_item* map_item = &map->map_list[hash_key];
    while(map_item!=NULL){
        if(strcmp(map_item->key, key)==0){
            return map_item->object;
        }
        map_item = map_item->sub_item;
    }
    return NULL;
    
}

struct hash_map* create_hashmap(int object_size){
    struct hash_map* map = calloc(1, sizeof(struct hash_map));
    map->object_size = object_size;
    map->put = push_hashmap_value;
    map->get = get_hashmap_value;
    return map;
}


int main(){
    struct hash_map* map = create_hashmap(sizeof(int));
    int a=1;
    int b=2;
    int c=3;
    int m=34;
    map->put(map, "first", &a);
    map->put(map, "second", &b);
    map->put(map, "third", &c);
    map->put(map, "firts", &m);
    
    // Print all the values
    void *res;
    foreach_map(map, res)
        int res_int  = *(int*)res;
        printf("The result is: %d\r\n", res_int);
    end_map
    
    // Print a specific value
    res = map->get(map, "first");
    int res_int  = *(int*)res;
    printf("The result of key first is: %d\r\n", res_int);
}
