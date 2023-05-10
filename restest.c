#include "rbtree.h"
#include <stdio.h>
#include <stdlib.h>

rbtree *new_rbtree(void) {
  rbtree *p = (rbtree *)calloc(1, sizeof(rbtree));
  // TODO: initialize struct if needed
  p->nil = (node_t *)calloc(1,sizeof(node_t));
  p->root = p->nil;
  p->nil->color = RBTREE_BLACK;
  return p;
}
void postorder(rbtree *t,node_t *delete_node){
    if(delete_node == NULL || delete_node == t->nil){
        return;
    }
    postorder(t,delete_node->right);
    postorder(t,delete_node->left);
    if(delete_node != NULL){
        free(delete_node);
    }
}
void delete_rbtree(rbtree *t) {
  // TODO: reclaim the tree nodes's memory
  postorder(t,t->root);
  free(t->nil);
  free(t);
}
void rotate_left(rbtree *t,node_t *x){
    node_t *y = x->right; // 현재노드의 오른쪽 노드 포인터
    x->right = y->left; // 왼쪽으로 돌리는것. 돌리는 노드 자식 왼쪽 노드가 기존 노드의 오른쪽에 붙는다.
    
    if (y->left != t->nil) { // 원래 자손노드가 닐노드가 아니면
        y->left->parent = x; // 위치 로테이션
    }
    y->parent = x->parent; // 조상 갱신
    if (x->parent == t->nil){
        t->root = y;
    }
    else if(x == x->parent->left) { // 위에서 처리 못한 x 노드도 붙여준다.
        x->parent->left = y;
    }
    else {
        x->parent->right = y;
    }
    y->left = x;
    x->parent = y;
}
// 왼쪽 함수와 반대로 돌기
void rotate_right(rbtree *t,node_t *y){
    node_t *x = y->left;
    y->left = x->right;
    // 왼쪽 
    if (x->right != t->nil) {
        x->right->parent = y;
    }
    // 
    x->parent = y->parent;
    if (y->parent == t->nil) {
        t->root = x;
    }
    else if(y == y->parent->right) {
        y->parent->right = x;
    }
    else {
        y->parent->left = x;
    }
    x->right = y;
    y->parent = x;
}
// 삽입 밸런스 팩터
void insert_fixup(rbtree* t,node_t *z){
    // 함수 실행전 불변식은 3가지
    // 1. 노드 z는 RED이다.
    // 2. z의 부모가 루트면 z의 부모는 Black
    // 3. 레드 블랙트리의 특성 위반의 경우 1가지만 위반하는 경우는 2,4의 특성이다. 2가 위반되면 z가 루트이고 적색이고 4가 위반되면 z와 z의 부모가 모두 적색이다.
    while (z->parent->color == RBTREE_RED){
        // 가족(결합)이면 진행한다.
        if (z->parent == z->parent->parent->left) {
            node_t *y = z->parent->parent->right;
            if (y->color == RBTREE_RED) {   // Case 1 삼촌 노드가 적색인경우
                z->parent->color = RBTREE_BLACK;
                y->color = RBTREE_BLACK;
                z->parent->parent->color = RBTREE_RED;
                z = z->parent->parent;
            }
            else{
                if(z == z->parent->right){ // Case 2 삼촌 y가 흑색이며, z가 오른쪽 자식인 경우
                    z = z->parent;
                    rotate_left(t,z);
                }
                z->parent->color = RBTREE_BLACK; // Case 3 삼촌 y가 흑색이며, z가 왼쪽 자식인 경우
                z->parent->parent->color = RBTREE_RED;
                rotate_right(t,z->parent->parent);
            }
        }
        // 반대로수행
        else{
            node_t *y = z->parent->parent->left;
            if (y->color == RBTREE_RED) {
                z->parent->color = RBTREE_BLACK;
                y->color = RBTREE_BLACK;
                z->parent->parent->color = RBTREE_RED;
                z = z->parent->parent;
            }
            else{
                if(z == z->parent->left){
                    z = z->parent;
                    rotate_right(t,z);
                }
                z->parent->color = RBTREE_BLACK;
                z->parent->parent->color = RBTREE_RED;
                rotate_left(t,z->parent->parent);
            }
        }
    }
    t->root->color = RBTREE_BLACK;
}
// 삽입
node_t *rbtree_insert(rbtree *t, const key_t key) {
    node_t *z = (node_t *)calloc(1,sizeof(node_t));
    z->key = key;
    node_t *y = t->nil;
    node_t *x = t->root;
    // 크기를 확인하고 왼쪽 오른쪽
    while (x != t->nil){
        y = x;
        if (z->key < x->key){
            x = x->left;
        }
        else x = x->right;
    }
    z->parent = y;
    if(y == t->nil){
        t->root = z;
    }
    else if(z->key < y->key){
        y->left = z;
    }
    else {
        y->right = z;
    }
    z->left = t->nil;
    z->right = t->nil;
    z->color = RBTREE_RED;
    insert_fixup(t,z);
    return z;
}

node_t *rbtree_find(const rbtree *t, const key_t key) {
  // TODO: implement find
  // 탐색 노드를 하나 생성해서 루트 노드에서 부터 탐색
  node_t *ptr = t->root;
  while(ptr != t->nil && key != ptr->key){
    if (key < ptr->key){
        ptr = ptr->left;
    }
    else {
        ptr = ptr->right;
    }
  }
  if (ptr == t->nil) {
    return NULL;
  }
  return ptr;
}
// 가장 작은 노드 반환
node_t *tree_minimum(node_t *x,node_t *nil){
    while (x->left != nil){
        x = x->left;
    }
    return x;
}
node_t *rbtree_min(const rbtree *t) {
  // TODO: implement find
  node_t *realy_min;
  realy_min = tree_minimum(t->root,t->nil);
  return realy_min;
}
// 가장 큰 노드 반환
node_t *rbtree_max(const rbtree *t) {
  // TODO: implement find
  node_t *realy_max = t->root;
  while (realy_max->right != t->nil){
        realy_max = realy_max->right;
    }
  return realy_max;
}
// 부모와 위치 변환 함수
void rb_transplant(rbtree *t,node_t *n1,node_t *n2) {
    if(n1->parent == t->nil){
        t->root = n2;
    }
    else if(n1 == n1->parent->left){
        n1->parent->left = n2;
    }
    else {
        n1->parent->right = n2;
    }
    n2->parent = n1->parent;
}
// 삭제 밸런스 팩터
void rb_delete_fixup(rbtree *t,node_t *x){
    while (x != t->root && x->color == RBTREE_BLACK){
        if(x == x->parent->left){
            node_t *w = x->parent->right;
            if(w->color == RBTREE_RED){
                w->color = RBTREE_BLACK;        //  Case 1 x의 형제 w가 적색인 경우
                x->parent->color = RBTREE_RED;
                rotate_left(t,x->parent);
                w = x->parent->right;
            }
            if (w->left->color == RBTREE_BLACK && w->right->color == RBTREE_BLACK){
                w->color = RBTREE_RED;          // Case2  x의 형제 w는 흑색이고 w의 두 자식이 모두 흑색인 경우
                x = x->parent;
            }
            else{ 
                if(w->right->color == RBTREE_BLACK){
                    w->left->color = RBTREE_BLACK;      // Case3    x의 형제 w는 흑색w의 오니쪽 자식은 RED, w의 오른쪽 자식은 Black
                    w->color = RBTREE_RED;
                    rotate_right(t,w);
                    w = x->parent->right;
                }
                w->color = x->parent->color;        // Case4 x의 형제 w는 흑색이고 w의 오른쪽 자식은 RED인 경우
                x->parent->color = RBTREE_BLACK;
                w->right->color = RBTREE_BLACK;
                rotate_left(t,x->parent);
                x = t->root;
            }
        }
        else {
            // 반대 상황일 때
            node_t *w = x->parent->left;
            if(w->color == RBTREE_RED){
                w->color = RBTREE_BLACK;
                x->parent->color = RBTREE_RED;
                rotate_right(t,x->parent);
                w = x->parent->left;
            }
            if (w->right->color == RBTREE_BLACK && w->left->color == RBTREE_BLACK){
                w->color = RBTREE_RED;
                x = x->parent;
            }
            else{ 
                if(w->left->color == RBTREE_BLACK){
                    w->right->color = RBTREE_BLACK;
                    w->color = RBTREE_RED;
                    rotate_left(t,w);
                    w = x->parent->left;
                }
                w->color = x->parent->color;
                x->parent->color = RBTREE_BLACK;
                w->left->color = RBTREE_BLACK;
                rotate_right(t,x->parent);
                x = t->root;
            }
        }
    }
    x->color = RBTREE_BLACK;
}
// 삭제 함수
int rbtree_erase(rbtree *t, node_t *p) {
  // TODO: implement erase
  node_t *x;
  node_t *y = p;
  int orginal_color = y->color;
  if(p->left == t->nil){
    x = p->right;
    rb_transplant(t,p,p->right); // 두 노드의 위치 변환
  }
  else if(p->right == t->nil){
    x = p->left;
    rb_transplant(t,p,p->left);
  }
  else{
    y = tree_minimum(p->right,t->nil);
    orginal_color = y->color;
    x = y->right;
    if(y->parent == p){
        x->parent = y;
    }
    else{
        rb_transplant(t,y,y->right);
        y->right = p->right;
        y->right->parent = y;
    }
    rb_transplant(t,p,y);
    y->left = p->left;
    y->left->parent = y;
    y->color = p->color;
  }
  if (orginal_color == RBTREE_BLACK){
    rb_delete_fixup(t,x);
  }
  free(p);
  return 0;
}
void inorder(const rbtree *t,node_t *search, key_t *arr,int* index){
    if (search == t->nil) return;
    inorder(t,search->left,arr,index);
    arr[(*index)++] = search->key;
    printf("%d->",search->key);
    inorder(t,search->right,arr,index);
}
// 배열 출력
int rbtree_to_array(const rbtree *t, key_t *arr, const size_t n) {
  // TODO: implement to_array
  node_t *search_node = t->root;
  int *index = calloc(1,sizeof(int));
  inorder(t,search_node,arr,index);
  free(index);
  return 0;
}