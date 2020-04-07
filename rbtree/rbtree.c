/*
 * 红黑树是一种含有红黑结点并能自平衡的二叉查找树。它必须满足下面性质：

	性质1：每个节点要么是黑色，要么是红色。
	性质2：根节点是黑色。
	性质3：每个叶子节点（NIL）是黑色。
	性质4：每个红色结点的两个子结点一定都是黑色。
	性质5：任意一结点到每个叶子结点的路径都包含数量相同的黑结点。
*/
#include <stdio.h>
#include "rbtree.h"

//唯一的一个叶子节点指针，每次fixup的时候，其父节点会随着需要而变化
rbnode NIL={0, COLOR_BLACK, NULL, NULL, NULL};

#define COLOR(node) node->color

rbtree * rbtree_init()
{
	rbtree * tree = malloc(sizeof(rbtree));
	tree->count = 0;
	tree->root = &NIL;
	return tree;
}

void rbtree_node_init(rbnode *node, int key)
{
	node->key = key;
	node->color = COLOR_RED;
	node->parent = NULL;
	node->left = &NIL;
	node->right = &NIL;
}

void rotate_left(rbtree *tree, rbnode *node)
{
	rbnode * parent = node->parent;
	if ( parent->parent ) {
		if ( parent == parent->parent->left ) {
			parent->parent->left = node;
		} else {
			parent->parent->right = node;
		}
	}
	node->parent = parent->parent;
	parent->parent = node;
	node->left->parent = parent;
	parent->right = node->left;
	node->left = parent;
	if ( node->parent == NULL )
		tree->root = node;
}

void rotate_right(rbtree *tree, rbnode *node)
{
	rbnode * parent = node->parent;
	if ( parent->parent ) {
		if ( parent == parent->parent->left ) {
			parent->parent->left = node;
		} else {
			parent->parent->right = node;
		}
	}
	node->parent = parent->parent;
	parent->parent = node;
	node->right->parent = parent;
	parent->left = node->right;
	node->right = parent;
	if ( node->parent == NULL )
		tree->root = node;
}

//只考虑存在右子节点的情况，其他情况不需要考虑
rbnode * successor(rbtree *tree, rbnode *node)
{
	if ( node->right == &NIL )
		return node->right;
	rbnode * snode = node->right;
	while ( snode != &NIL && snode->left != &NIL) {
		snode = snode->left;
	}
	return snode;
}

void fixup_insert(rbtree *tree, rbnode *node)
{
	if ( !node )
		return;
	while ( node->parent && COLOR(node->parent) == COLOR_RED ) {
		rbnode * parent = node->parent;
		if ( parent == parent->parent->left ) {
			rbnode * uncle = parent->parent->right;
			if ( COLOR(uncle) == COLOR_RED ) {
				COLOR(parent) = COLOR_BLACK;
				COLOR(uncle) = COLOR_BLACK;
				COLOR(parent->parent) = COLOR_RED;
				node = parent->parent;
			} else {
				if ( node == parent->right ) {
					rotate_left(tree, node);
				} else 
					node = node->parent;
				rotate_right(tree, node);
				COLOR(node->right) = COLOR_RED;
				COLOR(node) = COLOR_BLACK;
			}
		} else {
			rbnode * uncle = parent->parent->left;
			if ( COLOR(uncle) == COLOR_RED ) {
				COLOR(parent) = COLOR_BLACK;
				COLOR(uncle) = COLOR_BLACK;
				COLOR(parent->parent) = COLOR_RED;
				node = parent->parent;
			} else {
				if ( node == parent->left ) {
					rotate_right(tree, node);
				} else
					node = node->parent;
				rotate_left(tree, node);
				COLOR(node->left) = COLOR_RED;
				COLOR(node) = COLOR_BLACK;
			}
		}
	}
	if ( !node->parent ) {
		COLOR(node) = COLOR_BLACK;
		tree->root = node;
	}
}

void fixup_del(rbtree *tree, rbnode *node)
{
	while ( node!=tree->root && COLOR(node) == COLOR_BLACK ) {
		if ( !node->parent ) {
			break;
		}
		rbnode * parent = node->parent;
		if ( node==node->parent->left ) {
			rbnode * sibling = parent->right;
			if ( COLOR(sibling) == COLOR_RED ) {
				//此时parent肯定是黑色
				COLOR(parent) = COLOR_RED;
				COLOR(sibling) = COLOR_BLACK;
				rotate_left(tree, sibling);
			} else if ( COLOR(sibling) == COLOR_BLACK ) {
				if ( COLOR(sibling->left) == COLOR_BLACK && COLOR(sibling->right) == COLOR_BLACK ) {
					COLOR(sibling) = COLOR_RED;
					node = parent;
				} else if ( COLOR(sibling->left) == COLOR_RED && COLOR(sibling->right) == COLOR_BLACK ) {
					COLOR(sibling) = COLOR_RED;
					COLOR(sibling->left) = COLOR_BLACK;
					rotate_right(tree, sibling->left);
				} else if ( COLOR(sibling->right) == COLOR_RED ) {
					COLOR(sibling) = COLOR_RED;
					COLOR(parent) = COLOR_BLACK;
					COLOR(sibling->right) = COLOR_BLACK;
					rotate_left(tree, sibling);
					node = tree->root;
				}
			}
		} else {
			rbnode * sibling = parent->left;
			if ( COLOR(sibling) == COLOR_RED ) {
				//此时parent肯定是黑色
				COLOR(parent) = COLOR_RED;
				COLOR(sibling) = COLOR_BLACK;
				rotate_right(tree, sibling);
			} else if ( COLOR(sibling) == COLOR_BLACK ) {
				if ( COLOR(sibling->left) == COLOR_BLACK && COLOR(sibling->right) == COLOR_BLACK ) {
					COLOR(sibling) = COLOR_RED;
					node = parent;
				} else if ( COLOR(sibling->right) == COLOR_RED && COLOR(sibling->left) == COLOR_BLACK ) {
					COLOR(sibling) = COLOR_RED;
					COLOR(sibling->right) = COLOR_BLACK;
					rotate_left(tree, sibling->right);
				} else if ( COLOR(sibling->left) == COLOR_RED ) {
					COLOR(sibling) = COLOR_RED;
					COLOR(parent) = COLOR_BLACK;
					COLOR(sibling->left) = COLOR_BLACK;
					rotate_right(tree, sibling);
					node = tree->root;
				}
			}
		}
	}

	COLOR(node) = COLOR_BLACK;
}

rbnode * rbtree_find(rbtree *tree, int key)
{
	rbnode * node = tree->root;
	while (node!=&NIL) {
		if ( key == node->key )
			return node;
		if ( key < node->key ) {
			node = node->left;
		} else {
			node = node->right;
		}
	}
	return NULL;
}

int rbtree_insert(rbtree *tree, rbnode *node)
{
	rbnode ** cur = &tree->root;
	while (*cur != &NIL) {
		if ( (*cur)->key == node->key )
			return -1;
		node->parent = *cur;
		if ( node->key < (*cur)->key ) {
			cur = &(*cur)->left;
		} else {
			cur = &(*cur)->right;
		}
	}
	if ( cur == &tree->root )
		node->color = COLOR_BLACK;
	*cur = node;

	fixup_insert(tree, node);

	return 0;
}

void rbtree_del(rbtree *tree, rbnode *node)
{
	if ( !node )
		return;
	int color_del = COLOR_RED;
	rbnode * node_to_fixup = &NIL;
	rbnode * node_to_del = &NIL;
	if ( node->left == &NIL && node->right == &NIL ) {
		//删掉node节点
		color_del = node->color;
		node_to_del = node;
		node_to_fixup = &NIL;
		node_to_fixup->parent = node_to_del->parent;
		//摘除node并且将node_to_fixup挂载
		rbnode * parent = node->parent;
		if ( !parent )
			tree->root = node_to_fixup;
		else if ( node == parent->left )
			parent->left = node_to_fixup;
		else 
			parent->right = node_to_fixup;
	} else if ( node->right == &NIL ) {
		//删掉node节点
		color_del = node->color;
		node_to_del = node;
		node_to_fixup = node->left;
		node_to_fixup->parent = node_to_del->parent;
		//调整node的父节点的孩子指针
		rbnode * parent = node->parent;
		if ( !parent )
			tree->root = node_to_fixup;
		else if ( node == parent->left )
			parent->left = node_to_fixup;
		else 
			parent->right = node_to_fixup;
	} 
	else {
		//1,找到真正要删除的节点：后继节点（实际上是将这个节点和node节点互换后删掉node），确定fixup节点
		node_to_del = successor(tree, node);
		node_to_fixup = node_to_del->right;
		
		//2,用node_to_fixup节点替换node_to_del节点
		//2.1,更改node_to_fixup父节点指针
		if ( node != node_to_del->parent )
			node_to_fixup->parent = node_to_del->parent;
		else
			node_to_fixup->parent = node_to_del;
		//2.2,更改node_to_fixup父节点的孩子指针
		if ( node_to_del == node_to_del->parent->left )
			node_to_del->parent->left = node_to_fixup;
		else
			node_to_del->parent->right = node_to_fixup;

		//3,用node_to_del替换node.
		//3.1,复制node，除了key,将node_to_del替换node节点
		int key = node_to_del->key;
		color_del = node_to_del->color;
		*node_to_del = *node;
		node_to_del->key = key;
		//3.2,调整node的孩子指针指向新的父亲节点node_to_del
		if ( node_to_del->left != &NIL )
			node_to_del->left->parent = node_to_del;
		if ( node_to_del->right != &NIL )
			node_to_del->right->parent = node_to_del;

		//3.2,调整node_to_del的父节点的孩子指针
		rbnode * parent = node_to_del->parent;
		if ( !parent )
			tree->root = node_to_del;
		else if ( node == parent->left )
			parent->left = node_to_del;
		else 
			parent->right = node_to_del;
	} 
	
	if ( color_del == COLOR_BLACK )
		fixup_del(tree, node_to_fixup);
}

void rbtree_print_node(rbnode *node, int left_or_right) {
	if ( node == &NIL )
		return;
	
	printf("%s %d(%c)%d\t", left_or_right==1?"left":"right", \
			node->key, COLOR(node)==COLOR_RED?'r':'b', node->parent?node->parent->key:-1);
	rbtree_print_node(node->left, 1);
	rbtree_print_node(node->right, 2);
	printf("\n");
}

void rbtree_print(rbtree *tree) {
	rbnode * cur = tree->root;
	rbtree_print_node(cur, 0);
}

void rbtree_destroy(rbtree *tree)
{
	free(tree);
}

bool check_root(rbtree *tree)
{
	if ( COLOR(tree->root) != COLOR_BLACK )
		return false;
	return true;
}

bool check_color(rbnode *node)
{
	if ( !node )
		return true;
	if ( COLOR(node) != COLOR_BLACK && COLOR(node) != COLOR_RED )
		return false;
	return check_color(node->left) && check_color(node->right);
}

bool check_child(rbnode *node)
{
	if ( !node )
		return true;
	if ( COLOR(node) == COLOR_RED ) {
		if ( COLOR(node->left) != COLOR_BLACK || COLOR(node->right) != COLOR_BLACK )
			return false;
	}
	return check_child(node->left) && check_child(node->right);
}

bool check_leaf_color(rbnode *node)
{
	if ( !node )
		return true;
	if ( node == &NIL && COLOR(node) != COLOR_BLACK )
		return false;
	return check_leaf_color(node->left) && check_leaf_color(node->right);
}

int check_black_depth(rbnode *node)
{
	if ( node == &NIL )
		return 1;
	int left_black_depth = check_black_depth(node->left);
	int right_black_depth = check_black_depth(node->right);
	printf("left_black_depth=%d right_black_depth=%d\n", left_black_depth, right_black_depth);
	if ( left_black_depth < 0 || right_black_depth < 0 )
		return -1;
	if ( left_black_depth != right_black_depth ) {
		printf("error node=%d\n", node->key);
		return -1;
	}

	return left_black_depth + (COLOR(node)==COLOR_BLACK?1:0);
}

void rbtree_check(rbtree *tree)
{
	bool rtn = check_root(tree);
	printf("check root color:%s\n", rtn?"ok":"fail");
	rtn = check_color(tree->root);
	printf("check color:%s\n", rtn?"ok":"fail");
	rtn = check_child(tree->root);
	printf("check child color of red node:%s\n", rtn?"ok":"fail");
	rtn = check_leaf_color(tree->root);
	printf("check leaf color:%s\n", rtn?"ok":"fail");
	int depth = check_black_depth(tree->root);
	printf("depth=%d\n", depth);
	printf("check_black_depth:%s\n", depth>0?"ok":"fail");
}
