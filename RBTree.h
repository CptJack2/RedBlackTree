#pragma once
template<class K,class V>
class RBTreeNode{
public:
	K key;
	RBTreeNode* left;
	RBTreeNode* right;
	RBTreeNode* parent;
	V value;
	char color;
	RBTreeNode(K k, V v):key(k), value(v), left(nullptr), right(nullptr), parent(nullptr), color('r') {}
	RBTreeNode(K k, V v, RBTreeNode* p) :key(k), value(v), left(nullptr), right(nullptr), parent(p), color('r') {}
	void build_left(K k, V v) {left=new RBTreeNode<K,V>(k, v,this);}
	void build_right(K k, V v) { right = new RBTreeNode<K, V>(k, v, this); }
};
template<class K,class V>
class RBTreeMap {
	RBTreeNode<K,V>* root;
	int _size;
	void insert_fixup(RBTreeNode<K, V>*);
	void left_rotate(RBTreeNode<K, V>*);
	void right_rotate(RBTreeNode<K, V>*);
	friend void test(RBTreeMap& m);
	RBTreeNode<K, V>* get_node_ptr(const K& k);
	void remove_fixup(RBTreeNode<K, V>*);
public:
	RBTreeMap():_size(0),root(nullptr){}
	void insert(const K& k,const V& v);
	void remove(const K&);
	bool has_key(const K& k);
	int size() { return _size; }
};
template<class K, class V>
void RBTreeMap<K, V>::remove_fixup(RBTreeNode<K, V>* x) {
	while (x != root && x->color == 'b') {//x为黑+黑节点且不为root（为root直接设置root为黑，为红+黑直接设为红即可）
		if (x == x->parent->left) {//若x是左孩子
			RBTreeNode<K, V>* w = x->parent->right;//w储存x的兄弟
			//case1，x为黑，w为红，px为黑，wl/wr为黑
			//处理完后，为黑+黑，红父，黑兄弟的case2/3/4
			if (w->color == 'r') {
				w->color = 'b';//将兄弟设为黑
				x->parent->color = 'r';//将父亲设为红
				left_rotate(x->parent);//对父亲左旋
				w = x->parent->right;//重新设置x的兄弟
			}
			//case2，x为黑+黑，w为黑，wl/wr都为黑
			//处理完后，右侧黑数-1，可视为x的父节点+黑
			if ((!w->left|| w->left->color == 'b' )&&( !w->right|| w->right->color == 'b')) {
				w->color = 'r';//兄弟设为红
				x = x->parent;//x转为其父节点
			}
			else{ 
				//case3,x黑+黑，w黑，wl红，wr黑
				//处理完后，变为黑+黑，黑兄弟，兄弟右子为红的case4
				if (!w->right||w->right->color == 'b') {
					w->left->color = 'b';//wl变黑
					w->color = 'r';//w变红
					right_rotate(w);//w右旋
					w = x->parent->right;//重置x的兄弟
				}
				//case4，黑+黑，黑兄弟，兄弟右子为红，左子随意颜色的case4
				//处理完后，x丢弃多余的黑色，左右已平衡
				w->color = x->parent->color;//将px颜色给予w
				x->parent->color = 'b';//px改为黑
				w->right->color = 'b';//wr改为黑
				left_rotate(x->parent);//左旋px
				x = root;//调整完成
			}
		}
		else {
			if (x == x->parent->right) {//若x是右孩子
				RBTreeNode<K, V>* w = x->parent->left;//w储存x的兄弟
				//case1，x为黑，w为红，px为黑，wl/wr为黑
				//处理完后，为黑+黑，红父，黑兄弟的case2/3/4
				if (w->color == 'r') {
					w->color = 'b';//将兄弟设为黑
					x->parent->color = 'r';//将父亲设为红
					right_rotate(x->parent);//对父亲右旋
					w = x->parent->left;//重新设置x的兄弟
				}
				//case2，x为黑+黑，w为黑，wl/wr都为黑
				//处理完后，右侧黑数-1，可视为x的父节点+黑
				if ((!w->left || w->left->color == 'b') && (!w->right || w->right->color == 'b')) {
					w->color = 'r';//兄弟设为红
					x = x->parent;//x转为其父节点
				}
				else {
					//case3,x黑+黑，w黑，wl红，wr黑
					//处理完后，变为黑+黑，黑兄弟，兄弟左子为红的case4
					if (!w->left||w->left->color == 'b') {
						w->right->color = 'b';//wl变黑
						w->color = 'r';//w变红
						left_rotate(w);//w左旋
						w = x->parent->left;//重置x的兄弟
					}
					//case4，黑+黑，黑兄弟，兄弟左子为红，右子随意颜色的case4
					//处理完后，x丢弃多余的黑色，左右已平衡
					w->color = x->parent->color;//将px颜色给予w
					x->parent->color = 'b';//px改为黑
					w->left->color = 'b';//wl改为黑
					right_rotate(x->parent);//右旋px
					x = root;//调整完成
				}			}		}	}
	x->color = 'b';//将黑+红节点直接设置为红即可
}
template<class K, class V>
void RBTreeMap<K, V>::remove(const K& k) {
	RBTreeNode<K, V>* z = get_node_ptr(k),*y,*x;//z代表存储要删除的key的节点，y代表实际被删除的节点（将y的kv移动到z）
	if (!z->left || !z->right)//如果z只有一颗子树或者为叶子
		y = z;
	else {//如果z有两颗子树，找它的后继（右子树的最左节点）
		y = z->right;
		while (y->left)
			y = y->left;
		z->key = y->key;
		z->value = y->value;
	}
	//此时的y只能有0/1个子树，用x储存y的子树
	if (y->left)
		x = y->left;
	else
		x = y->right;
	//如果被删节点是叶子,且不为单独一个根，将其假想为一个黑NULL节点
	if (!y->left && !y->right && y!=root) {
		if(y->color=='b')
			remove_fixup(y);
		//从树中删除y
		if (y == y->parent->left)
			y->parent->left = nullptr;
		else
			y->parent->right = nullptr;
		--_size;
		delete y;
	}
	else {//被删除的节点只有一个子树
		//从树中删除y
		if(x)
			x->parent = y->parent;
		if (!y->parent)//若被删除的是根节点，则要更新root
			root = x;
		else if (y == y->parent->left)
			y->parent -> left = x;
		else
			y->parent->right = x;
		--_size;
		if (root&& y->color == 'b')//如果被删除节点为黑色，才需调整，为红色直接删除即可
				remove_fixup(x);
		delete y;
	}
}
template<class K, class V>
bool RBTreeMap<K, V>::has_key(const K& k) {
	if (get_node_ptr(k))
		return true;
	else
		return false;
}
template<class K, class V>
RBTreeNode<K, V>* RBTreeMap<K, V>::get_node_ptr(const K& k) {
	RBTreeNode<K, V>* p = root;
	while (p && p->key != k) {
		if (k > p->key)
			p = p->right;
		else if (k < p->key)
			p = p->left;
	}
	return p;
}
template<class K, class V>
void RBTreeMap<K, V>::insert(const K& k, const V& v) {
	//若树尚未建立
	if (_size == 0) {
		root = new RBTreeNode<K, V>(k, v);
		root->color = 'b';
		++_size;
		return;
	}
	//找到对应插入的叶子节点
	RBTreeNode<K, V>* p = root,*t;
	while (1) {
		if (k < p->key)
			if (p->left)
				p = p->left;
			else{
				p->build_left(k, v);
				t = p->left;
				break;
			}
		else if (k>p->key)
			if (p->right)
				p = p->right;
			else {
				p->build_right(k, v);
				t = p->right;
				break;
			}
		else//相同key值，放弃插入
			return;
	}
	//在叶子节点插入
	//RBTreeNode<K, V>* t;
	//if (k < p->key){
	//	p->build_left(k, v);
	//	t = p->left;}
	//else if (k>p->key){
	//	p->build_right(k,v);
	//	t = p->right;}
	//else//相同key值，放弃插入
	//	return;
	++_size;
	insert_fixup(t);
}
template<class K, class V>
void RBTreeMap<K, V>::insert_fixup(RBTreeNode<K, V>* z) {
	while (z->parent && z->parent->color == 'r') {// 若“当前节点(z)的父节点是红色”，则进行以下处理。(若到达根节点，直接退出即可）
		if (z->parent == z->parent->parent->left) {// 若“z的父节点”是“z的祖父节点的左孩子”，则进行以下处理。
			RBTreeNode<K, V>* y = z->parent->parent->right;// 将y设置为“z的叔叔节点(z的祖父节点的右孩子)”
			if (y&& y->color == 'r') {// Case 1条件：叔叔是红色
				z->parent->color = 'b';//  (01) 将“父节点”设为黑色
				y->color = 'b';//  (02) 将“叔叔节点”设为黑色。
				z->parent->parent->color='r';//  (03) 将“祖父节点”设为“红色”。
				z = z->parent->parent;//  (04) 将“祖父节点”设为“当前节点”(红色节点)
			}
			else{//若叔叔不存在，则归为叔叔为黑节点的情况
				//将左->右的情况化为左->左
				if (z == z->parent->right) {// Case 2条件：叔叔是黑色，且当前节点是右孩子
					z = z->parent; //  (01) 将“父节点”作为“新的当前节点”。
					left_rotate(z);//  (02) 以“新的当前节点”为支点进行左旋。
				}
				//处理左->左的情况
				z->parent->color = 'b';// Case 3条件：叔叔是黑色，且当前节点是左孩子。(01) 将“父节点”设为“黑色”。
				z->parent->parent->color = 'r';//  (02) 将“祖父节点”设为“红色”。
				right_rotate(z->parent->parent); //  (03) 以“祖父节点”为支点进行右旋。
			}
		}
		else {// 若“z的父节点”是“z的祖父节点的右孩子”，则进行以下处理。
			RBTreeNode<K, V>* y = z->parent->parent->left;// 将y设置为“z的叔叔节点(z的祖父节点的左孩子)”
			if (y&& y->color == 'r') {// Case 1条件：叔叔是红色
				z->parent->color = 'b';//  (01) 将“父节点”设为黑色
				y->color = 'b';//  (02) 将“叔叔节点”设为黑色。
				z->parent->parent->color = 'r';//  (03) 将“祖父节点”设为“红色”。
				z = z->parent->parent;//  (04) 将“祖父节点”设为“当前节点”(红色节点)
			}
			else {//若叔叔不存在，则归为叔叔为黑节点的情况
				//将右->左的情况化为右->右
				if (z == z->parent->left) {// Case 2条件：叔叔是黑色，且当前节点是左孩子
					z = z->parent; //  (01) 将“父节点”作为“新的当前节点”。
					right_rotate(z);//  (02) 以“新的当前节点”为支点进行右旋。
				}
				//处理右->右的情况
				z->parent->color = 'b';// Case 3条件：叔叔是黑色，且当前节点是右孩子。(01) 将“父节点”设为“黑色”。
				z->parent->parent->color = 'r';//  (02) 将“祖父节点”设为“红色”。
				left_rotate(z->parent->parent); //  (03) 以“祖父节点”为支点进行右旋。
			}
		}
	}
	root->color = 'b';//根节点染为黑色
}
template<class K, class V>
void RBTreeMap<K, V>::left_rotate(RBTreeNode<K, V>* t) {
	//将t的parent改为t->right的parent
	RBTreeNode<K, V>* tmp = t->parent;
	if (t->parent) {
		if (t == t->parent->left) {
			t->parent->left = t->right;
		}
		else {
			t->parent->right = t->right;
		}
	}
	t->parent = t->right;
	t->right->parent = tmp;
	//将t->right的左子树改为t的右子树
	tmp = t->right->left;
	t->right->left = t;
	t->right = tmp;
	if(tmp)
		tmp->parent = t;
	//如果根节点被旋转，修改根节点指针
	if (root == t)
		root = t->parent;
}
template<class K, class V>
void RBTreeMap<K, V>::right_rotate(RBTreeNode<K, V>* t) {
	//将t的parent改为t->left的parent
	RBTreeNode<K, V>* tmp = t->parent;
	if (t->parent) {
		if (t == t->parent->left) {
			t->parent->left = t->left;
		}
		else {
			t->parent->right = t->left;
		}
	}
	t->parent = t->left;
	t->left->parent = tmp;
	//将t->left的右子树改为t的左子树
	tmp = t->left->right;
	t->left->right = t;
	t->left = tmp;
	if (tmp)
		tmp->parent = t;
	//如果根节点被旋转了，修改根节点指针
	if (root == t)
		root = t->parent;
}
