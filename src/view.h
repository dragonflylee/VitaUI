#ifndef _VITA_VIEW_H_
#define _VITA_VIEW_H_

struct _list_item;
typedef struct _list_item list_item;

// list_view
typedef struct
{
    int num;
    float height; // item height
    list_item *head;
    list_item *cur;
} list_view;

void list_view_init(list_view *view);
void list_view_destroy(list_view *view);
int list_view_render(list_view *view);

#endif // _VITA_VIEW_H_