

#ifndef __EXT_LIST_H__
#define __EXT_LIST_H__


#include <string.h>
#include <stdlib.h>
#include <NuAPI/ExtMutex.h>


struct ExtListInode
{
    struct ExtListInode *prev;
    struct ExtListInode *next;
    void *self;
};


typedef struct
{
    struct ExtListInode *first;
    struct ExtListInode *last;
    ExtMutex mutex;
    size_t size;
}ExtList;


#define extlist_foreach(inode, first_inode)                     \
            for(inode = first_inode; inode; inode = inode->next)

#define extlist_back_foreach(inode, last_inode)                 \
            for(inode = last_inode; inode; inode = inode->prev)


#define extlist_clean_foreach_begin(inode, first_inode)   if(1) {               \
        struct ExtListInode *pinode = first_inode;                              \
        int i = 0;                                                              \
        for(inode = first_inode; inode; pinode = inode, inode = inode->next, ++i) {


#define extlist_clean_foreach_end(list)                \
            if(i && pinode)                                         \
                extlist_del_inode(list, pinode);                    \
        }                                                           \
        if(pinode)                                                  \
            extlist_del_inode(list, pinode);                        \
    }


//void extlist_init(ExtList *list);
void extlist_init_deb(ExtList *list, const char *, int);
void extlist_del_inode_db(ExtList *list, struct ExtListInode *inode, const char *file, int line);

#define extlist_init(list) extlist_init_deb(list, __FILE__, __LINE__)
#define extlist_del_inode(list, inode) extlist_del_inode_db(list, inode, __FILE__, __LINE__)

void extlist_release(ExtList *list);
void extlist_push_back(ExtList *list, void *val);
void extlist_insert(ExtList *list, struct ExtListInode *inode_from, void *val);
//void extlist_del_inode(ExtList *list, struct ExtListInode *inode);
int extlist_pop(ExtList *list, void *value);
void extlist_lock(ExtList *list);
void extlist_unlock(ExtList *list);


#endif
