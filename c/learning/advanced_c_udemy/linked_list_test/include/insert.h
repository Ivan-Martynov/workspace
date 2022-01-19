#ifndef INSERT_INCLUDED
#define INSERT_INCLUDED

struct Node;

struct Node* create_node(const int data);

void b_insert(struct Node** head, int data);
void n_insert(struct Node** head, int data, int pos);
void e_insert(struct Node** head, int data);
)

#endif // INSERT_INCLUDED