#include "user.h"

void initialize(user_list** list, unsigned char behavior) {
    *list = (user_list*) malloc(sizeof(user_list));
    (*list)->behavior = behavior;
    (*list)->dirty = SERVANT_FALSE;
    (*list)->size = 0;
}

void set_user_at(unsigned int index, user_list* list, servant_user* user) {
    list->users[index] = *user;
}

void shift(user_list* list, unsigned int index, unsigned char direction) {
    unsigned int i;

    switch (direction) {
    case RIGHT:
        for (i = list->size; i > index; i--) {
            list->users[i] = list->users[i-1];
        }
        break;

    case LEFT:
        for (i = index+1; i < list->size; i++) {
            list->users[i-1] = list->users[i];
        }
        break;
    }
}

void append(user_list* list, servant_user* new_user) {
    set_user_at(list->size++, list, new_user);
}

void insert_in_order(user_list* list, servant_user* new_user) {
    unsigned int i;

    for (i = 0; i < list->size && strcmp((list->users[i]).username, new_user->username) < 0; i++);

    shift(list, i, RIGHT);
    set_user_at(i, list, new_user);
    list->size++;
}

servant_bool_t add(user_list* list, servant_user* new_user) {
    if (list->size >= MAX_SERVANT_USERS) {
        return SERVANT_FALSE;
    }

    if (list->behavior == ORDERED_LIST) {
        insert_in_order(list, new_user);
    } else {
        append(list, new_user);
    }

    list->dirty = SERVANT_TRUE;

    return SERVANT_TRUE;
}

int sequential_search(user_list* list, char* username) {
    int i;

    for (i = 0; i < list->size && strcmp((list->users[i]).username, username); i++);

    if (i == list->size) {
        return USER_NOT_FOUND;
    }

    return i;
}

int binary_search(user_list* list, char* username) {
    int min, max, mid;
    min = 0;
    max = list->size - 1;

    while (min <= max) {
        mid = (min+max)/2;
        if (strcmp((list->users[mid]).username, username) < 0) {
            min = mid + 1;
        } else if (strcmp((list->users[mid]).username, username) > 0) {
            max = mid - 1;
        } else {
            return mid;
        }
    }

    return USER_NOT_FOUND;
}

int search(user_list* list, char* username) {
    if (list->behavior == ORDERED_LIST) {
        return binary_search(list, username);
    }

    return sequential_search(list, username);

}

servant_bool_t remove_by_username(user_list* list, char* username) {
    int index;

    index = search(list, username);

    if (index == USER_NOT_FOUND) {
        return SERVANT_FALSE;
    }

    shift(list, index, LEFT);
    list->size--;

    list->dirty = SERVANT_TRUE;

    return SERVANT_TRUE;
}

servant_bool_t remove_user(user_list* list, servant_user* user) {
    return remove_by_username(list, user->username);
}

servant_bool_t registered(user_list* list, char* username, char* password) { 
    int index;

	index = search(list, username);

	if (index == USER_NOT_FOUND) {
		return SERVANT_FALSE;
	} else if (!strcmp(list->users[index].password, password)) {
		return SERVANT_TRUE;
	}	

	return SERVANT_FALSE;
}

servant_bool_t has_user(user_list* list, char* username) {
    int index;

    index = search(list, username);

    if (index == USER_NOT_FOUND) {
        return SERVANT_FALSE;
    }

    return SERVANT_TRUE;
}

servant_user get_user(user_list* list, char* username, char* password) { 
    int index;

	index = search(list, username);

	return list->users[index];
}
